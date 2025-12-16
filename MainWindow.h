//
// Created by Admin on 29.10.2025.
//


#ifndef DBSYS_MAINWINDOW_H
#define DBSYS_MAINWINDOW_H
#include <QApplication>
#include "FileViewer.h"
#include "CreateWindow.h"
#include "DeleteWindow.h"
#include "OpenWindow.h"
#include "AddWindow.h"
#include "LineDeletion.h"
#include "editWindow.h"
#include "zipWindow.h"



class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr) : QWidget(parent)
    {

        QPushButton *createButton = new QPushButton("Создать/очистить БД", this);
        QPushButton *deleteButton = new QPushButton("Удалить БД", this);
        QPushButton *zipButton = new QPushButton("сохранить копию БД", this);
        QPushButton *openButton = new QPushButton("открыть БД", this);
        QPushButton *addButton = new QPushButton("добавить", this);
        QPushButton *lineButton = new QPushButton("Select/Delete lines", this);
        QPushButton *showButton = new QPushButton("Help", this);
        QPushButton *editButton = new QPushButton("edit lines", this);
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        mainLayout->addWidget(createButton);
        mainLayout->addWidget(addButton);
        mainLayout->addWidget(openButton);
        mainLayout->addWidget(lineButton);
        mainLayout->addWidget(editButton);
        mainLayout->addWidget(deleteButton);
        mainLayout->addWidget(zipButton);
        mainLayout->addWidget(showButton);

        setLayout(mainLayout);
        setWindowTitle("");
        resize(600, 500);

        connect(createButton, &QPushButton::clicked, this, &MainWindow::createDB);
        connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteDB);
        connect(zipButton, &QPushButton::clicked, this, &MainWindow::zipDB);
        connect(addButton, &QPushButton::clicked, this, &MainWindow::addtoDB);
        connect(openButton, &QPushButton::clicked, this, &MainWindow::openDB);
        connect(lineButton, &QPushButton::clicked, this, &MainWindow::lineDB);
        connect(editButton, &QPushButton::clicked, this, &MainWindow::editDB);
        connect(showButton, &QPushButton::clicked, this, &MainWindow::showRules);
    }

private slots:
    static void createDB();
    static void deleteDB();

    static void zipDB();

    static void showRules();
    static void openDB();
    static void lineDB();


    static void addtoDB();

    static void editDB();

private:
    QTextEdit *inputTextEdit;
    QTextEdit *outputTextEdit;
};


#endif //DBSYS_MAINWINDOW_H