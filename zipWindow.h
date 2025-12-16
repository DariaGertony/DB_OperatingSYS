//
// Created by Admin on 16.11.2025.
//

#ifndef DBSYS_ZIPWINDOW_H
#define DBSYS_ZIPWINDOW_H




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

class zipWindow : public QWidget
{
    Q_OBJECT

public:
    explicit zipWindow(QWidget *parent = nullptr);


private slots:
    void closeWindow();
    void deleteFile();
    void toxlx();

    void toxlxpy();

    void unzipFile();


private:
    QTextEdit *nameEdit;
    QPushButton *closeButton;
    QPushButton *doButton;
    QPushButton *textButton;
    QPushButton *xlxButton;
    QPushButton *undoButton;

};



#endif //DBSYS_ZIPWINDOW_H