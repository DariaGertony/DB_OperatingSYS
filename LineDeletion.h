//
// Created by Admin on 31.10.2025.
//

#ifndef DBSYS_LINEDELETION_H
#define DBSYS_LINEDELETION_H




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

class LineDeletion : public QWidget
{
    Q_OBJECT

public:
    explicit LineDeletion(QWidget *parent = nullptr);


private slots:
    void closeWindow();
    void selLine();
    void delLine();

private:
    QTextEdit *nameEdit;
    QTextEdit *fieldEdit;
    QTextEdit *outputEdit{};
    QPushButton *closeButton;
    QPushButton *doButton;
    QPushButton *delButton;
};


#endif //DBSYS_LINEDELETION_H