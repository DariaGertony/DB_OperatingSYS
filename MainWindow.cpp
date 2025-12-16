//
// Created by Admin on 29.10.2025.
//

#include "MainWindow.h"
#include "FileViewer.h"


void MainWindow::createDB() {
    auto *viewer = new CreateWindow();
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}

void MainWindow::deleteDB() {
    auto *viewer = new DeleteWindow();
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();

}
 void MainWindow::zipDB() {
    auto *viewer = new zipWindow();
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();

}


void MainWindow::showRules() {

    auto *viewer = new FileViewer();
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();

}

void MainWindow::openDB() {

    auto *viewer = new OpenWindow();
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}

void MainWindow::lineDB() {
    auto *viewer = new LineDeletion();
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}


void MainWindow::addtoDB() {

    auto *viewer = new addWindow();
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}
void MainWindow::editDB() {

    auto *viewer = new editWindow();
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}
