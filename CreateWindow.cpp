//
// Created by Admin on 30.10.2025.
//

#include "CreateWindow.h"
#include <QDir>

using namespace std;

bool createDirectoryIfNotExists(const QString &path) {
    QDir dir(path);

    if (dir.exists()) {
        qDebug() << "Папка уже существует:" << path;
        return true;
    }

    if (dir.mkpath(".")) {
        qDebug() << "Папка создана:" << path;
        return true;
    } else {
        qDebug() << "Ошибка создания папки:" << path;
        return false;
    }
}


void CreateWindow::createFile() {
    QString fields = this->fieldEdit->toPlainText();
    QStringList args = fields.trimmed().split(' ');
    args.prepend(QString("1"));
    args.prepend(QString("int"));
    args.prepend(QString("id"));
    QString name = this->nameEdit->toPlainText().trimmed();
    QStringList n_s = name.trimmed().split(' ');
    if (n_s.count() < 2) {
        QString errorMsg = QString("неккоректный ввод");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
    QString filePath = n_s[0] +".txt";
    QFile file(filePath);
    int structure_len = 0;
    int numoflines = n_s[1].toInt();
    if (args.length()%3!=0) {
        QString errorMsg = QString("неккоректный ввод");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
    QMap<QString, int> lens = {{"int", 4}, {"str", 16}, {"bool", 1}, {"float", 4}};
    for (int i = 1; i < args.length(); i+=3) {

        if (!lens.contains(args[i])) {
            QString errorMsg = QString("неккоректный ввод");
            QMessageBox::warning(this, "Ошибка", errorMsg);
            return;
        }
        structure_len+= lens[args[i]];

    }

    createDirectoryIfNotExists(n_s[0]);
    for (int i = 0; i < args.length(); i+=3) {
        QString valfilepath = n_s[0]+"/"+args[i]+".txt";
        QFile valfile(valfilepath);
        if (valfile.open(QIODevice::WriteOnly)) {
            QTextStream stream(&valfile);


            QByteArray zeros(4 * numoflines * 2, '\0');
            valfile.write(zeros);

            valfile.close();


        } else {
            QString errorMsg = QString("Не удалось открыть файл");
            QMessageBox::warning(this, "Ошибка", errorMsg);
            return;
        }

    }

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream<<n_s[1]<<' ';
        for (int i = 0; i < args.length(); i++) {
            stream << args[i] << ' ';
        }

        stream << '\n';
        stream.flush();

        QByteArray zeros(structure_len * numoflines, '\0');
        file.write(zeros);

        file.close();
        QMessageBox::information(this, "Success", "critical success");
        CreateWindow::closeWindow();


    } else {
        QString errorMsg = QString("Не удалось открыть файл");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
}

void CreateWindow::clearFile() {
    QString name = this->nameEdit->toPlainText().trimmed();
    QStringList n_s = name.trimmed().split(' ');
    QString filePath = n_s[0] +".txt";
    QFile file(filePath);
    int structure_len = 0;
    QStringList structure;
    if (n_s.count() < 2) {
        QString errorMsg = QString("неккоректный ввод");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
    int numoflines = n_s[1].toInt();
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString line;
        line = in.readLine();
        structure = line.trimmed().split(" ");
        structure.pop_front();
        structure_len = 0;
        QMap<QString, int> lens = {{"int", 4}, {"str", 16}, {"bool", 1}, {"float", 4}};
        for (int i = 1; i < structure.length(); i+=3) {
            structure_len+= lens[structure[i]];
        }
        file.close();
    }
    QMap<QString, int> lens = {{"int", 4}, {"str", 16}, {"bool", 1}, {"float", 4}};
    for (int i = 1; i < structure.length(); i+=3) {
        if (!lens.contains(structure[i])) {
            QString errorMsg = QString("неккоректный файл");
            QMessageBox::warning(this, "Ошибка", errorMsg);
            return;
        }
        structure_len+= lens[structure[i]];
    }

    createDirectoryIfNotExists(n_s[0]);
    for (int i = 0; i < structure.length(); i+=3) {
        QString valfilepath = n_s[0]+"/"+structure[i]+".txt";
        QFile valfile(valfilepath);
        if (valfile.open(QIODevice::WriteOnly)) {
            QTextStream stream(&valfile);


            QByteArray zeros(4 * numoflines * 2, '\0');
            valfile.write(zeros);

            valfile.close();


        } else {
            QString errorMsg = QString("Не удалось открыть файл");
            QMessageBox::warning(this, "Ошибка", errorMsg);
            return;
        }

    }
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << numoflines << ' ';
        for (int i = 0; i < structure.length(); i++) {
            stream << structure[i] << ' ';
        }
        stream << '\n';
        stream.flush();

        QByteArray zeros(structure_len * numoflines, '\0');
        file.write(zeros);

        file.close();
        QMessageBox::information(this, "Success", "critical success");
        CreateWindow::closeWindow();


    } else {
        QString errorMsg = QString("Не удалось cоздать файл");
        QMessageBox::warning(this, "Ошибка", errorMsg);
        return;
    }
}

CreateWindow::CreateWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Создание файла" );
    resize(800, 200);

    nameEdit = new QTextEdit(this);
    fieldEdit = new QTextEdit(this);
    doButton = new QPushButton("Применить", this);
    closeButton = new QPushButton("Закрыть", this);
    auto *inputLabel = new QLabel( "Ввод названия", this);
    auto *outputLabel = new QLabel("Ввод полей", this);
    clearButton = new QPushButton("Очистить", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(inputLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(outputLabel);
    layout->addWidget(fieldEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addWidget(doButton);
    buttonLayout->addWidget(clearButton);

    layout->addLayout(buttonLayout);


    connect(closeButton, &QPushButton::clicked, this, &CreateWindow::closeWindow);
    connect(doButton, &QPushButton::clicked, this, &CreateWindow::createFile);
    connect(clearButton, &QPushButton::clicked, this, &CreateWindow::clearFile);
}

void CreateWindow::closeWindow()
{
    close();
}