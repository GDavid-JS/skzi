void MainWindow::init_decrypt() {
    QStringList columns = {"Имя субъекта", "Алгоритм"};

    QSqlQuery decrypt_query;
    decrypt_query.prepare("SELECT * FROM ("
        "SELECT c.name, a.type FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.private_key IS NOT NULL AND a.is_encryption = 1 "
        "UNION ALL "
        "SELECT c.name, a.type FROM symmetric_container AS c "
        "JOIN symmetric AS a ON c.symmetric_id = a.id "
        ") AS keys "
        "ORDER BY keys.name, keys.type "
    );

    decrypt_table.init_table(ui->decrypt_table, columns);
    decrypt_table.fill_table(decrypt_query);
}

void MainWindow::decrypt_search(const QString &text) {
    QSqlQuery query;

    query.prepare("SELECT * FROM ("
        "SELECT c.name, a.type FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.private_key IS NOT NULL AND a.is_encryption = 1 "
        "UNION ALL "
        "SELECT c.name, a.type FROM symmetric_container AS c "
        "JOIN symmetric AS a ON c.symmetric_id = a.id "
        ") AS keys "
        "WHERE keys.name LIKE ? "
        "ORDER BY keys.name, keys.type "
    );
    
    query.addBindValue(text + "%");
    
    decrypt_table.search(query);
}

void MainWindow::decrypt_file_open() {
    QString file_path = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "Файлы (*.*)");

    if (!file_path.isEmpty()) {
        ui->decrypt_filepath_open->setText(file_path);
        if (file_path.endsWith(".enc")) {
            file_path.chop(4);
        }
        
        ui->decrypt_filepath_close->setText(file_path);
    }
}

void MainWindow::decrypt_file_close() {
    QString file_path = QFileDialog::getSaveFileName(nullptr, "Сохранить файл", "", "Текстовые файлы (*.txt);;Все файлы (*)");

    ui->decrypt_filepath_close->setText(file_path);
}

void MainWindow::decrypt_save() {
    int selected_row = decrypt_table.get_selected_row();
    QString name = decrypt_table.get_value(selected_row, 0);
    QString type = decrypt_table.get_value(selected_row, 1);

    QSqlQuery query;
    query.prepare("SELECT keys.key FROM ("
        "SELECT c.name, a.type, c.private_key AS key FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.private_key IS NOT NULL AND a.is_encryption = 1 "
        "UNION ALL "
        "SELECT c.name, a.type, c._key AS key FROM symmetric_container AS c "
        "JOIN symmetric AS a ON c.symmetric_id = a.id "
        ") AS keys "
        "WHERE keys.name = :name AND keys.type = :type "
    );

    query.bindValue(":name", name);
    query.bindValue(":type", type);

    if (query.exec() && query.next()) {
        std::string key = query.value(0).toString().toStdString();

        if (key.empty()) {
            std::cout << "ключ пуст";
        }

        QString file_path_open = ui->decrypt_filepath_open->text();
        QString file_path_close = ui->decrypt_filepath_close->text();

        QFile file_open(file_path_open);

        if (file_open.exists()) {
            if (!file_path_close.isEmpty()) {

                if (Asymmetric* algorithm = asymmetric_register[type.toStdString()]) {
                    algorithm->set_private(key);

                    if (Encryption* encryption = dynamic_cast<Encryption*>(algorithm)) {
                        encryption->decrypt(file_path_open.toStdString(), file_path_close.toStdString());
                    }
                }

                if (Symmetric* algorithm = symmetric_register[type.toStdString()]) {
                    algorithm->set_key(key);
                    algorithm->encrypt(file_path_open.toStdString(), file_path_close.toStdString());
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