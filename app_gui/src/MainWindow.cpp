#include "MainWindow.hpp"
#include "SequenceController.hpp"
#include "SequenceWidget.hpp"
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    
    setWindowTitle("Sequence System - Lab 2");
    setMinimumSize(900, 700);
    
    QHBoxLayout* createLayout = new QHBoxLayout();
    typeCombo = new QComboBox();
    typeCombo->addItem("ArraySequence");
    typeCombo->addItem("ListSequence");
    typeCombo->addItem("BitSequence");
    
    mutabilityCombo = new QComboBox();
    mutabilityCombo->addItem("Mutable");
    mutabilityCombo->addItem("Immutable");
    
    createBtn = new QPushButton("Create New Sequence");
    createBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    
    createLayout->addWidget(new QLabel("Type:"));
    createLayout->addWidget(typeCombo);
    createLayout->addWidget(new QLabel("Mutability:"));
    createLayout->addWidget(mutabilityCombo);
    createLayout->addWidget(createBtn);
    createLayout->addStretch();
    mainLayout->addLayout(createLayout);
    
    tabWidget = new QTabWidget();
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);
    mainLayout->addWidget(tabWidget);
    
    connect(createBtn, &QPushButton::clicked, this, &MainWindow::onCreate);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabClose);
}
    
void MainWindow::onCreate() {
    bool isMutable = (mutabilityCombo->currentIndex() == 0);
    int typeIdx = typeCombo->currentIndex();
    
    QString initData;
    if (!isMutable) {
        bool ok;
        if (typeIdx == 2) {
            initData = QInputDialog::getText(this, "Инициализация", 
                "Введите начальные биты (только 0 и 1, без пробелов):\n"
                "Например: 10110",
                QLineEdit::Normal, "", &ok);
        } else {
            initData = QInputDialog::getText(this, "Инициализация", 
                "Введите начальные числа, разделенные пробелом:\n"
                "Например: 10 20 30",
                QLineEdit::Normal, "", &ok);
        }
        if (!ok) return;
        
        if (typeIdx == 2) {
            for (QChar ch : initData) {
                if (ch != '0' && ch != '1') {
                    QMessageBox::warning(this, "Ошибка", 
                        "BitSequence может содержать только 0 и 1!");
                    return;
                }
            }
        }
    }
    
    SequenceController* controller = new SequenceController();
    
    if (typeIdx == 0) {
        controller->createArray(isMutable);
    } else if (typeIdx == 1) {
        controller->createList(isMutable);
    } else {
        controller->createBit(isMutable);
    }
    
    if (!isMutable && !initData.isEmpty()) {
        controller->initializeFromString(initData);
    }
    
    SequenceWidget* widget = new SequenceWidget(controller);
    QString tabName = QString("%1_%2")
        .arg(typeCombo->currentText())
        .arg(isMutable ? "Mutable" : "Immutable");
    
    tabWidget->addTab(widget, tabName);
    tabWidget->setCurrentWidget(widget);
    
    connect(widget, &SequenceWidget::newSequenceCreated, this, &MainWindow::addNewTab);
}

void MainWindow::onTabClose(int index) {
    QWidget* widget = tabWidget->widget(index);
    tabWidget->removeTab(index);
    delete widget;
}

void MainWindow::addNewTab(SequenceController* controller, const QString& name) {
    SequenceWidget* widget = new SequenceWidget(controller);
    tabWidget->addTab(widget, name);
    tabWidget->setCurrentWidget(widget);
    connect(widget, &SequenceWidget::newSequenceCreated, this, &MainWindow::addNewTab);
}