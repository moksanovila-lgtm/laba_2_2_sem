#include "MainWindow.hpp"
#include <QMessageBox>
#include <QScrollBar>
#include <sstream>

// ==================== ÊÎÍÑÒÐÓÊÒÎÐ / ÄÅÑÒÐÓÊÒÎÐ ====================

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , currentSeq(nullptr)
    , currentBitSeq(nullptr)
    , currentType(-1)
{
    setupUI();
    updateStatus();
}

MainWindow::~MainWindow()
{
    delete currentSeq;
    delete currentBitSeq;
}

// ==================== ÂÑÏÎÌÎÃÀÒÅËÜÍÛÅ ÌÅÒÎÄÛ ====================

void MainWindow::appendOutput(const QString& text)
{
    outputArea->append(text);
    outputArea->verticalScrollBar()->setValue(
        outputArea->verticalScrollBar()->maximum()
    );
}

void MainWindow::updateStatus()
{
    QString status;
    if (currentType == 2 && currentBitSeq) {
        status = QString("BitSequence | Size: %1").arg(currentBitSeq->GetCount());
    } else if (currentSeq) {
        QString typeName = (currentType == 0) ? "ArraySequence" : "ListSequence";
        QString mutability = currentSeq->IsMutable() ? "Mutable" : "Immutable";
        status = QString("%1 | %2 | Size: %3")
            .arg(typeName).arg(mutability).arg(currentSeq->GetCount());
    } else {
        status = "No sequence created";
    }
    statusLabel->setText("?? " + status);
}

template<typename T>
QString MainWindow::sequenceToString(Sequence<T>* seq)
{
    if (!seq || seq->GetCount() == 0) return "[empty]";
    
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < seq->GetCount(); ++i) {
        ss << seq->Get(i);
        if (i < seq->GetCount() - 1) ss << ", ";
    }
    ss << "]";
    return QString::fromStdString(ss.str());
}

// ==================== ÍÀÑÒÐÎÉÊÀ UI ====================

void MainWindow::setupUI()
{
    setWindowTitle("Sequence System");
    setMinimumSize(900, 700);
    
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    
    // Ñòàòóñ áàð
    statusLabel = new QLabel("No sequence created");
    statusLabel->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 8px; font-weight: bold; }");
    mainLayout->addWidget(statusLabel);
    
    // Tab âèäæåò
    tabWidget = new QTabWidget();
    createTabCreation();
    createTabOperations();
    createTabMapReduce();
    createTabBit();
    createTabOutput();
    mainLayout->addWidget(tabWidget);
    
    // Ïîäêëþ÷åíèå ñèãíàëîâ
    connect(createBtn, &QPushButton::clicked, this, &MainWindow::onCreate);
    connect(showBtn, &QPushButton::clicked, this, &MainWindow::onShow);
    connect(appendBtn, &QPushButton::clicked, this, &MainWindow::onAppend);
    connect(prependBtn, &QPushButton::clicked, this, &MainWindow::onPrepend);
    connect(insertBtn, &QPushButton::clicked, this, &MainWindow::onInsert);
    connect(getBtn, &QPushButton::clicked, this, &MainWindow::onGet);
    connect(setBtn, &QPushButton::clicked, this, &MainWindow::onSet);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClear);
    connect(subsequenceBtn, &QPushButton::clicked, this, &MainWindow::onGetSubsequence);
    connect(concatBtn, &QPushButton::clicked, this, &MainWindow::onConcat);
    connect(mapBtn, &QPushButton::clicked, this, &MainWindow::onMap);
    connect(whereBtn, &QPushButton::clicked, this, &MainWindow::onWhere);
    connect(reduceBtn, &QPushButton::clicked, this, &MainWindow::onReduce);
    connect(iteratorBtn, &QPushButton::clicked, this, &MainWindow::onIterator);
    connect(andBtn, &QPushButton::clicked, this, &MainWindow::onBitAnd);
    connect(orBtn, &QPushButton::clicked, this, &MainWindow::onBitOr);
    connect(xorBtn, &QPushButton::clicked, this, &MainWindow::onBitXor);
    connect(notBtn, &QPushButton::clicked, this, &MainWindow::onBitNot);
}

