#pragma once

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QTabWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "Sequence.hpp"
#include "ArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ListSequence.hpp"
#include "ImmutableListSequence.hpp"
#include "BitSequence.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Sequence<int>* currentSeq;
    BitSequence* currentBitSeq;
    int currentType;  
    
    QTabWidget* tabWidget;
    QTextEdit* outputArea;
    QLabel* statusLabel;
    
    QComboBox* typeCombo;
    QComboBox* mutabilityCombo;
    QPushButton* createBtn;
    
    QLineEdit* valueInput;
    QLineEdit* indexInput;
    QLineEdit* startInput;
    QLineEdit* endInput;
    QPushButton* appendBtn;
    QPushButton* prependBtn;
    QPushButton* insertBtn;
    QPushButton* getBtn;
    QPushButton* setBtn;
    QPushButton* clearBtn;
    QPushButton* showBtn;
    QPushButton* subsequenceBtn;
    QPushButton* concatBtn;

    QPushButton* getFirstBtn;   
    QPushButton* getLastBtn;
    
    QPushButton* mapBtn;
    QPushButton* whereBtn;
    QPushButton* reduceBtn;
    QPushButton* iteratorBtn;
    
    QPushButton* andBtn;
    QPushButton* orBtn;
    QPushButton* xorBtn;
    QPushButton* notBtn;
    QLineEdit* bitInput;
    
    void setupUI();
    void createTabCreation();
    void createTabOperations();
    void createTabMapReduce();
    void createTabBit();
    void createTabOutput();
    
    void appendOutput(const QString& text);
    void updateStatus();
    template<typename T> QString sequenceToString(Sequence<T>* seq);
    
private slots:
    void onCreate();
    void onShow();
    void onAppend();
    void onPrepend();
    void onInsert();
    void onGet();
    void onGetFirst();   
    void onGetLast();
    void onClear();
    void onGetSubsequence();
    void onConcat();
    void onMap();
    void onWhere();
    void onReduce();
    void onIterator();
    void onBitAnd();
    void onBitOr();
    void onBitXor();
    void onBitNot();
    void onBitSet();
    void onBitGet();

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};
