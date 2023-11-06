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
        "  WHEN a.is_encryption = 1 THEN 'Обмена' "
        "  WHEN a.is_encryption = 0 THEN 'Подписи' "
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
        "  WHEN a.is_encryption = 1 THEN 'Обмена' "
        "  WHEN a.is_encryption = 0 THEN 'Подписи' "
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
    int id = asymmetric_table.get_value(selected_row, 0).toInt();

    std::string folder_path = QFileDialog::getExistingDirectory(this, "Выберите папку", QDir::homePath()).toStdString();

    if (!folder_path.empty()) {
        QSqlQuery query;
        query.prepare("SELECT public_key, private_key FROM asymmetric_container WHERE id = ?;");
        query.addBindValue(id);

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
            "  WHEN a.is_encryption = 1 THEN 'Обмена' "
            "  WHEN a.is_encryption = 0 THEN 'Подписи' "
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