void MainWindow::createTabCreation()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Òèï ïîñëåäîâàòåëüíîñòè
    QGroupBox* typeGroup = new QGroupBox("Sequence Type");
    QVBoxLayout* typeLayout = new QVBoxLayout(typeGroup);
    typeCombo = new QComboBox();
    typeCombo->addItem("ArraySequence (DynamicArray)");
    typeCombo->addItem("ListSequence (LinkedList)");
    typeCombo->addItem("BitSequence (packed bits)");
    typeLayout->addWidget(typeCombo);
    
    // Mutability
    QGroupBox* mutGroup = new QGroupBox("Mutability");
    QVBoxLayout* mutLayout = new QVBoxLayout(mutGroup);
    mutabilityCombo = new QComboBox();
    mutabilityCombo->addItem("Mutable (can be modified)");
    mutabilityCombo->addItem("Immutable (read-only)");
    mutLayout->addWidget(mutabilityCombo);
    
    // Êíîïêà ñîçäàíèÿ
    createBtn = new QPushButton("Create Sequence");
    createBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    
    layout->addWidget(typeGroup);
    layout->addWidget(mutGroup);
    layout->addWidget(createBtn);
    layout->addStretch();
    
    tabWidget->addTab(tab, "Create");
}

void MainWindow::createTabOperations()
{
    QWidget* tab = new QWidget();
    QGridLayout* layout = new QGridLayout(tab);
    
    int row = 0;
    
    showBtn = new QPushButton("Show Sequence");
    layout->addWidget(showBtn, row++, 0, 1, 2);
    
    // Value input
    QLabel* valueLabel = new QLabel("Value:");
    valueInput = new QLineEdit();
    valueInput->setPlaceholderText("Enter value...");
    layout->addWidget(valueLabel, row, 0);
    layout->addWidget(valueInput, row++, 1);
    
    appendBtn = new QPushButton("Append (add to end)");
    prependBtn = new QPushButton("Prepend (add to beginning)");
    layout->addWidget(appendBtn, row++, 0, 1, 2);
    layout->addWidget(prependBtn, row++, 0, 1, 2);
    
    // Index input
    QLabel* indexLabel = new QLabel("Index:");
    indexInput = new QLineEdit();
    indexInput->setPlaceholderText("Enter index...");
    layout->addWidget(indexLabel, row, 0);
    layout->addWidget(indexInput, row++, 1);
    
    insertBtn = new QPushButton("Insert At Index");
    getBtn = new QPushButton("Get Element");
    setBtn = new QPushButton("Set Element");
    clearBtn = new QPushButton("Clear Sequence");
    
    layout->addWidget(insertBtn, row++, 0, 1, 2);
    layout->addWidget(getBtn, row++, 0, 1, 2);
    layout->addWidget(setBtn, row++, 0, 1, 2);
    
    clearBtn->setStyleSheet("QPushButton { background-color: #f44336; color: white; }");
    layout->addWidget(clearBtn, row++, 0, 1, 2);
    
    // Subsequence
    QLabel* startLabel = new QLabel("Start:");
    QLabel* endLabel = new QLabel("End:");
    startInput = new QLineEdit();
    endInput = new QLineEdit();
    subsequenceBtn = new QPushButton("Get Subsequence");
    concatBtn = new QPushButton("Concat with Test Sequence");
    
    layout->addWidget(startLabel, row, 0);
    layout->addWidget(startInput, row++, 1);
    layout->addWidget(endLabel, row, 0);
    layout->addWidget(endInput, row++, 1);
    layout->addWidget(subsequenceBtn, row++, 0, 1, 2);
    layout->addWidget(concatBtn, row++, 0, 1, 2);
    
    layout->setRowStretch(row, 1);
    
    tabWidget->addTab(tab, "Operations");
}

