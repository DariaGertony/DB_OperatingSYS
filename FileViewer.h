//
// Created by Admin on 29.10.2025.
//

#ifndef DBSYS_FILEVIEWER_H
#define DBSYS_FILEVIEWER_H

#include <QWidget>

class QTextEdit;
class QPushButton;
class QVBoxLayout;

class FileViewer : public QWidget
{
    Q_OBJECT

public:
    explicit FileViewer(QWidget *parent = nullptr);

private slots:
    void loadFile();
    void closeWindow();

private:
    QTextEdit *textEdit;
    QPushButton *closeButton;
    QString filePath = "commands.txt";
};

#endif //DBSYS_FILEVIEWER_H