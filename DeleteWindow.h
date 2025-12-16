//
// Created by Admin on 30.10.2025.
//

#ifndef DBSYS_DELETEWINDOW_H
#define DBSYS_DELETEWINDOW_H



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
#include <QDir>
#include <QMessageBox>

class DeleteWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DeleteWindow(QWidget *parent = nullptr);


private slots:
    void closeWindow();
    void deleteFile();


private:
    QTextEdit *nameEdit;
    QPushButton *closeButton;
    QPushButton *doButton;

};


#endif //DBSYS_DELETEWINDOW_H