void MainWindow::createTabMapReduce()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    mapBtn = new QPushButton("Map (x ? x * 2)");
    whereBtn = new QPushButton("Where (filter even numbers)");
    reduceBtn = new QPushButton("Reduce (sum of all elements)");
    iteratorBtn = new QPushButton("Test Iterator");
    
    layout->addWidget(mapBtn);
    layout->addWidget(whereBtn);
    layout->addWidget(reduceBtn);
    layout->addWidget(iteratorBtn);
    layout->addStretch();
    
    tabWidget->addTab(tab, "Map-Reduce");
}

void MainWindow::createTabBit()
{
    QWidget* tab = new QWidget();
    QGridLayout* layout = new QGridLayout(tab);
    
    int row = 0;
    
    QLabel* bitLabel = new QLabel("Bit value (0/1):");
    bitInput = new QLineEdit();
    bitInput->setPlaceholderText("Enter 0 or 1...");
    layout->addWidget(bitLabel, row, 0);
    layout->addWidget(bitInput, row++, 1);
    
    andBtn = new QPushButton("AND with test sequence (11010)");
    orBtn = new QPushButton("OR with test sequence (11010)");
    xorBtn = new QPushButton("XOR with test sequence (11010)");
    notBtn = new QPushButton("NOT (bitwise complement)");
    
    layout->addWidget(andBtn, row++, 0, 1, 2);
    layout->addWidget(orBtn, row++, 0, 1, 2);
    layout->addWidget(xorBtn, row++, 0, 1, 2);
    layout->addWidget(notBtn, row++, 0, 1, 2);
    
    layout->setRowStretch(row, 1);
    
    tabWidget->addTab(tab, "Bit Operations");
}

void MainWindow::createTabOutput()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    outputArea = new QTextEdit();
    outputArea->setReadOnly(true);
    outputArea->setFont(QFont("Courier New", 10));
    layout->addWidget(outputArea);
    
    tabWidget->addTab(tab, "Output");
}

// ==================== ÑËÎÒÛ ====================

void MainWindow::onCreate()
{
    int type = typeCombo->currentIndex();
    bool isMutable = (mutabilityCombo->currentIndex() == 0);
    
    delete currentSeq;
    delete currentBitSeq;
    currentSeq = nullptr;
    currentBitSeq = nullptr;
    
    try {
        if (type == 0) { // ArraySequence
            currentSeq = isMutable ? new ArraySequence<int>() : new ImmutableArraySequence<int>();
            currentType = 0;
            appendOutput(QString("? Created %1 ArraySequence")
                .arg(isMutable ? "Mutable" : "Immutable"));
        } else if (type == 1) { // ListSequence
            currentSeq = isMutable ? new ListSequence<int>() : new ImmutableListSequence<int>();
            currentType = 1;
            appendOutput(QString("? Created %1 ListSequence")
                .arg(isMutable ? "Mutable" : "Immutable"));
        } else if (type == 2) { // BitSequence
            currentBitSeq = new BitSequence(isMutable);
            currentType = 2;
            appendOutput(QString("? Created %1 BitSequence")
                .arg(isMutable ? "Mutable" : "Immutable"));
        }
        updateStatus();
    } catch (const std::exception& e) {
        appendOutput(QString("? Error: %1").arg(e.what()));
    }
}

void MainWindow::onShow()
{
    if (currentType == 2 && currentBitSeq) {
        QString bits;
        for (size_t i = 0; i < currentBitSeq->GetCount(); ++i) {
            bits += QString::number(currentBitSeq->Get(i).toInt());
        }
        appendOutput(QString("?? BitSequence: %1 (size: %2)")
            .arg(bits).arg(currentBitSeq->GetCount()));
    } else if (currentSeq) {
        appendOutput(QString("?? Sequence: %1 (size: %2)")
            .arg(sequenceToString(currentSeq))
            .arg(currentSeq->GetCount()));
    } else {
        appendOutput("? No sequence created");
    }
}

