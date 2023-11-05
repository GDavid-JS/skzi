#include "mainwindow.h"
#include <QApplication>

// Encryption* enc = dynamic_cast<Encryption*>(asym);

#include "crypto/crypto.cpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}