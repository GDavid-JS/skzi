#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QScrollArea>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    

    // connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::pushButton);
    // connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::pushButton_8);
    // connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::pushButton_3);
    // connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::pushButton_5);
    // connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::pushButton_6);
}

// void MainWindow::pushButton()
// {
//     ui->stackedWidget->setCurrentIndex(0);
// }

// void MainWindow::pushButton_3()
// {
//     ui->stackedWidget->setCurrentIndex(1);
// }

// void MainWindow::pushButton_8()
// {
//     ui->stackedWidget->setCurrentIndex(2);
// }

// void MainWindow::pushButton_5()
// {
//     QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "Все файлы (*.*);;Текстовые файлы (*.txt);;Изображения (*.png *.jpg)");
    
//     if (!filePath.isEmpty()) {
//         ui->lineEdit_2->setText(filePath);
//     }
// }

// void MainWindow::pushButton_6()
// {
//     QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "Все файлы (*.*);;Текстовые файлы (*.txt);;Изображения (*.png *.jpg)");
    
//     if (!filePath.isEmpty()) {
//         QMessageBox::information(nullptr, "Информация", filePath);
//     }
// }

MainWindow::~MainWindow()
{
    delete ui;
}
