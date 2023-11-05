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

//sql
#include <QSqlDatabase>
#include <QSqlQuery>
// #include <sqlite3.h>

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

    static int show_table(QTableWidget *tableWidget, int argc, char **argv, char **azColName);
    static int add_row(QTableWidget *tableWidget, int argc, char **argv, char **azColName);
    static int asymmetric_table(void *data, int argc, char **argv, char **azColName);
    static int asymmetric_algoritms(void *data, int argc, char **argv, char **azColName);
    static std::tuple<int, int> get_selected_row(QTableWidget *tableWidget);
    static int asymmetric_export(void *data, int argc, char **argv, char **azColName);
    // static int symmetric_table(void *data, int argc, char **argv, char **azColName);

private slots:
    void general_click();
    void asymmetric_click();
    // void symmetric_click();

    void asymmetric_search(const QString &text);

    void asymmetric_delete_click();
    void asymmetric_export_click();
    void asymmetric_create_click();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
};

#endif // MAINWINDOW_H
