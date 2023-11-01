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


    std::string sql_asymmetric_algoritms = "SELECT type FROM asymmetric;";

    rc = sqlite3_exec(db, sql_asymmetric_algoritms.c_str(), asymmetric_algoritms, ui->asymmetric_combo_box, 0);

    // std::string sql_symmetric = "SELECT "
    //              "  c.id AS 'Контейнер', "
    //              "  c.name AS 'Имя субъекта', "
    //              "  a.type AS 'Тип ключа' "
    //              "FROM symmetric_container AS c "
    //              "JOIN symmetric AS a ON c.symmetric_id = a.id;";

    // rc = sqlite3_exec(db, sql_symmetric.c_str(), symmetric_table, ui, 0);

    connect(ui->general_button, &QPushButton::clicked, this, &MainWindow::general_click);
    connect(ui->asymmetric_button, &QPushButton::clicked, this, &MainWindow::asymmetric_click);
    // connect(ui->asymmetric_set_button, &QPushButton::clicked, this, &MainWindow::asymmetric_set_click);
    // connect(ui->symmetric_button, &QPushButton::clicked, this, &MainWindow::symmetric_click);
    connect(ui->asymmetric_search, &QLineEdit::textChanged, this, &MainWindow::asymmetric_search);

    connect(ui->asymmetric_table, &QTableWidget::itemSelectionChanged, this, [=]() {
        ui->asymmetric_export_button->setEnabled(true);
        ui->asymmetric_delete_button->setEnabled(true);
    });
    
    connect(ui->asymmetric_delete_button, &QPushButton::clicked, this, &MainWindow::asymmetric_delete_click);
    connect(ui->asymmetric_export_button, &QPushButton::clicked, this, &MainWindow::asymmetric_export_click);

    connect(ui->asymmetric_create_button, &QPushButton::clicked, this, &MainWindow::asymmetric_create_click);


}

int MainWindow::show_table(QTableWidget *tableWidget, int argc, char **argv, char **azColName) {
    tableWidget->setColumnCount(argc);
    tableWidget->verticalHeader()->hide();


    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    add_row(tableWidget, argc, argv, azColName);


    QString css = "QTableView QTableCornerButton::section { font-size: 8pt; }"
                "QHeaderView::section { font-size: 8pt; }";

    tableWidget->setStyleSheet(css);

    return 0;
}

int MainWindow::add_row(QTableWidget *tableWidget, int argc, char **argv, char **azColName) {
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

    return 0;
}

std::tuple<int, int> MainWindow::get_selected_row(QTableWidget *tableWidget) {
    QModelIndexList selectedIndexes = tableWidget->selectionModel()->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        int row = selectedIndexes.first().row();
        int col = 0;
        QTableWidgetItem *item = tableWidget->item(row, col);
        return std::make_tuple(item->text().toInt(), row);
    }

    return std::make_tuple(-1, -1);
}



