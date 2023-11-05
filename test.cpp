// symmetric
void MainWindow::init_symmetric() {
    QSqlQuery query;
    query.prepare("SELECT type FROM symmetric;");

    if (query.exec()) {
        while (query.next()) {
            ui->symmetric_combo_box->addItem(query.value(0).toString());
        }
    }

    QStringList columns = {"Контейнер", "Имя субъекта", "Алгоритм"};

    QSqlQuery symmetric_query;
    symmetric_query.prepare("SELECT "
        "c.id,"
        "c.name, "
        "a.type "
        "FROM symmetric_container AS c "
        "JOIN symmetric AS a ON c.symmetric_id = a.id "
    );
    
    symmetric_table.init_table(ui->symmetric_table, columns);
    symmetric_table.fill_table(symmetric_query);
}

void MainWindow::symmetric_search(const QString &text) {
    QSqlQuery query;

    query.prepare("SELECT "
        "c.id,"
        "c.name, "
        "a.type "
        "FROM symmetric_container AS c "
        "JOIN symmetric AS a ON c.symmetric_id = a.id "
        "WHERE c.name LIKE ?"
    );
    
    query.addBindValue(text + "%");
    
    symmetric_table.search(query);
}

void MainWindow::symmetric_delete() {
    QSqlQuery query;
    query.prepare("DELETE FROM symmetric_container WHERE id = ?;");
    symmetric_table.delete_selected_row(query);
}

void MainWindow::symmetric_export() {
    int selected_row = symmetric_table.get_selected_row();
    int index = symmetric_table.get_value(selected_row, 0).toInt();

    std::string folder_path = QFileDialog::getExistingDirectory(this, "Выберите папку", QDir::homePath()).toStdString();

    if (!folder_path.empty()) {
        QSqlQuery query;
        query.prepare("SELECT _key FROM symmetric_container WHERE id = ?;");
        query.addBindValue(index);

        if (query.exec() && query.next()) {
            QString key = query.value(0).toString();

            std::string key_path = folder_path + "/key.pem";

            std::ofstream key_file(key_path);

            if (key_file.is_open()) {
                key_file << key.toStdString();

                key_file.close();
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

void MainWindow::symmetric_create() {
    QString algorithm_type = ui->symmetric_combo_box->currentText();
    QString name = ui->symmetric_name->text();

    SymmetricRegister symmetric_register;
    Symmetric* algorithm = symmetric_register[algorithm_type.toStdString()];
    algorithm->generate_key();

    QString key = QString::fromStdString(algorithm->get_key());

    QSqlQuery query;
    query.prepare("INSERT INTO symmetric_container (symmetric_id, _key, name) "
    "VALUES ((SELECT id FROM symmetric WHERE type = :algorithm_type), :key, :name);");

    query.bindValue(":algorithm_type", algorithm_type);
    query.bindValue(":key", key);
    query.bindValue(":name", name);


    std::cout << key.toStdString();


    if (query.exec()) {
        QSqlQuery selectQuery;

        selectQuery.prepare("SELECT "
            "c.id,"
            "c.name, "
            "a.type "
            "FROM symmetric_container AS c "
            "JOIN symmetric AS a ON c.symmetric_id = a.id "
            "WHERE c.name = :name"
        );

        selectQuery.bindValue(":name", name);

        symmetric_table.fill_table(selectQuery);
    } else {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось выполнить SQL-запрос: " + query.lastError().text());
    }
}