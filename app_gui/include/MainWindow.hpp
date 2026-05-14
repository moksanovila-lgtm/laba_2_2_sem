#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QInputDialog> 
#include <QMessageBox>
#include "SequenceController.hpp"
#include "SequenceWidget.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
    
private:
    QTabWidget* tabWidget;
    QComboBox* typeCombo;
    QComboBox* mutabilityCombo;
    QPushButton* createBtn;
    
    void setupUI();
    
private slots:
    void onCreate();
    void onTabClose(int index);
    void addNewTab(SequenceController* controller, const QString& name);
    
public:
    MainWindow(QWidget* parent = nullptr);
};