void MainWindow::general_click () {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::asymmetric_click () {
    ui->stackedWidget->setCurrentIndex(1);
}

// void MainWindow::symmetric_click () {
//     ui->stackedWidget->setCurrentIndex(2);
// }


int MainWindow::asymmetric_table(void *data, int argc, char **argv, char **azColName) {
    Ui::MainWindow* ui = static_cast<Ui::MainWindow*>(data);

    show_table(ui->asymmetric_table, argc, argv, azColName);

    return 0;
}

// int MainWindow::symmetric_table(void *data, int argc, char **argv, char **azColName) {
//     Ui::MainWindow* ui = static_cast<Ui::MainWindow*>(data);

//     show_table(ui->symmetric_table, argc, argv, azColName);

//     return 0;
// }

void MainWindow::asymmetric_search(const QString &text) {
    const char* sql_asymmetric = "SELECT "
                 "  c.id AS 'Контейнер', "
                 "  c.name AS 'Имя субъекта', "
                 "  a.type AS 'Тип ключа' "
                 "FROM asymmetric_container AS c "
                 "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
                 "WHERE c.name LIKE '%q%%';";

    char *sql = sqlite3_mprintf(sql_asymmetric, text.toUtf8().constData());

    while (ui->asymmetric_table->rowCount() > 0) {
        ui->asymmetric_table->removeRow(0);
    }

    int rc = sqlite3_exec(db, sql, asymmetric_table, ui, 0);
}

void MainWindow::asymmetric_delete_click() {
    std::tuple<int, int> result = get_selected_row(ui->asymmetric_table);

    int index = std::get<0>(result);
    int row = std::get<1>(result);

    if (index == -1 && row == -1) {

    } else {
        ui->asymmetric_table->removeRow(row);
        std::string sql_delete = "DELETE FROM asymmetric_container WHERE id = " + std::to_string(index) + ";";

        int rc = sqlite3_exec(db, sql_delete.c_str(), nullptr, nullptr, nullptr);
    }
}

void MainWindow::asymmetric_export_click() {
    std::tuple<int, int> result = get_selected_row(ui->asymmetric_table);

    int index = std::get<0>(result);
    int row = std::get<1>(result);

    if (index == -1 && row == -1) {

    } else {
        std::string folder_path = QFileDialog::getExistingDirectory(this, "Выберите папку", QDir::homePath()).toStdString();
 
        if (!folder_path.empty()) {
            std::string sql = "SELECT public_key, private_key FROM asymmetric_container WHERE id = " + std::to_string(index) + ";";

            int rc = sqlite3_exec(db, sql.c_str(), asymmetric_export, &folder_path, nullptr);
        } else {
            
        }
    }
}

int MainWindow::asymmetric_export(void* userData, int argc, char** argv, char**) {
    std::string* folderPath = static_cast<std::string*>(userData);

    std::string public_key = argv[0];
    std::string private_key = argv[1];

    std::string public_key_path = *folderPath + "/public_key.txt";
    std::string private_key_path = *folderPath + "/private_key.txt";

    std::ofstream public_key_file(public_key_path);
    std::ofstream private_key_file(private_key_path);

    if (public_key_file.is_open() && private_key_file.is_open()) {
        public_key_file << public_key;
        private_key_file << private_key;

        public_key_file.close();
        private_key_file.close();
    } else {

    }

    return 0;
}

int MainWindow::asymmetric_algoritms(void *data, int argc, char **argv, char **azColName) {
    QComboBox* comboBox = static_cast<QComboBox*>(data);

    for (int i = 0; i < argc; i++) {
        comboBox->addItem(argv[i]);
    }

    return 0;
}

void MainWindow::asymmetric_create_click() {
    std::string algorithm_type = ui->asymmetric_combo_box->currentText().toStdString();
    std::string name = ui->asymmetric_name->text().toStdString();

    AsymmetricRegister asymmetric_register;
    Asymmetric* algorithm = asymmetric_register[algorithm_type];
    algorithm->generate_keys();

    std::string public_key = algorithm->get_public();
    std::string private_key = algorithm->get_private();

    std::string sql = "INSERT INTO asymmetric_container (asymmetric_id, public_key, private_key, name) "
                 "VALUES ((SELECT id FROM asymmetric WHERE type = '" + algorithm_type + "'), '"
                 + public_key + "', '" + private_key + "', '" + name + "');";

    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);

    if (rc == SQLITE_OK) {
        std::string sql = "SELECT "
                 "  c.id AS 'Контейнер', "
                 "  c.name AS 'Имя субъекта', "
                 "  a.type AS 'Тип ключа' "
                 "FROM asymmetric_container AS c "
                 "JOIN asymmetric AS a ON c.asymmetric_id = a.id;";


        while (ui->asymmetric_table->rowCount() > 0) {
            ui->asymmetric_table->removeRow(0);
        }

        int rc = sqlite3_exec(db, sql.c_str(), asymmetric_table, ui, 0);
    } else {

    }
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

