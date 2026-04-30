#include "MainWindow.hpp"
#include <QMessageBox>
#include <QScrollBar>
#include <sstream>


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
        status = QString("%1 | %2 | Размер: %3")
            .arg(typeName).arg(mutability).arg(currentSeq->GetCount());
    } else {
        status = " Последовательность не создана";
    }
    statusLabel->setText(status);
}

template<typename T>
QString MainWindow::sequenceToString(Sequence<T>* seq)
{
    if (!seq || seq->GetCount() == 0) return "[пусто]";
    
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < seq->GetCount(); ++i) {
        ss << seq->Get(i);
        if (i < seq->GetCount() - 1) ss << ", ";
    }
    ss << "]";
    return QString::fromStdString(ss.str());
}


void MainWindow::setupUI()
{
    setWindowTitle("Sequence System");
    setMinimumSize(900, 700);
    
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    
    statusLabel = new QLabel("No sequence created");
    statusLabel->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 8px; font-weight: bold; }");
    mainLayout->addWidget(statusLabel);
    
    tabWidget = new QTabWidget();
    createTabCreation();
    createTabOperations();
    createTabMapReduce();
    createTabBit();
    createTabOutput();
    mainLayout->addWidget(tabWidget);
    
    connect(createBtn, &QPushButton::clicked, this, &MainWindow::onCreate);
    connect(showBtn, &QPushButton::clicked, this, &MainWindow::onShow);
    connect(appendBtn, &QPushButton::clicked, this, &MainWindow::onAppend);
    connect(prependBtn, &QPushButton::clicked, this, &MainWindow::onPrepend);
    connect(insertBtn, &QPushButton::clicked, this, &MainWindow::onInsert);
    connect(getBtn, &QPushButton::clicked, this, &MainWindow::onGet);
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
    connect(getFirstBtn, &QPushButton::clicked, this, &MainWindow::onGetFirst);
    connect(getLastBtn, &QPushButton::clicked, this, &MainWindow::onGetLast);
}

void MainWindow::createTabCreation()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    QGroupBox* typeGroup = new QGroupBox("Sequence Type");
    QVBoxLayout* typeLayout = new QVBoxLayout(typeGroup);
    typeCombo = new QComboBox();
    typeCombo->addItem("ArraySequence (DynamicArray)");
    typeCombo->addItem("ListSequence (LinkedList)");
    typeCombo->addItem("BitSequence (packed bits)");
    typeLayout->addWidget(typeCombo);
    
    QGroupBox* mutGroup = new QGroupBox("Mutability");
    QVBoxLayout* mutLayout = new QVBoxLayout(mutGroup);
    mutabilityCombo = new QComboBox();
    mutabilityCombo->addItem("Mutable (can be modified)");
    mutabilityCombo->addItem("Immutable (read-only)");
    mutLayout->addWidget(mutabilityCombo);
    
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
    


    getFirstBtn = new QPushButton("Get First Element");
    getLastBtn = new QPushButton("Get Last Element");
    layout->addWidget(getFirstBtn, row++, 0, 1, 2);
    layout->addWidget(getLastBtn, row++, 0, 1, 2);



    QLabel* valueLabel = new QLabel("Value:");
    valueInput = new QLineEdit();
    valueInput->setPlaceholderText("Enter value...");
    layout->addWidget(valueLabel, row, 0);
    layout->addWidget(valueInput, row++, 1);
    
    appendBtn = new QPushButton("Append (add to end)");
    prependBtn = new QPushButton("Prepend (add to beginning)");
    layout->addWidget(appendBtn, row++, 0, 1, 2);
    layout->addWidget(prependBtn, row++, 0, 1, 2);
    
    QLabel* indexLabel = new QLabel("Index:");
    indexInput = new QLineEdit();
    indexInput->setPlaceholderText("Enter index...");
    layout->addWidget(indexLabel, row, 0);
    layout->addWidget(indexInput, row++, 1);
    
    insertBtn = new QPushButton("Insert At Index");
    getBtn = new QPushButton("Get Element");
    clearBtn = new QPushButton("Clear Sequence");
    
    layout->addWidget(insertBtn, row++, 0, 1, 2);
    layout->addWidget(getBtn, row++, 0, 1, 2);
    clearBtn->setStyleSheet("QPushButton { background-color: #f44336; color: white; }");
    layout->addWidget(clearBtn, row++, 0, 1, 2);
    
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
    
    andBtn = new QPushButton("AND with test sequence (11010)");
    orBtn = new QPushButton("OR with test sequence (11010)");
    xorBtn = new QPushButton("XOR with test sequence (11010)");
    notBtn = new QPushButton("NOT ");
    
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


