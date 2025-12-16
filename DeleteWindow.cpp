//
// Created by Admin on 30.10.2025.
//

#include "DeleteWindow.h"

#include <iostream>

using namespace std;

void DeleteWindow::deleteFile() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name +".txt";
    QString folderPath = name;
    QFile file(filePath);

    if (!file.exists()) {
        QString errorMsg = QString("неккоректный ввод");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }

    if (file.remove()) {
        QDir dir(folderPath);
        if (!dir.exists()) {
            qDebug() << "Папка не существует:" << folderPath;
        }

        bool success = dir.removeRecursively();
        if (!success) {
            qDebug() << "❌ Не удалось удалить папку:" << folderPath;
        } else {
            qDebug() << "✅ Папка удалена:" << folderPath;
        }
        QMessageBox::information(this, "Success", "critical success");
        DeleteWindow::closeWindow();
    } else {
        QMessageBox::warning(this, "Ошибка", file.errorString());
        return;
    }


}

DeleteWindow::DeleteWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Удаление файла" );
    resize(800, 200);

    nameEdit = new QTextEdit(this);
    doButton = new QPushButton("Удалить", this);
    closeButton = new QPushButton("Закрыть", this);
    auto *inputLabel = new QLabel( "Ввод названия", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(inputLabel);
    layout->addWidget(nameEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addWidget(doButton);

    layout->addLayout(buttonLayout);


    connect(closeButton, &QPushButton::clicked, this, &DeleteWindow::closeWindow);
    connect(doButton, &QPushButton::clicked, this, &DeleteWindow::deleteFile);
}

void DeleteWindow::closeWindow()
{
    close();
}