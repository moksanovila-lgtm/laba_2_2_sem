#include "SequenceWidget.hpp"
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>

SequenceWidget::SequenceWidget(SequenceController* ctrl, QWidget* parent)
    : QWidget(parent), controller(ctrl), selectedStart(-1), selectedEnd(-1), hasStart(false) {
    setupUI();
    connect(controller, &SequenceController::dataChanged, this, &SequenceWidget::updateDisplay);
    connect(controller, &SequenceController::error, this, &SequenceWidget::onError);
    connect(controller, &SequenceController::newSequenceCreated, this, &SequenceWidget::newSequenceCreated);
    updateDisplay();
}

SequenceWidget::~SequenceWidget() {
    delete controller;
}

bool SequenceWidget::isBitSequence() const {
    int type = controller->getType();
    return (type == 5 || type == 6);
}

void SequenceWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    infoLabel = new QLabel();
    infoLabel->setStyleSheet("QLabel { background-color: #e0e0e0; padding: 8px; font-weight: bold; }");
    mainLayout->addWidget(infoLabel);
    
    listWidget = new QListWidget();
    listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    mainLayout->addWidget(listWidget);
    
    QHBoxLayout* inputLayout = new QHBoxLayout();
    valueInput = new QLineEdit();
    valueInput->setPlaceholderText("Enter value...");
    appendBtn = new QPushButton("Append");
    prependBtn = new QPushButton("Prepend");
    inputLayout->addWidget(valueInput);
    inputLayout->addWidget(appendBtn);
    inputLayout->addWidget(prependBtn);
    mainLayout->addLayout(inputLayout);
    
    QHBoxLayout* opLayout = new QHBoxLayout();
    mapBtn = new QPushButton("Map");
    whereBtn = new QPushButton("Where");
    reduceBtn = new QPushButton("Reduce");
    concatBtn = new QPushButton("Concat");
    subseqBtn = new QPushButton("Get Subsequence");
    opLayout->addWidget(mapBtn);
    opLayout->addWidget(whereBtn);
    opLayout->addWidget(reduceBtn);
    opLayout->addWidget(concatBtn);
    opLayout->addWidget(subseqBtn);
    mainLayout->addLayout(opLayout);
    
    QHBoxLayout* bitLayout = new QHBoxLayout();
    secondSeqInput = new QLineEdit();
    secondSeqInput->setPlaceholderText("Вторая последовательность (например, 11010)");
    andBtn = new QPushButton("AND");
    orBtn = new QPushButton("OR");
    xorBtn = new QPushButton("XOR");
    notBtn = new QPushButton("NOT");
    bitLayout->addWidget(secondSeqInput);
    bitLayout->addWidget(andBtn);
    bitLayout->addWidget(orBtn);
    bitLayout->addWidget(xorBtn);
    bitLayout->addWidget(notBtn);
    mainLayout->addLayout(bitLayout);
    
    connect(appendBtn, &QPushButton::clicked, this, &SequenceWidget::onAppend);
    connect(prependBtn, &QPushButton::clicked, this, &SequenceWidget::onPrepend);
    connect(mapBtn, &QPushButton::clicked, this, &SequenceWidget::onMap);
    connect(whereBtn, &QPushButton::clicked, this, &SequenceWidget::onWhere);
    connect(reduceBtn, &QPushButton::clicked, this, &SequenceWidget::onReduce);
    connect(concatBtn, &QPushButton::clicked, this, &SequenceWidget::onConcat);
    connect(subseqBtn, &QPushButton::clicked, this, &SequenceWidget::onSubsequence);
    connect(andBtn, &QPushButton::clicked, this, &SequenceWidget::onAnd);
    connect(orBtn, &QPushButton::clicked, this, &SequenceWidget::onOr);
    connect(xorBtn, &QPushButton::clicked, this, &SequenceWidget::onXor);
    connect(notBtn, &QPushButton::clicked, this, &SequenceWidget::onNot);
    connect(listWidget, &QListWidget::itemClicked, this, &SequenceWidget::onItemClicked);
    connect(listWidget, &QListWidget::customContextMenuRequested, this, &SequenceWidget::showContextMenu);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &SequenceWidget::onItemDoubleClicked);
    
    secondSeqInput->setVisible(false);
    andBtn->setVisible(false);
    orBtn->setVisible(false);
    xorBtn->setVisible(false);
    notBtn->setVisible(false);
}

void SequenceWidget::updateDisplay() {
    QString typeName;
    int type = controller->getType();
    
    if (type == 1 || type == 2) typeName = "ArraySequence";
    else if (type == 3 || type == 4) typeName = "ListSequence";
    else if (type == 5 || type == 6) typeName = "BitSequence";
    else typeName = "Unknown";
    
    infoLabel->setText(QString("%1 | %2 | Размер: %3")
        .arg(typeName)
        .arg(controller->isMutable() ? "Изменяемая" : "Неизменяемая")
        .arg(controller->getSize()));
    
    bool isBit = (type == 5 || type == 6);
    secondSeqInput->setVisible(isBit);
    andBtn->setVisible(isBit);
    orBtn->setVisible(isBit);
    xorBtn->setVisible(isBit);
    notBtn->setVisible(isBit);
    
    listWidget->clear();
    
    if (isBit) {
        IEnumerator<Bit>* it = controller->getBitEnumerator();
        if (it) {
            while (it->MoveNext()) {
                listWidget->addItem(QString::number(it->Current() ? 1 : 0));
            }
            delete it;
        }
    } else {
        IEnumerator<int>* it = controller->getEnumerator();
        if (it) {
            while (it->MoveNext()) {
                listWidget->addItem(QString::number(it->Current()));
            }
            delete it;
        }
    }
    
    clearSelection();
}

