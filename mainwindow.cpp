#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    int rc = sqlite3_open("database.db", &db);

    std::string sql_asymmetric = "SELECT "
                 "  c.id AS 'Контейнер', "
                 "  c.name AS 'Имя субъекта', "
                 "  a.type AS 'Тип ключа' "
                 "FROM asymmetric_container AS c "
                 "JOIN asymmetric AS a ON c.asymmetric_id = a.id;";

    rc = sqlite3_exec(db, sql_asymmetric.c_str(), asymmetric_table, ui, 0);

    std::string sql_symmetric = "SELECT "
                 "  c.id AS 'Контейнер', "
                 "  c.name AS 'Имя субъекта', "
                 "  a.type AS 'Тип ключа' "
                 "FROM symmetric_container AS c "
                 "JOIN symmetric AS a ON c.symmetric_id = a.id;";

    rc = sqlite3_exec(db, sql_symmetric.c_str(), symmetric_table, ui, 0);

    connect(ui->general_button, &QPushButton::clicked, this, &MainWindow::general_click);
    connect(ui->asymmetric_button, &QPushButton::clicked, this, &MainWindow::asymmetric_click);
    connect(ui->symmetric_button, &QPushButton::clicked, this, &MainWindow::symmetric_click);
}

void MainWindow::general_click () {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::asymmetric_click () {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::symmetric_click () {
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::show_table(QTableWidget *tableWidget, int argc, char **argv, char **azColName) {
    tableWidget->setColumnCount(argc);
    tableWidget->verticalHeader()->hide();

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    for (int i = 0; i < argc; i++) {
        QTableWidgetItem *item = new QTableWidgetItem(argv[i]);
        tableWidget->setItem(row, i, item);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        QHeaderView *horizontalHeader = tableWidget->horizontalHeader();
        horizontalHeader->setSectionResizeMode(i, QHeaderView::Stretch);
        tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(azColName[i]));
    }

    QString css = "QTableView QTableCornerButton::section { font-size: 8pt; }"
                "QHeaderView::section { font-size: 8pt; }";

    tableWidget->setStyleSheet(css);
}

int MainWindow::asymmetric_table(void *data, int argc, char **argv, char **azColName) {
    Ui::MainWindow* ui = static_cast<Ui::MainWindow*>(data);

    show_table(ui->asymmetric_table, argc, argv, azColName);

    return 0;
}

int MainWindow::symmetric_table(void *data, int argc, char **argv, char **azColName) {
    Ui::MainWindow* ui = static_cast<Ui::MainWindow*>(data);

    show_table(ui->symmetric_table, argc, argv, azColName);

    return 0;
}

MainWindow::~MainWindow() {
    delete ui;
    sqlite3_close(db);
}



// void MainWindow::pushButton_5()
// {
//     QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "Все файлы (*.*);;Текстовые файлы (*.txt);;Изображения (*.png *.jpg)");
    
//     if (!filePath.isEmpty()) {
//         ui->lineEdit_2->setText(filePath);
//     }
// }

