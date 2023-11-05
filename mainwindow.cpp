#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    db.setDatabaseName("database.db");

    connect(ui->general_button, &QPushButton::clicked, this, &MainWindow::general_click);
    connect(ui->asymmetric_button, &QPushButton::clicked, this, &MainWindow::asymmetric_click);
    connect(ui->symmetric_button, &QPushButton::clicked, this, &MainWindow::symmetric_click);
    connect(ui->certificate_button, &QPushButton::clicked, this, &MainWindow::certificate_click);
    connect(ui->signature_button, &QPushButton::clicked, this, &MainWindow::signature_click);
    connect(ui->verification_button, &QPushButton::clicked, this, &MainWindow::verification_click);
    connect(ui->encrypt_button, &QPushButton::clicked, this, &MainWindow::encrypt_click);
    connect(ui->decrypt_button, &QPushButton::clicked, this, &MainWindow::decrypt_click);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть базу данных: " + db.lastError().text());
    } else {
        init_asymmetric();

        connect(ui->asymmetric_table, &QTableWidget::itemSelectionChanged, this, [=]() {
            ui->asymmetric_export_button->setEnabled(true);
            ui->asymmetric_delete_button->setEnabled(true);
        });

        connect(ui->asymmetric_search, &QLineEdit::textChanged, this, &MainWindow::asymmetric_search);
        connect(ui->asymmetric_delete_button, &QPushButton::clicked, this, &MainWindow::asymmetric_delete);
        connect(ui->asymmetric_export_button, &QPushButton::clicked, this, &MainWindow::asymmetric_export);
        connect(ui->asymmetric_create_button, &QPushButton::clicked, this, &MainWindow::asymmetric_create);



        init_symmetric();

        connect(ui->symmetric_table, &QTableWidget::itemSelectionChanged, this, [=]() {
            ui->symmetric_export_button->setEnabled(true);
            ui->symmetric_delete_button->setEnabled(true);
        });

        connect(ui->symmetric_search, &QLineEdit::textChanged, this, &MainWindow::symmetric_search);
        connect(ui->symmetric_delete_button, &QPushButton::clicked, this, &MainWindow::symmetric_delete);
        connect(ui->symmetric_export_button, &QPushButton::clicked, this, &MainWindow::symmetric_export);
        connect(ui->symmetric_create_button, &QPushButton::clicked, this, &MainWindow::symmetric_create);
    }
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

void MainWindow::certificate_click () {
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::verification_click () {
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::signature_click () {
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::encrypt_click () {
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::decrypt_click () {
    ui->stackedWidget->setCurrentIndex(7);
}



// asymmetric
void MainWindow::init_asymmetric() {
    QSqlQuery query;
    query.prepare("SELECT type FROM asymmetric;");

    if (query.exec()) {
        while (query.next()) {
            ui->asymmetric_combo_box->addItem(query.value(0).toString());
        }
    }

    QStringList columns = {"Контейнер", "Имя субъекта", "Алгоритм", "Тип ключа"};

    QSqlQuery asymmetric_query;
    asymmetric_query.prepare("SELECT "
        "c.id,"
        "c.name, "
        "a.type, "
        "CASE "
        "  WHEN a.is_encryption = 1 THEN 'Подписи' "
        "  WHEN a.is_encryption = 0 THEN 'Обмена' "
        "END "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id"
    );
    
    asymmetric_table.init_table(ui->asymmetric_table, columns);
    asymmetric_table.fill_table(asymmetric_query);
}


void MainWindow::asymmetric_search(const QString &text) {
    QSqlQuery query;

    query.prepare("SELECT "
        "  c.id, "
        "  c.name, "
        "  a.type, "
        "CASE "
        "  WHEN a.is_encryption = 1 THEN 'Подписи' "
        "  WHEN a.is_encryption = 0 THEN 'Обмена' "
        "END "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.name LIKE ?;"
    );
    
    query.addBindValue(text + "%");
    
    asymmetric_table.search(query);
}

void MainWindow::asymmetric_delete() {
    QSqlQuery query;
    query.prepare("DELETE FROM asymmetric_container WHERE id = ?;");
    asymmetric_table.delete_selected_row(query);
}

void MainWindow::asymmetric_export() {
    int selected_row = asymmetric_table.get_selected_row();
    int index = asymmetric_table.get_value(selected_row, 0).toInt();

    std::string folder_path = QFileDialog::getExistingDirectory(this, "Выберите папку", QDir::homePath()).toStdString();

    if (!folder_path.empty()) {
        QSqlQuery query;
        query.prepare("SELECT public_key, private_key FROM asymmetric_container WHERE id = ?;");
        query.addBindValue(index);

        if (query.exec() && query.next()) {
            QString public_key = query.value(0).toString();
            QString private_key = query.value(1).toString();

            std::string public_key_path = folder_path + "/public_key.pem";
            std::string private_key_path = folder_path + "/private_key.pem";

            std::ofstream public_key_file(public_key_path);
            std::ofstream private_key_file(private_key_path);

            if (public_key_file.is_open() && private_key_file.is_open()) {
                public_key_file << public_key.toStdString();
                private_key_file << private_key.toStdString();

                public_key_file.close();
                private_key_file.close();
            } else {
                // Обработка ошибки открытия файлов
            }
        } else {
            // Обработка ошибки выполнения SQL-запроса или отсутствия данных
        }
    } else {
        // Обработка отмены выбора папки
    }
}

void MainWindow::asymmetric_create() {
    QString algorithm_type = ui->asymmetric_combo_box->currentText();
    QString name = ui->asymmetric_name->text();

    AsymmetricRegister asymmetric_register;
    Asymmetric* algorithm = asymmetric_register[algorithm_type.toStdString()];
    algorithm->generate_keys();

    QString public_key = QString::fromStdString(algorithm->get_public());
    QString private_key = QString::fromStdString(algorithm->get_private());

    QSqlQuery query;
    query.prepare("INSERT INTO asymmetric_container (asymmetric_id, public_key, private_key, name) "
        "VALUES ((SELECT id FROM asymmetric WHERE type = :algorithm_type), :public_key, :private_key, :name);");

    query.bindValue(":algorithm_type", algorithm_type);
    query.bindValue(":public_key", public_key);
    query.bindValue(":private_key", private_key);
    query.bindValue(":name", name);

    if (query.exec()) {
        QSqlQuery selectQuery;

        selectQuery.prepare("SELECT "
            "  c.id, "
            "  c.name, "
            "  a.type, "
            "CASE "
            "  WHEN a.is_encryption = 1 THEN 'Подписи' "
            "  WHEN a.is_encryption = 0 THEN 'Обмена' "
            "END "
            "FROM asymmetric_container AS c "
            "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
            "WHERE c.name = :name"
        );

        selectQuery.bindValue(":name", name);

        asymmetric_table.fill_table(selectQuery);
    } else {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось выполнить SQL-запрос: " + query.lastError().text());
    }
}


MainWindow::~MainWindow() {
    delete ui;
    db.close();
}