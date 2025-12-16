//
// Created by Admin on 31.10.2025.
//

#include "LineDeletion.h"

#include <iostream>

#include "multifile_functions.h"
using namespace std;

LineDeletion::LineDeletion(QWidget *parent)
    : QWidget(parent) {
    setWindowTitle("Select/Delete lines");
    resize(800, 200);

    nameEdit = new QTextEdit(this);
    fieldEdit = new QTextEdit(this);
    outputEdit = new QTextEdit(this);
    outputEdit->setReadOnly(true);

    doButton = new QPushButton("Найти", this);
    delButton = new QPushButton("Удалитб", this);
    closeButton = new QPushButton("Закрыть", this);
    auto *inputLabel = new QLabel("Ввод названия", this);
    auto *outputLabel = new QLabel("Ввод полей", this);
    auto *outLabel = new QLabel("Output", this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(inputLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(outputLabel);
    layout->addWidget(fieldEdit);
    layout->addWidget(outLabel);
    layout->addWidget(outputEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addWidget(doButton);
    buttonLayout->addWidget(delButton);

    layout->addLayout(buttonLayout);


    connect(closeButton, &QPushButton::clicked, this, &LineDeletion::closeWindow);
    connect(doButton, &QPushButton::clicked, this, &LineDeletion::selLine);

    connect(delButton, &QPushButton::clicked, this, &LineDeletion::delLine);
}


void LineDeletion::closeWindow() {
    close();
}



template<typename T>
std::vector<T> intersect_all(const std::vector<std::vector<T>>& vectors) {
    if (vectors.empty()) return {};

    std::unordered_map<T, int> count;

    // Пройдём по каждому вектору, но считаем элемент *не более одного раза* в каждом векторе
    for (const auto& vec : vectors) {
        std::unordered_set<T> seen_in_this;
        for (const T& x : vec) {
            if (seen_in_this.insert(x).second) {  // true — если вставлено (т.е. первый раз в этом векторе)
                count[x]++;
            }
        }
    }

    // Элементы, встретившиеся во всех векторах:
    std::vector<T> result;
    int n = vectors.size();
    for (const auto& [elem, cnt] : count) {
        if (cnt == n && elem>0) {
            result.push_back(elem);
        }
    }
    return result;
}



void LineDeletion::selLine() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name + ".txt";
    QFile file(filePath);
    int structure_len = 0;
    int infolen = 0;
    QStringList structure;
    vector<int> allposes;

    int numoflines = 1;
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
    vector<vector<unsigned int>> results;
    for (int i = 0; i < args.length(); i += 2) {
        if (args[i] == "id") {
            vector<unsigned int> ids;
            ids.push_back(args[i+1].toInt());
            if (args[i+1].toInt() > numoflines) {
                QMessageBox::warning(this, "wrong input", QString("%1 there is no such string").arg(args[1]));
                return;
            }
            results.push_back(ids);
            continue;
        }
        int strcpos = structure.indexOf(args[i]);
        QString type = structure[structure.indexOf(args[i]) + 1];
        int ifkey = structure[structure.indexOf(args[i]) + 2].toInt();
        if (type == "int") {
            results.push_back(findvalue(name+"/"+args[i]+".txt", encodeInt(args[i+1].toInt()),type, numoflines, filePath, structure_len, infolen, allposes[strcpos/3],ifkey));
        }
        else if (type == "bool") {
            results.push_back(findvalue(name+"/"+args[i]+".txt", encodeBool(args[i+1].toInt()),type, numoflines, filePath, structure_len, infolen, allposes[strcpos/3],ifkey));
        }
        else if (type == "float") {
            results.push_back(findvalue(name+"/"+args[i]+".txt", encodeFloat(args[i+1].toFloat()),type, numoflines, filePath, structure_len, infolen, allposes[strcpos/3],ifkey));
        }
        else if (type == "str") {
            results.push_back(findvalue(name+"/"+args[i]+".txt", args[i+1].toUtf8(),type, numoflines, filePath, structure_len, infolen, allposes[strcpos/3],ifkey));
        }

    }
    vector<unsigned int> intersects = intersect_all(results);
    QByteArrayList lines;
    if (file.open(QIODevice::ReadOnly)) {
        for (const auto& x : intersects) {
            file.seek(infolen + (x-1)*structure_len);
            QByteArray f = file.read(structure_len);
            lines += f;
        }
    }
    QString out = decodeOutput(structure, structure_len,lines);
    outputEdit->setPlainText(out);
    QMessageBox::information(this, "Success", "Critical success");
}

void LineDeletion::delLine() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name + ".txt";
    QFile file(filePath);
    int structure_len = 0;
    int infolen = 0;
    QStringList structure;
    vector<int> allposes;

    int numoflines = 100000;
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
    vector<vector<unsigned int>> results;
    for (int i = 0; i < args.length(); i += 2) {
        if (args[i] == "id") {
            vector<unsigned int> ids;
            ids.push_back(args[i+1].toInt());
            if (args[i+1].toInt() > numoflines) {
                QMessageBox::warning(this, "wrong input", QString("%1 there is no such string").arg(args[1]));
                return;
            }
            results.push_back(ids);
            continue;
        }
        int strcpos = structure.indexOf(args[i]);
        QString type = structure[structure.indexOf(args[i]) + 1];

        int ifkey = structure[structure.indexOf(args[i]) + 2].toInt();
        if (type == "int") {
            results.push_back(findvalue(name+"/"+args[i]+".txt", encodeInt(args[i+1].toInt()),type, numoflines, filePath, structure_len, infolen, allposes[strcpos/3],ifkey));
        }
        else if (type == "bool") {
            results.push_back(findvalue(name+"/"+args[i]+".txt", encodeBool(args[i+1].toInt()),type, numoflines, filePath, structure_len, infolen, allposes[strcpos/3],ifkey));
        }
        else if (type == "float") {
            results.push_back(findvalue(name+"/"+args[i]+".txt", encodeFloat(args[i+1].toFloat()),type, numoflines, filePath, structure_len, infolen, allposes[strcpos/3],ifkey));
        }
        else if (type == "str") {
            results.push_back(findvalue(name+"/"+args[i]+".txt", args[i+1].toUtf8(),type, numoflines, filePath, structure_len, infolen, allposes[strcpos/3],ifkey));
        }

    }
    vector<unsigned int> intersects = intersect_all(results);
    if (file.open(QIODevice::ReadWrite)) {
        for (const auto& x : intersects) {
            for (int i = 0; i < structure.length(); i += 3) {
                QFile afile(name + "/" + structure[i] + ".txt");
                if (afile.open(QIODevice::ReadWrite)) {
                    QByteArray data = afile.readAll();
                    QByteArray f(4, 255);
                    cout<<encodeUnsignedInt(x).toStdString();
                    cout<<x;
                    data.replace(encodeUnsignedInt(x), f);
                    afile.seek(0);
                    afile.write(data);
                }
            }
            cout<<x<<endl;
            file.seek(infolen + (x-1)*structure_len);
            cout<<infolen + (x-1)*structure_len;
            QByteArray cleer(structure_len, '\0');
            file.write(cleer);
        }
        file.close();
    }
    QMessageBox::information(this, "Success", "Critical success");
    LineDeletion::closeWindow();
}
