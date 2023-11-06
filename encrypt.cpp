void MainWindow::init_encrypt() {
    QStringList columns = {"Имя субъекта", "Алгоритм"};

    QSqlQuery encrypt_query;
    encrypt_query.prepare("SELECT * FROM ("
        "SELECT c.name, a.type FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.public_key IS NOT NULL AND a.is_encryption = 1 "
        "UNION ALL "
        "SELECT c.name, a.type FROM symmetric_container AS c "
        "JOIN symmetric AS a ON c.symmetric_id = a.id "
        ") AS keys "
        "ORDER BY keys.name, keys.type "
    );

    encrypt_table.init_table(ui->encrypt_table, columns);
    encrypt_table.fill_table(encrypt_query);
}

void MainWindow::encrypt_search(const QString &text) {
    QSqlQuery query;

    query.prepare("SELECT * FROM ("
        "SELECT c.name, a.type FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.public_key IS NOT NULL AND a.is_encryption = 1 "
        "UNION ALL "
        "SELECT c.name, a.type FROM symmetric_container AS c "
        "JOIN symmetric AS a ON c.symmetric_id = a.id "
        ") AS keys "
        "WHERE keys.name LIKE ? "
        "ORDER BY keys.name, keys.type "
    );
    
    query.addBindValue(text + "%");
    
    encrypt_table.search(query);
}

void MainWindow::encrypt_file_open() {
    QString file_path = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "Файлы (*.*)");

    if (!file_path.isEmpty()) {
        ui->encrypt_filepath_open->setText(file_path);
        ui->encrypt_filepath_close->setText(file_path + ".enc");
    }
}

void MainWindow::encrypt_file_close() {
    QString file_path = QFileDialog::getSaveFileName(nullptr, "Сохранить файл", "", "Текстовые файлы (*.txt);;Все файлы (*)");

    ui->encrypt_filepath_close->setText(file_path);
}

void MainWindow::encrypt_save() {
    int selected_row = encrypt_table.get_selected_row();
    QString name = encrypt_table.get_value(selected_row, 0);
    QString type = encrypt_table.get_value(selected_row, 1);

    QSqlQuery query;
    query.prepare("SELECT keys.key FROM ("
        "SELECT c.name, a.type, c.public_key AS key FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.public_key IS NOT NULL AND a.is_encryption = 1 "
        "UNION ALL "
        "SELECT c.name, a.type, c._key AS key FROM symmetric_container AS c "
        "JOIN symmetric AS a ON c.symmetric_id = a.id "
        ") AS keys "
        "WHERE keys.name = :name AND keys.type = :type"
    );

    query.bindValue(":name", name);
    query.bindValue(":type", type);

    if (query.exec() && query.next()) {
        std::string key = query.value(0).toString().toStdString();

        QString file_path_open = ui->encrypt_filepath_open->text();
        QString file_path_close = ui->encrypt_filepath_close->text();

        QFile file_open(file_path_open);

        if (file_open.exists()) {
            if (!file_path_close.isEmpty()) {
                if (Asymmetric* asymAlgorithm = asymmetric_register[type.toStdString()]) {
                    asymAlgorithm->set_public(key);

                    if (Encryption* algorithm = dynamic_cast<Encryption*>(algorithm)) {
                        try {
                            algorithm->encrypt(file_path_open.toStdString(), file_path_close.toStdString());
                            QMessageBox::information(this, "Success", "Файл успешно зашифрован.");
                        } catch (const std::exception& ex) {
                            QMessageBox::critical(this, "Error", QString("Ошибка при шифровании файла: ") + ex.what());
                        }
                    }
                }

                if (Symmetric* algorithm = symmetric_register[type.toStdString()]) {
                    algorithm->set_key(key);
                    
                    try {
                        algorithm->encrypt(file_path_open.toStdString(), file_path_close.toStdString());
                        QMessageBox::information(this, "Success", "Файл успешно зашифрован.");
                    } catch (const std::exception& ex) {
                        QMessageBox::critical(this, "Error", QString("Ошибка при шифровании файла: ") + ex.what());
                    }
                }

            } else {
                QMessageBox::critical(this, "Error", "Путь к файлу закрытия не указан.");
                return;
            }
        } else {
            QMessageBox::critical(this, "Error", "Не удалось открыть файл для чтения: " + file_open.errorString());
            return;
        }

    } else {
        QMessageBox::critical(this, "Error", "Ошибка при выполнении SQL-запроса: " + query.lastError().text());
        return;
    }
}
