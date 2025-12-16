//
// Created by Admin on 30.10.2025.
//

#include "OpenWindow.h"

#include <iostream>


using namespace std;
#include "multifile_functions.h"
void OpenWindow::OpenFile() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name + ".txt";
    QFile file(filePath);
    QStringList structure;
    int structure_len = 0;

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

        QString result = decodeOutput(structure, structure_len, Codeddata);
        OpenWindow::output->setPlainText(result);
        file.close();
    }
    else {
        QString errorMsg = QString("Не удалось открыть файл");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
}
OpenWindow::OpenWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Чтение файла" );
    resize(800, 200);

    nameEdit = new QTextEdit(this);
    output = new QTextEdit(this);
    output->setReadOnly(true);
    doButton = new QPushButton("Применить", this);
    closeButton = new QPushButton("Закрыть", this);
    auto *inputLabel = new QLabel( "Ввод названия", this);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(inputLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(output);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addWidget(doButton);

    layout->addLayout(buttonLayout);


    connect(closeButton, &QPushButton::clicked, this, &OpenWindow::closeWindow);
    connect(doButton, &QPushButton::clicked, this, &OpenWindow::OpenFile);
}

void OpenWindow::closeWindow()
{
    close();
}