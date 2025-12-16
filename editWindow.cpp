
#include "editWindow.h"
#include <QSet>
#include <QStringList>
#include <iostream>

#include "multifile_functions.h"
using namespace std;

editWindow::editWindow(QWidget *parent)
    : QWidget(parent) {
    setWindowTitle("Select/Delete lines");
    resize(800, 200);

    nameEdit = new QTextEdit(this);
    fieldEdit = new QTextEdit(this);
    outputEdit = new QTextEdit(this);

    doButton = new QPushButton("Найти", this);
    closeButton = new QPushButton("Закрыть", this);
    auto *inputLabel = new QLabel("Ввод названия", this);
    auto *outputLabel = new QLabel("Ввод полей criteria", this);
    auto *outLabel = new QLabel("new info", this);
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

    layout->addLayout(buttonLayout);


    connect(closeButton, &QPushButton::clicked, this, &editWindow::closeWindow);
    connect(doButton, &QPushButton::clicked, this, &editWindow::selLine);

}


void editWindow::closeWindow() {
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

void editWindow::addLines(vector<QByteArray> lines, QStringList structure, int structurelen, int infolen, vector<int> allposes, int numoflines) {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name + ".txt";
    QFile file(filePath);
    if (file.open(QFile::ReadWrite)) {
        for (auto str : lines) {
            QString result = "id";
            result += "\t";
            int id = decodeUnsignedInt(str.sliced(0, 4));
            if (id == 0) {
                continue;
            }
            result += QString("%1").arg(id);
            result += "\t";
            int psl = 4;
            for (int i = 4; i < structure.size(); i+=3) {
                result += QString("%1").arg(structure[i-1]);
                result += "\t";
                if (structure[i]=="int") {
                    result += QString("%1").arg(decodeInt(str.sliced(psl, 4)));
                    result += "\t";
                    psl+=4;
                }
                if (structure[i]=="bool") {
                    result += decodebool(str.sliced(psl, 1));
                    result += "\t";
                    psl+=1;
                }
                if (structure[i]=="str") {
                    result += str.sliced(psl, 16);
                    result += "\t";
                    psl+=16;
                }
                if (structure[i]=="float") {
                    result += QString("%1").arg(decodeFloat(str.sliced(psl, 4)));
                    result += "\t";
                    psl+=4;
                }
            }
            cout<<result.toStdString()<<endl;
            QStringList args = result.trimmed().split("\t");
            for (auto arg:args) {
                cout<<arg.toStdString()<<endl;
            }
            // replace args
            QStringList newargs = this->outputEdit->toPlainText().trimmed().split(" ");
            for (int j = 0; j < newargs.length(); j+=2) {
                args[args.indexOf(newargs[j])+ 1] = newargs[j+1];
            }
            int targetPos = infolen + (args[1].toInt() - 1) * structurelen;
            for (int i = 0; i < args.size(); i += 2) {
                int posid = structure.indexOf(args[i]);
                QString codetype = structure[posid + 1];
                QByteArray data;
                if (args[i] != "id") {
                    writevalue(name+"/"+args[i]+".txt", args[i+1], codetype, numoflines, id);
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
        }
    }
    QMessageBox::information(this, "Success", "Critical success");
    editWindow::closeWindow();
}

void editWindow::selLine() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QString filePath = name + ".txt";
    QFile file(filePath);
    int structure_len = 0;
    int infolen = 0;
    QStringList structure;
    vector<int> allposes;
    vector<int> keyposes;
    QStringList keyFields;
    QStringList keytypes;

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
            if (structure[i + 1] == "1") {
                keyposes.push_back(structure_len);
                keyFields.append(structure[i - 1]);
                keytypes.append(structure[i]);
            }
        }
        file.close();
    } else {
        QString errorMsg = QString("Не удалось  файл");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
    QStringList newargs = this->outputEdit->toPlainText().trimmed().split(" ");
    for (int j = 0; j < keyFields.length(); j++) {
        cout<<1<<'f';
        QString key = keyFields[j];
        int pos = keyposes[j];

        if (key == "id" && newargs.contains(key)) {
            file.seek(infolen + structure_len*(newargs[newargs.indexOf(key)+1].toInt() - 1) + pos);
            QByteArray f = file.read(4);
            QByteArray bytes(4,'\0');

            if (f != bytes or newargs[newargs.indexOf(key)+1].toInt() > numoflines) {
                QMessageBox::warning(this, "wrong input", QString("%1 this value exists in key field i").arg(newargs[newargs.indexOf(key)+1].toInt()));
                return;
            }
            continue;
        }
        QMap<QString, int> lens = {{"int", 4}, {"str", 16}, {"bool", 1}, {"float", 4}};
        QString codetype = keytypes[j];

        QByteArray checkdata = univencoder(newargs[newargs.indexOf(key)+1], codetype);

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
    QSet<QString> set1(newargs.begin(), newargs.end());
    QSet<QString> set2(keyFields.begin(), keyFields.end());
    QSet set3 = set1.intersect(set2);

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
        int ifkey = structure[structure.indexOf(args[i]) + 1].toInt();
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
    if (set3.size() != 0 and intersects.size() > 1) {
        QMessageBox::warning(this, "wrong input", QString("u r trying to put key field to more than 1 line"));
        return;
    }
    vector<QByteArray> linestoedit;
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
            QByteArray r = file.read(structure_len);
            linestoedit.push_back(r);
            file.seek(infolen + (x-1)*structure_len);
            QByteArray cleer(structure_len, '\0');
            file.write(cleer);
        }
        file.close();
    }
    addLines(linestoedit, structure,structure_len, infolen, allposes, numoflines);

}
