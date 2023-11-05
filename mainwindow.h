// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//base
#include <iostream>
#include <fstream>
#include <string>

// qt
#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QTableWidgetItem>

#include <QStringList>
#include <QTableWidget>
#include <QSqlQuery>
#include <QHeaderView>


//sql
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
// #include <sqlite3.h>

#include "sql_table.cpp"

// crypto
#include "crypto/crypto.cpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init_asymmetric();
    void init_symmetric();

private slots:
    void general_click();
    void asymmetric_click();
    void symmetric_click();
    void certificate_click();
    void signature_click();
    void verification_click();
    void encrypt_click();
    void decrypt_click();

    void asymmetric_search(const QString &text);
    void asymmetric_delete();
    void asymmetric_export();
    void asymmetric_create();

    void symmetric_search(const QString &text);
    void symmetric_delete();
    void symmetric_export();
    void symmetric_create();
private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    SqlTable asymmetric_table;
    SqlTable symmetric_table;
};

#endif // MAINWINDOW_H
