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
#include <QFile>
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
    void init_certificate();
    void init_signature();
    void init_verification();
    void init_encrypt();
    void init_decrypt();

private slots:
    void general();
    void asymmetric();
    void symmetric();
    void certificate();
    void signature();
    void verification();
    void encrypt();
    void decrypt();

    void asymmetric_search(const QString &text);
    void asymmetric_delete();
    void asymmetric_export();
    void asymmetric_create();

    void symmetric_search(const QString &text);
    void symmetric_delete();
    void symmetric_export();
    void symmetric_create();

    void certificate_search(const QString &text);
    void certificate_delete();
    void certificate_export();
    void certificate_create();

    void signature_search(const QString &text);
    void signature_file_open();
    void signature_file_close();
    void signature_save();

    void verification_search(const QString &text);
    void verification_file_open();
    void verification_file_close();
    void verification_test();

    void encrypt_search(const QString &text);
    void encrypt_file_open();
    void encrypt_file_close();
    void encrypt_save();

    void decrypt_search(const QString &text);
    void decrypt_file_open();
    void decrypt_file_close();
    void decrypt_save();
private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    SqlTable asymmetric_table;
    SqlTable symmetric_table;
    SqlTable certificate_table;
    SqlTable signature_table;
    SqlTable verification_table;
    SqlTable encrypt_table;
    SqlTable decrypt_table;

    AsymmetricRegister asymmetric_register;
    SymmetricRegister symmetric_register;
};

#endif // MAINWINDOW_H
