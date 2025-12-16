//
// Created by Admin on 30.10.2025.
//

#ifndef DBSYS_ADDWINDOW_H
#define DBSYS_ADDWINDOW_H



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

class addWindow : public QWidget
{
    Q_OBJECT

public:
    explicit addWindow(QWidget *parent = nullptr);


private slots:
    void closeWindow();
    void addLine();

private:
    QTextEdit *nameEdit;
    QTextEdit *fieldEdit;
    QPushButton *closeButton;
    QPushButton *doButton;
};

#endif //DBSYS_ADDWINDOW_H