void MainWindow::onAppend()
{
    if (currentType == 2 && currentBitSeq) {
        bool ok;
        int val = bitInput->text().toInt(&ok);
        if (!ok || (val != 0 && val != 1)) {
            appendOutput("? Invalid bit value (must be 0 or 1)");
            return;
        }
        try {
            currentBitSeq->Append(Bit(val));
            appendOutput(QString("? Appended bit %1").arg(val));
            bitInput->clear();
            updateStatus();
        } catch (const std::exception& e) {
            appendOutput(QString("? Error: %1").arg(e.what()));
        }
    } else if (currentSeq) {
        bool ok;
        int val = valueInput->text().toInt(&ok);
        if (!ok) {
            appendOutput("? Invalid value");
            return;
        }
        try {
            currentSeq->Append(val);
            appendOutput(QString("? Appended %1").arg(val));
            valueInput->clear();
            updateStatus();
        } catch (const std::exception& e) {
            appendOutput(QString("? Error: %1").arg(e.what()));
        }
    } else {
        appendOutput("? No sequence created");
    }
}

void MainWindow::onGet()
{
    if (currentType == 2 && currentBitSeq) {
        bool ok;
        size_t idx = indexInput->text().toULongLong(&ok);
        if (!ok) {
            appendOutput("? Invalid index");
            return;
        }
        try {
            Bit val = currentBitSeq->Get(idx);
            appendOutput(QString("?? Element at index %1: %2").arg(idx).arg(val.toInt()));
        } catch (const std::exception& e) {
            appendOutput(QString("? Error: %1").arg(e.what()));
        }
    } else if (currentSeq) {
        bool ok;
        size_t idx = indexInput->text().toULongLong(&ok);
        if (!ok) {
            appendOutput("? Invalid index");
            return;
        }
        try {
            int val = currentSeq->Get(idx);
            appendOutput(QString("?? Element at index %1: %2").arg(idx).arg(val));
        } catch (const std::exception& e) {
            appendOutput(QString("? Error: %1").arg(e.what()));
        }
    } else {
        appendOutput("? No sequence created");
    }
}

void MainWindow::onMap()
{
    if (!currentSeq) {
        appendOutput("? No sequence created");
        return;
    }
    
    try {
        auto doubleFunc = [](const int& x) { return x * 2; };
        Sequence<int>* result = currentSeq->Map(doubleFunc);
        appendOutput(QString("?? Map (x ? x*2): %1").arg(sequenceToString(result)));
        delete result;
    } catch (const std::exception& e) {
        appendOutput(QString("? Error: %1").arg(e.what()));
    }
}

void MainWindow::onWhere()
{
    if (!currentSeq) {
        appendOutput("? No sequence created");
        return;
    }
    
    try {
        auto isEven = [](const int& x) { return x % 2 == 0; };
        Sequence<int>* result = currentSeq->Where(isEven);
        appendOutput(QString("?? Where (even numbers): %1").arg(sequenceToString(result)));
        delete result;
    } catch (const std::exception& e) {
        appendOutput(QString("? Error: %1").arg(e.what()));
    }
}

void MainWindow::onReduce()
{
    if (!currentSeq) {
        appendOutput("? No sequence created");
        return;
    }
    
    try {
        auto sum = [](const int& a, const int& b) { return a + b; };
        int result = currentSeq->Reduce(sum, 0);
        appendOutput(QString("?? Reduce (sum): %1").arg(result));
    } catch (const std::exception& e) {
        appendOutput(QString("? Error: %1").arg(e.what()));
    }
}

