//
// Created by Admin on 16.11.2025.
//

#include "zipWindow.h"
#include <cstdlib>

#include "multifile_functions.h"
#include <iostream>

using namespace std;

void zipWindow::deleteFile() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name +".txt";
    QFile file(filePath);

    if (!file.exists()) {
        QString errorMsg = QString("неккоректный ввод");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
    int result = std::system(("..\\.venv\\Scripts\\python.exe ..\\cmake-build-debug\\script.py " + name.toStdString()).c_str());
    if (result == 0) {
        QMessageBox::information(this, "Success", "critical success");
        zipWindow::closeWindow();
    } else {
        QMessageBox::warning(this, "Ошибка", QString("%1").arg(result));
        return;
    }

}

void zipWindow::toxlx() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name + ".txt";
    QFile file(filePath);
    QFile text(name+"decoded.txt");
    QStringList structure;
    int structure_len = 0;
    QString result;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream textStream(&file);
        QString line = textStream.readLine();
        structure = line.trimmed().split(" ");
        structure.pop_front();
        if (structure.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Файл пуст или структура некорректна.");
            return;
        }

        QMap<QString, int> lens = {{"int", 4}, {"str", 16}, {"bool", 1}, {"float", 4}};
        for (int i = 1; i < structure.length(); i += 3) {
            if (lens.contains(structure[i])) {
                structure_len += lens[structure[i]];
            } else {
                QMessageBox::warning(this, "Ошибка", "Неизвестный тип в структуре: " + structure[i]);
                return;
            }
        }

        file.seek(textStream.pos());

        QByteArray binaryData = file.readAll();


        QByteArrayList Codeddata;
        for (int i = 0; i < binaryData.size(); i += structure_len) {
            QByteArray chunk = binaryData.mid(i, structure_len);
            if (chunk.size() == structure_len) {
                Codeddata.append((chunk));
            }
        }

        result = decodeOutput(structure, structure_len, Codeddata);

        file.close();
    }
    else {
        QString errorMsg = QString("Не удалось открыть файл");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }

    if (text.open(QIODevice::WriteOnly | QIODevice::Text)) {
        text.write(result.toStdString().c_str());


    }
    else{
        QString errorMsg = QString("Не удалось открыть файл");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
    file.close();
    QMessageBox::information(this, "Success", "critical success");




}



void zipWindow::toxlxpy() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name +".txt";
    QString textfilepath = name +"decoded.txt";
    int res = std::system(("..\\.venv\\Scripts\\python.exe ..\\cmake-build-debug\\xlsxscript.py " + name.toStdString()+ " ..\\cmake-build-debug\\" + textfilepath.toStdString()).c_str());
    if (res == 0) {
        QMessageBox::information(this, "Success", "critical success");
    } else {
        QMessageBox::warning(this, "Ошибка", QString("%1").arg(res));
        return;
    }
}

void zipWindow::unzipFile() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name +".txt";
    QFile file(filePath);

    if (!file.exists()) {
        QString errorMsg = QString("неккоректный ввод");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
    int result = std::system(("..\\.venv\\Scripts\\python.exe ..\\cmake-build-debug\\unscript.py " + name.toStdString()).c_str());
    if (result == 0) {
        QMessageBox::information(this, "Success", "critical success");
    } else {
        QMessageBox::warning(this, "Ошибка", QString("%1").arg(result));
        return;
    }

}

zipWindow::zipWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Удаление файла" );
    resize(800, 200);

    nameEdit = new QTextEdit(this);
    doButton = new QPushButton("zip", this);
    undoButton = new QPushButton("unzip", this);
    textButton = new QPushButton("to text", this);
    xlxButton = new QPushButton("to xlsx (if text created)", this);
    closeButton = new QPushButton("Закрыть", this);
    auto *inputLabel = new QLabel( "Ввод названия", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(inputLabel);
    layout->addWidget(nameEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addWidget(doButton);
    buttonLayout->addWidget(undoButton);
    buttonLayout->addWidget(xlxButton);
    buttonLayout->addWidget(textButton);

    layout->addLayout(buttonLayout);


    connect(closeButton, &QPushButton::clicked, this, &zipWindow::closeWindow);
    connect(doButton, &QPushButton::clicked, this, &zipWindow::deleteFile);
    connect(undoButton, &QPushButton::clicked, this, &zipWindow::unzipFile);
    connect(xlxButton, &QPushButton::clicked, this, &zipWindow::toxlxpy);
    connect(textButton, &QPushButton::clicked, this, &zipWindow::toxlx);
}

void zipWindow::closeWindow()
{
    close();
}