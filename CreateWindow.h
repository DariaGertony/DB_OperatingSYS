//
// Created by Admin on 30.10.2025.
//

#ifndef DBSYS_CREATEWINDOW_H
#define DBSYS_CREATEWINDOW_H



#include <QWidget>
#include <QFile>
#include <QBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextStream>
#include <QFileInfo>
#include <QMessageBox>

class CreateWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CreateWindow(QWidget *parent = nullptr);


private slots:
    void closeWindow();
    void createFile();
    void clearFile();

private:
    QTextEdit *nameEdit;
    QTextEdit *fieldEdit;
    QPushButton *closeButton;
    QPushButton *doButton;
    QPushButton *clearButton;
};

#endif //DBSYS_CREATEWINDOW_H