#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    db.setDatabaseName("database.db");
}


MainWindow::~MainWindow() {
    delete ui;
    db.close();
}

// void fill_table() {
//     add_row();
// }

// void add_row() {

// }


// void MainWindow::pushButton_5()
// {
//     QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "Все файлы (*.*);;Текстовые файлы (*.txt);;Изображения (*.png *.jpg)");
    
//     if (!filePath.isEmpty()) {
//         ui->lineEdit_2->setText(filePath);
//     }
// }

