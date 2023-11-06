#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "asymmetric.cpp"
#include "symmetric.cpp"
#include "certificate.cpp"
#include "signature.cpp"
#include "verification.cpp"
#include "encrypt.cpp"
#include "decrypt.cpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    db.setDatabaseName("database.db");

    connect(ui->general_button, &QPushButton::clicked, this, &MainWindow::general);
    connect(ui->asymmetric_button, &QPushButton::clicked, this, &MainWindow::asymmetric);
    connect(ui->symmetric_button, &QPushButton::clicked, this, &MainWindow::symmetric);
    connect(ui->certificate_button, &QPushButton::clicked, this, &MainWindow::certificate);
    connect(ui->signature_button, &QPushButton::clicked, this, &MainWindow::signature);
    connect(ui->verification_button, &QPushButton::clicked, this, &MainWindow::verification);
    connect(ui->encrypt_button, &QPushButton::clicked, this, &MainWindow::encrypt);
    connect(ui->decrypt_button, &QPushButton::clicked, this, &MainWindow::decrypt);

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



        init_certificate();

        connect(ui->certificate_table, &QTableWidget::itemSelectionChanged, this, [=]() {
            ui->certificate_export_button->setEnabled(true);
            ui->certificate_delete_button->setEnabled(true);
        });

        connect(ui->certificate_search, &QLineEdit::textChanged, this, &MainWindow::certificate_search);
        connect(ui->certificate_delete_button, &QPushButton::clicked, this, &MainWindow::certificate_delete);
        connect(ui->certificate_export_button, &QPushButton::clicked, this, &MainWindow::certificate_export);
        connect(ui->certificate_create_button, &QPushButton::clicked, this, &MainWindow::certificate_create);


        init_signature();

        connect(ui->signature_table, &QTableWidget::itemSelectionChanged, this, [=]() {
            ui->signature_save_button->setEnabled(true);
        });

        connect(ui->signature_search, &QLineEdit::textChanged, this, &MainWindow::signature_search);
        connect(ui->signature_file_open_button, &QPushButton::clicked, this, &MainWindow::signature_file_open);
        connect(ui->signature_file_close_button, &QPushButton::clicked, this, &MainWindow::signature_file_close);
        connect(ui->signature_save_button, &QPushButton::clicked, this, &MainWindow::signature_save);


        init_verification();

        connect(ui->verification_table, &QTableWidget::itemSelectionChanged, this, [=]() {
            ui->verification_test_button->setEnabled(true);
        });

        connect(ui->verification_search, &QLineEdit::textChanged, this, &MainWindow::verification_search);
        connect(ui->verification_file_open_button, &QPushButton::clicked, this, &MainWindow::verification_file_open);
        connect(ui->verification_file_close_button, &QPushButton::clicked, this, &MainWindow::verification_file_close);
        connect(ui->verification_test_button, &QPushButton::clicked, this, &MainWindow::verification_test);



        init_encrypt();

        connect(ui->encrypt_table, &QTableWidget::itemSelectionChanged, this, [=]() {
            ui->encrypt_save_button->setEnabled(true);
        });

        connect(ui->encrypt_search, &QLineEdit::textChanged, this, &MainWindow::encrypt_search);
        connect(ui->encrypt_file_open_button, &QPushButton::clicked, this, &MainWindow::encrypt_file_open);
        connect(ui->encrypt_file_close_button, &QPushButton::clicked, this, &MainWindow::encrypt_file_close);
        connect(ui->encrypt_save_button, &QPushButton::clicked, this, &MainWindow::encrypt_save);


        init_decrypt();

        connect(ui->decrypt_table, &QTableWidget::itemSelectionChanged, this, [=]() {
            ui->decrypt_save_button->setEnabled(true);
        });

        connect(ui->decrypt_search, &QLineEdit::textChanged, this, &MainWindow::decrypt_search);
        connect(ui->decrypt_file_open_button, &QPushButton::clicked, this, &MainWindow::decrypt_file_open);
        connect(ui->decrypt_file_close_button, &QPushButton::clicked, this, &MainWindow::decrypt_file_close);
        connect(ui->decrypt_save_button, &QPushButton::clicked, this, &MainWindow::decrypt_save);
    }
}

MainWindow::~MainWindow() {
    delete ui;
    db.close();
}

void MainWindow::general () {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::asymmetric () {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::symmetric () {
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::certificate () {
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::signature () {
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::verification () {
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::encrypt () {
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::decrypt () {
    ui->stackedWidget->setCurrentIndex(7);
}