void MainWindow::onCreate()
{
    int type = typeCombo->currentIndex();
    bool isMutable = (mutabilityCombo->currentIndex() == 0);
    
    delete currentSeq;
    delete currentBitSeq;
    currentSeq = nullptr;
    currentBitSeq = nullptr;
    
    try {
        if (type == 0) { 
            currentSeq = isMutable ? new ArraySequence<int>() : new ImmutableArraySequence<int>();
            currentType = 0;
            appendOutput(QString(" Создана %1 ArraySequence")
                .arg(isMutable ? "Mutable" : "Immutable"));
        } else if (type == 1) { 
            currentSeq = isMutable ? new ListSequence<int>() : new ImmutableListSequence<int>();
            currentType = 1;
            appendOutput(QString(" Создана %1 ListSequence")
                .arg(isMutable ? "Mutable" : "Immutable"));
        } else if (type == 2) { 
            currentBitSeq = new BitSequence(isMutable);
            currentType = 2;
            appendOutput(QString(" Создана %1 BitSequence")
                .arg(isMutable ? "Mutable" : "Immutable"));
        }
        updateStatus();
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::onShow()
{
    if (currentType == 2 && currentBitSeq) {
        QString bits;
        for (size_t i = 0; i < currentBitSeq->GetCount(); ++i) {
            bits += QString::number(currentBitSeq->Get(i) ? 1 : 0); 
        }
        appendOutput(QString(" BitSequence: %1 (size: %2)")
            .arg(bits).arg(currentBitSeq->GetCount()));
    } else if (currentSeq) {
        appendOutput(QString(" Последовательность: %1 (Размер: %2)")
            .arg(sequenceToString(currentSeq))
            .arg(currentSeq->GetCount()));
    } else {
        appendOutput(" Последовательность не создана");
    }
}

void MainWindow::onAppend()
{
    if (currentType == 2 && currentBitSeq) {
        QString inputText = valueInput->text();  
        bool ok;
        int val = inputText.toInt(&ok);
        
        valueInput->clear();
        
        if (!ok || (val != 0 && val != 1)) {
            appendOutput(QString(" Недопустимое битовое значение: '%1' (must be 0 or 1)").arg(inputText));
            return;
        }
        
        try {
            currentBitSeq->Append(val == 1);
            appendOutput(QString(" Appended bit %1").arg(val));
            updateStatus();
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else if (currentSeq) {
        QString inputText = valueInput->text();
        bool ok;
        int val = inputText.toInt(&ok);
        
        valueInput->clear();  
        
        if (!ok) {
            appendOutput(QString(" Недопустимое значение: '%1'").arg(inputText));
            return;
        }
        
        try {
            currentSeq->Append(val);
            appendOutput(QString(" Appended %1").arg(val));
            updateStatus();
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else {
        appendOutput(" Последовательность не создана");
    }
}

void MainWindow::onGet()
{
    if (currentType == 2 && currentBitSeq) {
        bool ok;
        size_t idx = indexInput->text().toULongLong(&ok);
        if (!ok) {
            appendOutput(" Недопустимый индекс");
            return;
        }
        try {
            Bit val = currentBitSeq->Get(idx);
            appendOutput(QString(" Элемент с индексом %1: %2").arg(idx).arg(val ? 1 : 0));  
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else if (currentSeq) {
        bool ok;
        size_t idx = indexInput->text().toULongLong(&ok);
        if (!ok) {
            appendOutput(" Недопустимый индекс");
            return;
        }
        try {
            int val = currentSeq->Get(idx);
            appendOutput(QString(" Элемент с индексом %1: %2").arg(idx).arg(val));
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else {
        appendOutput(" Последовательность не создана");
    }
}

void MainWindow::onMap()
{
    if (!currentSeq) {
        appendOutput(" Последовательность не создана");
        return;
    }
    
    try {
        auto doubleFunc = [](const int& x) { return x * 2; };
        Sequence<int>* result = currentSeq->Map(doubleFunc);
        appendOutput(QString(" Map (x ? x*2): %1").arg(sequenceToString(result)));
        delete result;
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::onWhere()
{
    if (!currentSeq) {
        appendOutput(" Последовательность не создана");
        return;
    }
    
    try {
        auto isEven = [](const int& x) { return x % 2 == 0; };
        Sequence<int>* result = currentSeq->Where(isEven);
        appendOutput(QString(" Where (even numbers): %1").arg(sequenceToString(result)));
        delete result;
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::onReduce()
{
    if (!currentSeq) {
        appendOutput(" Последовательность не создана");
        return;
    }
    
    try {
        auto sum = [](const int& a, const int& b) { return a + b; };
        int result = currentSeq->Reduce(sum, 0);
        appendOutput(QString(" Reduce (sum): %1").arg(result));
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::onIterator()
{
    if (currentType == 2 && currentBitSeq) {
        try {
            IEnumerator<Bit>* it = currentBitSeq->GetEnumerator();
            QString elements;
            while (it->MoveNext()) {
                elements += QString::number(it->Current() ? 1 : 0) + " ";  
            }
            appendOutput(QString(" Iterator: [%1]").arg(elements.trimmed()));
            delete it;
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else if (currentSeq) {
        try {
            IEnumerator<int>* it = currentSeq->GetEnumerator();
            QString elements;
            while (it->MoveNext()) {
                elements += QString::number(it->Current()) + " ";
            }
            appendOutput(QString(" Iterator: [%1]").arg(elements.trimmed()));
            delete it;
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else {
        appendOutput(" Последовательность не создана");
    }
}

void MainWindow::onBitAnd()
{
    if (!currentBitSeq) {
        appendOutput(" Битовая последовательность не создана");
        return;
    }
    
    try {
        bool bits[] = {true, true, false, true, false};  
        BitSequence other(bits, 5, false);
        
        if (currentBitSeq->GetCount() != other.GetCount()) {
            appendOutput(QString(" Не удаётся выполнить AND: sizes differ (%1 vs %2)")
                .arg(currentBitSeq->GetCount()).arg(other.GetCount()));
            appendOutput("   Пожалуйста, создайте битовую последовательность ровно из 5 бит");
            return;
        }
        
        BitSequence* result = currentBitSeq->And(other);
        QString resultBits;
        for (size_t i = 0; i < result->GetCount(); ++i) {
            resultBits += QString::number(result->Get(i) ? 1 : 0);
        }
        appendOutput(QString(" AND with 11010: %1").arg(resultBits));
        delete result;
        
    } catch (const IncompatibleSizesException& e) {
        appendOutput(QString(" Несоответствие размеров: %1").arg(e.what()));
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    } catch (...) {
        appendOutput(" Неизвестная ошибка во время операции AND");
    }
}

void MainWindow::onBitOr()
{
    if (!currentBitSeq) {
        appendOutput(" Битовая последовательность не создана");
        return;
    }
    
    try {
        bool bits[] = {true, true, false, true, false};  
        BitSequence other(bits, 5, false);
        
        if (currentBitSeq->GetCount() != other.GetCount()) {
            appendOutput(QString(" Не удаётся выполнить OR: размеры различаются (%1 vs %2)")
                .arg(currentBitSeq->GetCount()).arg(other.GetCount()));
            return;
        }
        
        BitSequence* result = currentBitSeq->Or(other);
        QString resultBits;
        for (size_t i = 0; i < result->GetCount(); ++i) {
            resultBits += QString::number(result->Get(i) ? 1 : 0);
        }
        appendOutput(QString(" OR with 11010: %1").arg(resultBits));
        delete result;
        
    } catch (const IncompatibleSizesException& e) {
        appendOutput(QString(" Несоответствие размеров: %1").arg(e.what()));
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::onBitXor()
{
    if (!currentBitSeq) {
        appendOutput(" Битовая последовательность не создана");
        return;
    }
    
    try {
        bool bits[] = {true, true, false, true, false};  
        BitSequence other(bits, 5, false);
        
        if (currentBitSeq->GetCount() != other.GetCount()) {
            appendOutput(QString(" Не удается выполнить XOR: размеры различаются (%1 vs %2)")
                .arg(currentBitSeq->GetCount()).arg(other.GetCount()));
            return;
        }
        
        BitSequence* result = currentBitSeq->Xor(other);
        QString resultBits;
        for (size_t i = 0; i < result->GetCount(); ++i) {
            resultBits += QString::number(result->Get(i) ? 1 : 0);
        }
        appendOutput(QString(" XOR with 11010: %1").arg(resultBits));
        delete result;
        
    } catch (const IncompatibleSizesException& e) {
        appendOutput(QString(" Несоответствие размеров: %1").arg(e.what()));
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::onBitNot()
{
    if (!currentBitSeq) {
        appendOutput(" Битовая последовательность не создана");
        return;
    }
    
    try {
        BitSequence* result = currentBitSeq->Not();
        QString resultBits;
        for (size_t i = 0; i < result->GetCount(); ++i) {
            resultBits += QString::number(result->Get(i) ? 1 : 0);
        }
        appendOutput(QString(" NOT: %1").arg(resultBits));
        delete result;
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}


void MainWindow::onPrepend()
{
    if (currentType == 2 && currentBitSeq) {
        bool ok;
        int val = valueInput->text().toInt(&ok);
        valueInput->clear(); 
        
        if (!ok || (val != 0 && val != 1)) {
            appendOutput(" Недопустимое значение бита (должно быть 0 или 1)");
            return;
        }
        
        try {
            currentBitSeq->Prepend(val == 1);
            appendOutput(QString(" Prepended bit %1").arg(val));
            updateStatus();
            onShow();
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else if (currentSeq) {
        bool ok;
        int val = valueInput->text().toInt(&ok);
        valueInput->clear();  
        
        if (!ok) {
            appendOutput(" Недопустимое значение");
            return;
        }
        
        try {
            currentSeq->Prepend(val);
            appendOutput(QString(" Prepended %1").arg(val));
            updateStatus();
            onShow();
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else {
        appendOutput(" Последовательность не создана");
    }
}

void MainWindow::onInsert()
{
    if (currentType == 2 && currentBitSeq) {
        bool ok1, ok2;
        int index = indexInput->text().toInt(&ok1);
        int val = valueInput->text().toInt(&ok2);
        
        indexInput->clear();
        valueInput->clear();
        
        if (!ok1 || !ok2) {
            appendOutput(" Недопустимый индекс или значение");
            return;
        }
        
        if (val != 0 && val != 1) {
            appendOutput(QString(" Бит должен быть равен 0 или 1 (получено значение% 1)").arg(val));
            return;
        }
        
        try {
            currentBitSeq->InsertAt(val == 1, index);
            appendOutput(QString(" Вставлен бит %1 с индексом %2").arg(val).arg(index));
            updateStatus();
            onShow();
        } catch (const IndexOutOfRangeException& e) {
            appendOutput(QString("Индекс вне диапазона: %1").arg(e.what()));
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else if (currentSeq) {
    } else {
        appendOutput(" Последовательность не создана");
    }
}

void MainWindow::onClear()
{
    if (currentType == 2 && currentBitSeq) {
        try {
            currentBitSeq->Clear();
            appendOutput(" BitSequence очищена");
            updateStatus();
            onShow();
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else if (currentSeq) {
        try {
            currentSeq->Clear();
            appendOutput(" Последовательность очищенна");
            updateStatus();
            onShow();
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else {
        appendOutput(" Последовательность не создана");
    }
}

void MainWindow::onGetSubsequence()
{
    if (!currentSeq || currentType == 2) {
        appendOutput(" GetSubsequence работает только для последовательности (int), а не для BitSequence");
        return;
    }
    
    bool ok1, ok2;
    int start = startInput->text().toInt(&ok1);
    int end = endInput->text().toInt(&ok2);
    
    if (!ok1 || !ok2) {
        appendOutput(" Недопустимый начальный или конечный индекс");
        return;
    }
    
    try {
        Sequence<int>* sub = currentSeq->GetSubsequence(start, end);
        appendOutput(QString(" Subsequence [%1..%2]: %3")
            .arg(start).arg(end).arg(sequenceToString(sub)));
        delete sub;
        startInput->clear();
        endInput->clear();
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::onConcat()
{
    if (!currentSeq || currentType == 2) {
        appendOutput(" Объединение работает только для последовательности (int), а не для BitSequence");
        return;
    }
    
    try {
        ArraySequence<int> tempSeq;
        tempSeq.Append(100);
        tempSeq.Append(200);
        tempSeq.Append(300);
        
        Sequence<int>* result = currentSeq->Concat(&tempSeq);
        appendOutput(QString(" Concat with [100,200,300]: %1")
            .arg(sequenceToString(result)));
        delete result;
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::onBitSet()
{
    if (!currentBitSeq) {
        appendOutput(" Битовая последовательность не создана");
        return;
    }
    
    bool ok1, ok2;
    int index = indexInput->text().toInt(&ok1);
    int val = bitInput->text().toInt(&ok2);
    
    if (!ok1 || !ok2) {
        appendOutput(" Недопустимый индекс или значение");
        return;
    }
    
    if (val != 0 && val != 1) {
        appendOutput(" Бит должен быть равен 0 или 1");
        return;
    }
    
    try {
        currentBitSeq->Set(index, val == 1);
        appendOutput(QString(" Установите бит с индексом от % 1 до %2").arg(index).arg(val));
        indexInput->clear();
        bitInput->clear();
        updateStatus();
        onShow();
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::onBitGet()
{
    if (!currentBitSeq) {
        appendOutput(" Битовая последовательность не создана");
        return;
    }
    
    bool ok;
    int index = indexInput->text().toInt(&ok);
    
    if (!ok) {
        appendOutput(" Недопустимый индекс");
        return;
    }
    
    try {
        Bit val = currentBitSeq->Get(index);
        appendOutput(QString(" Бит на позиции %1: %2").arg(index).arg(val ? 1 : 0));
        indexInput->clear();
    } catch (const std::exception& e) {
        appendOutput(QString(" Ошибка: %1").arg(e.what()));
    }
}


void MainWindow::onGetFirst()
{
    if (currentType == 2 && currentBitSeq) {
        try {
            Bit val = currentBitSeq->GetFirst();
            appendOutput(QString(" Первый бит: %1").arg(val ? 1 : 0));
        } catch (const EmptySequenceException& e) {
            appendOutput(" BitSequence пуста");
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else if (currentSeq) {
        try {
            int val = currentSeq->GetFirst();
            appendOutput(QString(" Первый элемент: %1").arg(val));
        } catch (const EmptySequenceException& e) {
            appendOutput(" Последовательность пуста");
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else {
        appendOutput(" Последовательность не создана");
    }
}

void MainWindow::onGetLast()
{
    if (currentType == 2 && currentBitSeq) {
        try {
            Bit val = currentBitSeq->GetLast();
            appendOutput(QString(" Последний bit: %1").arg(val ? 1 : 0));
        } catch (const EmptySequenceException& e) {
            appendOutput(" BitSequence пуста");
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else if (currentSeq) {
        try {
            int val = currentSeq->GetLast();
            appendOutput(QString(" Последний элемент: %1").arg(val));
        } catch (const EmptySequenceException& e) {
            appendOutput(" Последовательность пуста");
        } catch (const std::exception& e) {
            appendOutput(QString(" Ошибка: %1").arg(e.what()));
        }
    } else {
        appendOutput(" Последовательность не создана");
    }
}
