//
// Created by Admin on 30.10.2025.
//

#include "AddWindow.h"

#include <iostream>

#include "multifile_functions.h"
using namespace std;

addWindow::addWindow(QWidget *parent)
    : QWidget(parent) {
    setWindowTitle("добавление строки");
    resize(800, 200);

    nameEdit = new QTextEdit(this);
    fieldEdit = new QTextEdit(this);
    doButton = new QPushButton("Применить", this);
    closeButton = new QPushButton("Закрыть", this);
    auto *inputLabel = new QLabel("Ввод названия", this);
    auto *outputLabel = new QLabel("Ввод полей", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(inputLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(outputLabel);
    layout->addWidget(fieldEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addWidget(doButton);

    layout->addLayout(buttonLayout);


    connect(closeButton, &QPushButton::clicked, this, &addWindow::closeWindow);
    connect(doButton, &QPushButton::clicked, this, &addWindow::addLine);
}


void addWindow::closeWindow() {
    close();
}



void addWindow::addLine() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name + ".txt";
    QFile file(filePath);
    int structure_len = 0;
    int infolen = 0;
    QStringList structure;
    vector<int> keyposes;
    vector<int> allposes;
    QStringList keyFields;
    QStringList keytypes;

    int numoflines = 0;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString line;
        line = in.readLine();
        infolen = line.length() + 1;
        structure = line.trimmed().split(" ");
        numoflines = structure[0].toInt();
        structure.pop_front();
        structure_len = 0;
        QMap<QString, int> lens = {{"int", 4}, {"str", 16}, {"bool", 1}, {"float", 4}};
        for (int i = 1; i < structure.length(); i += 3) {
            if (structure[i + 1] == "1") {
                keyposes.push_back(structure_len);
                keyFields.append(structure[i - 1]);
                keytypes.append(structure[i]);
            }
            allposes.push_back(structure_len);
            structure_len += lens[structure[i]];
        }
        file.close();
    } else {
        QString errorMsg = QString("Не удалось  файл");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
    QStringList args = this->fieldEdit->toPlainText().trimmed().split(" ");
    if (args.length() % 2) {
        QString errorMsg = QString("неккоректный ввод");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
    if (file.open(QIODevice::ReadWrite)) {
        int id = 0;
        int targetPos = 0;

        if (args[0] == "id") {
            targetPos = infolen + (args[1].toInt() - 1) * structure_len;
            id = args[1].toInt();
            file.seek(targetPos);
            QByteArray idn = file.read(4);
            if (decodeUnsignedInt(idn)) {
                QString errorMsg = QString("такой id уже есть");
                QMessageBox::warning(this, "Ошибка", errorMsg);
                return;
            }
        } else {
            int pos = infolen;
            while (pos < file.size()) {
                file.seek(pos);
                QByteArray idData = file.read(4);

                if (pos + structure_len >= file.size() and idData != QByteArray(4, '\0')) {
                    QMessageBox::warning(this, "wrong input", "file full");
                    return;
                }
                if (decodeUnsignedInt(idData) == 0) {
                    targetPos = pos;
                    id = (pos - infolen) / structure_len + 1;
                    args.prepend(QString("%1").arg(id));
                    args.prepend("id");
                    break;
                }

                pos += structure_len;
            }
        }
        if (args[1].toInt() > numoflines) {
            QMessageBox::warning(this, "wrong input", QString("%1 there is no such string").arg(args[1]));
            return;
        }
        // uniqueness check

        for (int j = 0; j < keyFields.length(); j++) {
            cout<<1<<'f';
            QString key = keyFields[j];
            int pos = keyposes[j];

            if (key == "id" && args.contains(key)) {
                file.seek(infolen + structure_len*(args[args.indexOf(key)+1].toInt() - 1) + pos);
                QByteArray f = file.read(4);
                QByteArray bytes(4,'\0');

                if (f != bytes or args[args.indexOf(key)+1].toInt() > numoflines) {
                    QMessageBox::warning(this, "wrong input", QString("%1 this value exists in key field i").arg(args[args.indexOf(key)+1].toInt()));
                    return;
                }
                continue;
            }
            QMap<QString, int> lens = {{"int", 4}, {"str", 16}, {"bool", 1}, {"float", 4}};
            QString codetype = keytypes[j];

            QByteArray checkdata = univencoder(args[args.indexOf(key)+1], codetype);

            cout<<key.toStdString()<<endl;
            vector<unsigned> checker = findvalue(name+"/"+key+".txt", checkdata, codetype, numoflines, filePath, structure_len, infolen, pos, 1);
            for (auto el:checker) {
                cout<<el<<'f'<<endl;
            }
            if (checker[0]!=0) {
                cout<<checker[0]<<endl;
                QMessageBox::warning(this, "wrong input", "this value exists in key field " + key);
                return;
            }

        }


        //
        QByteArray lineData;
        file.seek(targetPos);
        for (int i = 0; i < args.size(); i += 2) {
            int posid = structure.indexOf(args[i]);
            QString codetype = structure[posid + 1];
            QByteArray data;
            if (args[i] != "id") {
                writevalue(name+"/"+args[i]+".txt", args[i+1], codetype,numoflines, id);
            }
            if (codetype == "int") {
                if (args[i] != "id") {
                    data = encodeInt(args[i + 1].toInt());
                } else {
                    data = (encodeUnsignedInt(args[i + 1].toInt()));
                }
            } else if (codetype == "float") {
                data = (encodeFloat(args[i + 1].toFloat()));
            } else if (codetype == "bool") {
                data = (encodeBool(args[i + 1].toInt()));
            } else if (codetype == "str") {
                QString t = args[i + 1];
                t.resize(16, '\0');
                data = (t.toUtf8());
            }
            file.seek(targetPos + allposes[posid / 3]);
            file.write(data);
        }

        file.flush();
        QMessageBox::information(this, "Success", "Critical success");
        addWindow::closeWindow();

        file.close();
    }
}
