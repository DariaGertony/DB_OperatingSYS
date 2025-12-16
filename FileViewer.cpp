#include "FileViewer.h"
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextStream>
#include <QFileInfo>
#include <QMessageBox>

FileViewer::FileViewer(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Просмотр файла: " + QFileInfo(filePath).fileName());
    resize(800, 600);

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    closeButton = new QPushButton("Закрыть", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(textEdit);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(closeButton);
    
    layout->addLayout(buttonLayout);

    loadFile();

    connect(closeButton, &QPushButton::clicked, this, &FileViewer::closeWindow);
}

void FileViewer::loadFile()
{

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString content = stream.readAll();
        textEdit->setPlainText(content);
        file.close();
        setWindowTitle("Просмотр файла: " + QFileInfo(filePath).fileName());

    } else {
        QString errorMsg = QString("Не удалось открыть файл:\n%1\n\nОшибка: %2")
                              .arg(filePath)
                              .arg(file.errorString());
        textEdit->setPlainText(errorMsg);
        setWindowTitle("Просмотр файла: ОШИБКА");
        QMessageBox::warning(this, "Ошибка", errorMsg);
    }
}
void FileViewer::closeWindow()
{
    close();
}