void SequenceWidget::clearSelection() {
    for (int i = 0; i < listWidget->count(); ++i) {
        listWidget->item(i)->setBackground(Qt::transparent);
        listWidget->item(i)->setForeground(Qt::black);
    }
    selectedStart = -1;
    selectedEnd = -1;
    hasStart = false;
}

void SequenceWidget::onItemClicked(QListWidgetItem* item) {
    int index = listWidget->row(item);
    if (index < 0) return;
    
    if (!hasStart) {
        selectedStart = index;
        hasStart = true;
        item->setBackground(Qt::blue);
        item->setForeground(Qt::white);
    } else {
        selectedEnd = index;
        hasStart = false;
        
        int s = std::min(selectedStart, selectedEnd);
        int e = std::max(selectedStart, selectedEnd);
        for (int i = s; i <= e; ++i) {
            listWidget->item(i)->setBackground(Qt::green);
            listWidget->item(i)->setForeground(Qt::black);
        }
    }
}

void SequenceWidget::onSubsequence() {
    if (selectedStart < 0 || selectedEnd < 0) {
        onError("Сначала выберите начальный и конечный элемент");
        return;
    }
    
    int s = std::min(selectedStart, selectedEnd);
    int e = std::max(selectedStart, selectedEnd);
    
    if (isBitSequence()) {
        controller->getSubsequenceBit(s, e);
    } else {
        controller->getSubsequence(s, e);
    }
    
    clearSelection();
}

void SequenceWidget::onMap() {
    if (isBitSequence()) {
        controller->mapBit();
    } else {
        controller->map();
    }
}

void SequenceWidget::onWhere() {
    if (isBitSequence()) {
        controller->whereBit();
    } else {
        controller->where();
    }
}

void SequenceWidget::onReduce() {
    if (isBitSequence()) {
        controller->reduceBit();
    } else {
        controller->reduce();
    }
}

void SequenceWidget::onConcat() {
    bool ok;
    
    if (isBitSequence()) {
        QString bits = QInputDialog::getText(this, "Concat", 
                    "Введите битовую последовательность (например, 10110):",
                    QLineEdit::Normal, "", &ok);
        if (ok && !bits.isEmpty()) {
            controller->concatBit(bits);
        }
    } else {
        QString numbers = QInputDialog::getText(this, "Concat", 
                    "Введите числа, разделенные пробелом (например, 1 2 3):",
                    QLineEdit::Normal, "", &ok);
        if (ok && !numbers.isEmpty()) {
            controller->concat(numbers);
        }
    }
}

void SequenceWidget::onAnd() {
    if (!secondSeqInput->text().isEmpty()) {
        controller->andBit(secondSeqInput->text());
        secondSeqInput->clear();
    }
}

void SequenceWidget::onOr() {
    if (!secondSeqInput->text().isEmpty()) {
        controller->orBit(secondSeqInput->text());
        secondSeqInput->clear();
    }
}

void SequenceWidget::onXor() {
    if (!secondSeqInput->text().isEmpty()) {
        controller->xorBit(secondSeqInput->text());
        secondSeqInput->clear();
    }
}

void SequenceWidget::onNot() {
    controller->notBit();
}

void SequenceWidget::showContextMenu(const QPoint& pos) {
    QListWidgetItem* item = listWidget->itemAt(pos);
    if (!item) return;
    
    int row = listWidget->row(item);
    QString currentValue = item->text();
    
    QMenu menu;
    QAction* insertBefore = menu.addAction("Вставить до");
    QAction* insertAfter = menu.addAction("Вставить после");
    QAction* edit = menu.addAction("Редактировать");
    
    QAction* selected = menu.exec(listWidget->viewport()->mapToGlobal(pos));
    
    if (selected == insertBefore) {
        bool ok;
        QString val = QInputDialog::getText(this, "Вставить", "Значение:", QLineEdit::Normal, "", &ok);
        if (ok && !val.isEmpty()) {
            controller->insertAt(row, val);
        }
    } else if (selected == insertAfter) {
        bool ok;
        QString val = QInputDialog::getText(this, "Вставить", "Значение:", QLineEdit::Normal, "", &ok);
        if (ok && !val.isEmpty()) {
            controller->insertAt(row + 1, val);
        }
    } else if (selected == edit) {
        bool ok;
        QString newVal = QInputDialog::getText(this, "Редактировать", "Новое значение:", 
                    QLineEdit::Normal, currentValue, &ok);
        if (ok && !newVal.isEmpty() && newVal != currentValue) {
            controller->set(row, newVal);
        }
    }
}

void SequenceWidget::onItemDoubleClicked(QListWidgetItem* item) {
    int row = listWidget->row(item);
    QString currentValue = item->text();
    bool ok;
    QString newVal = QInputDialog::getText(this, "Редактировать", "Новое значение:", 
                QLineEdit::Normal, currentValue, &ok);
    if (ok && !newVal.isEmpty() && newVal != currentValue) {
        controller->set(row, newVal);
    }
}

void SequenceWidget::onAppend() {
    if (!valueInput->text().isEmpty()) {
        controller->append(valueInput->text());
        valueInput->clear();
    }
}

void SequenceWidget::onPrepend() {
    if (!valueInput->text().isEmpty()) {
        controller->prepend(valueInput->text());
        valueInput->clear();
    }
}

void SequenceWidget::onError(const QString& msg) {
    QMessageBox::information(this, "Результат", msg);
}

void SequenceWidget::refresh() {
    updateDisplay();
}