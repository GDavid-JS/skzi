// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <iostream>
#include <QTableWidgetItem>
#include <sqlite3.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static void show_table(QTableWidget *tableWidget, int argc, char **argv, char **azColName);
    static int asymmetric_table(void *data, int argc, char **argv, char **azColName);
    static int symmetric_table(void *data, int argc, char **argv, char **azColName);

private slots:
    void general_click();
    void asymmetric_click();
    void symmetric_click();

private:
    Ui::MainWindow *ui;
    sqlite3* db;
};

#endif // MAINWINDOW_H
