//
// Created by Admin on 16.11.2025.
//

#ifndef DBSYS_EDITWINDOW_H
#define DBSYS_EDITWINDOW_H



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

class editWindow : public QWidget
{
    Q_OBJECT

public:
    explicit editWindow(QWidget *parent = nullptr);


private slots:
    void closeWindow();

    void addLines(std::vector<QByteArray> lines, QStringList structure, int structurelen, int infolen, std::vector<int> allposes, int
                  numoflines);

    void selLine();

private:
    QTextEdit *nameEdit;
    QTextEdit *fieldEdit;
    QTextEdit *outputEdit;
    QPushButton *closeButton;
    QPushButton *doButton;
};

#endif //DBSYS_EDITWINDOW_H