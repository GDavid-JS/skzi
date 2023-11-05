#include "mainwindow.h"
#include <QApplication>

// Encryption* enc = dynamic_cast<Encryption*>(asym);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}