void MainWindow::onIterator()
{
    if (currentType == 2 && currentBitSeq) {
        try {
            IEnumerator<Bit>* it = currentBitSeq->GetEnumerator();
            QString elements;
            while (it->MoveNext()) {
                elements += QString::number(it->Current().toInt()) + " ";
            }
            appendOutput(QString("?? Iterator: [%1]").arg(elements.trimmed()));
            delete it;
        } catch (const std::exception& e) {
            appendOutput(QString("? Error: %1").arg(e.what()));
        }
    } else if (currentSeq) {
        try {
            IEnumerator<int>* it = currentSeq->GetEnumerator();
            QString elements;
            while (it->MoveNext()) {
                elements += QString::number(it->Current()) + " ";
            }
            appendOutput(QString("?? Iterator: [%1]").arg(elements.trimmed()));
            delete it;
        } catch (const std::exception& e) {
            appendOutput(QString("? Error: %1").arg(e.what()));
        }
    } else {
        appendOutput("? No sequence created");
    }
}

void MainWindow::onBitAnd()
{
    if (!currentBitSeq) {
        appendOutput("? No BitSequence created");
        return;
    }
    
    try {
        Bit bits[] = {Bit(1), Bit(1), Bit(0), Bit(1), Bit(0)};
        BitSequence other(bits, 5, false);
        BitSequence* result = currentBitSeq->And(other);
        
        QString resultBits;
        for (size_t i = 0; i < result->GetCount(); ++i) {
            resultBits += QString::number(result->Get(i).toInt());
        }
        appendOutput(QString("?? AND with 11010: %1").arg(resultBits));
        delete result;
    } catch (const std::exception& e) {
        appendOutput(QString("? Error: %1").arg(e.what()));
    }
}

void MainWindow::onBitOr()
{
    if (!currentBitSeq) {
        appendOutput("? No BitSequence created");
        return;
    }
    
    try {
        Bit bits[] = {Bit(1), Bit(1), Bit(0), Bit(1), Bit(0)};
        BitSequence other(bits, 5, false);
        BitSequence* result = currentBitSeq->Or(other);
        
        QString resultBits;
        for (size_t i = 0; i < result->GetCount(); ++i) {
            resultBits += QString::number(result->Get(i).toInt());
        }
        appendOutput(QString("?? OR with 11010: %1").arg(resultBits));
        delete result;
    } catch (const std::exception& e) {
        appendOutput(QString("? Error: %1").arg(e.what()));
    }
}

void MainWindow::onBitXor()
{
    if (!currentBitSeq) {
        appendOutput("? No BitSequence created");
        return;
    }
    
    try {
        Bit bits[] = {Bit(1), Bit(1), Bit(0), Bit(1), Bit(0)};
        BitSequence other(bits, 5, false);
        BitSequence* result = currentBitSeq->Xor(other);
        
        QString resultBits;
        for (size_t i = 0; i < result->GetCount(); ++i) {
            resultBits += QString::number(result->Get(i).toInt());
        }
        appendOutput(QString("?? XOR with 11010: %1").arg(resultBits));
        delete result;
    } catch (const std::exception& e) {
        appendOutput(QString("? Error: %1").arg(e.what()));
    }
}

void MainWindow::onBitNot()
{
    if (!currentBitSeq) {
        appendOutput("? No BitSequence created");
        return;
    }
    
    try {
        BitSequence* result = currentBitSeq->Not();
        QString resultBits;
        for (size_t i = 0; i < result->GetCount(); ++i) {
            resultBits += QString::number(result->Get(i).toInt());
        }
        appendOutput(QString("?? NOT: %1").arg(resultBits));
        delete result;
    } catch (const std::exception& e) {
        appendOutput(QString("? Error: %1").arg(e.what()));
    }
}

void MainWindow::onSet() {}
void MainWindow::onPrepend() {}
void MainWindow::onInsert() {}
void MainWindow::onClear() {}
void MainWindow::onGetSubsequence() {}
void MainWindow::onConcat() {}
void MainWindow::onBitSet() {}
void MainWindow::onBitGet() {}