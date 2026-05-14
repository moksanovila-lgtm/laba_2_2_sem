#pragma once

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include "SequenceController.hpp"

class SequenceWidget : public QWidget {
    Q_OBJECT
    
private:
    SequenceController* controller;
    QListWidget* listWidget;
    QLabel* infoLabel;
    QLineEdit* valueInput;
    QLineEdit* secondSeqInput;
    QPushButton* appendBtn;
    QPushButton* prependBtn;
    QPushButton* mapBtn;
    QPushButton* whereBtn;
    QPushButton* reduceBtn;
    QPushButton* concatBtn;
    QPushButton* subseqBtn;
    QPushButton* andBtn;
    QPushButton* orBtn;
    QPushButton* xorBtn;
    QPushButton* notBtn;
    
    int selectedStart;
    int selectedEnd;
    bool hasStart;
    
    void setupUI();
    void updateDisplay();
    void clearSelection();
    bool isBitSequence() const;
    
private slots:
    void onAppend();
    void onPrepend();
    void onMap();
    void onWhere();
    void onReduce();
    void onConcat();
    void onSubsequence();
    void onAnd();
    void onOr();
    void onXor();
    void onNot();
    void onItemClicked(QListWidgetItem* item);
    void showContextMenu(const QPoint& pos);
    void onItemDoubleClicked(QListWidgetItem* item);
    void onError(const QString& msg);
    
public:
    explicit SequenceWidget(SequenceController* ctrl, QWidget* parent = nullptr);
    ~SequenceWidget();
    
    void refresh();
    
signals:
    void newSequenceCreated(SequenceController* controller, const QString& name);
};