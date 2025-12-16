//
// Created by Admin on 30.10.2025.
//

#ifndef DBSYS_OPENWINDOW_H
#define DBSYS_OPENWINDOW_H




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

class OpenWindow : public QWidget
{
    Q_OBJECT

public:
    explicit OpenWindow(QWidget *parent = nullptr);


private slots:
    void closeWindow();


    void OpenFile();

private:
    QTextEdit *nameEdit;
    QTextEdit *output;
    QPushButton *closeButton;
    QPushButton *doButton;
};


#endif //DBSYS_OPENWINDOW_H