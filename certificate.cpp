void MainWindow::init_certificate() {
    QSqlQuery query;
    query.prepare("SELECT name FROM asymmetric_container");

    if (query.exec()) {
        while (query.next()) {
            ui->certificate_combo_box->addItem(query.value(0).toString());
        }
    }

    QStringList columns = {"Контейнер", "Имя субъекта", "Алгоритм"};

    QSqlQuery certificate_query;
    certificate_query.prepare("SELECT "
        "c.id,"
        "c.name, "
        "a.type "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.certificate IS NOT NULL"
    );
    
    certificate_table.init_table(ui->certificate_table, columns);
    certificate_table.fill_table(certificate_query);
}

void MainWindow::certificate_search(const QString &text) {
    QSqlQuery query;

    query.prepare("SELECT "
        "c.id,"
        "c.name, "
        "a.type "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.certificate IS NOT NULL AND c.name LIKE ?"
    );
    
    query.addBindValue(text + "%");
    
    certificate_table.search(query);
}

void MainWindow::certificate_delete() {
    QSqlQuery query;
    query.prepare("UPDATE asymmetric_container SET certificate = NULL WHERE id = ?");
    certificate_table.delete_selected_row(query);
}

void MainWindow::certificate_export() {
    int selected_row = certificate_table.get_selected_row();
    int id = certificate_table.get_value(selected_row, 0).toInt();

    std::string folder_path = QFileDialog::getExistingDirectory(this, "Выберите папку", QDir::homePath()).toStdString();

    if (!folder_path.empty()) {
        QSqlQuery query;
        query.prepare("SELECT certificate FROM asymmetric_container WHERE id = ?;");
        query.addBindValue(id);

        if (query.exec() && query.next()) {
            QString certificate = query.value(0).toString();

            std::string certificate_path = folder_path + "/certificate.pem";

            std::ofstream certificate_file(certificate_path);

            if (certificate_file.is_open()) {
                certificate_file << certificate.toStdString();

                certificate_file.close();
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

void MainWindow::certificate_create() {
    QString name = ui->certificate_combo_box->currentText();
    QSqlQuery query;

    query.prepare("SELECT "
        "c.id, "
        "c.public_key, "
        "c.private_key, "
        "a.type "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.name = ?"
    );
    
    query.addBindValue(name);

    if (query.exec() && query.next()) {
        int id = query.value(0).toInt();
        QString public_key = query.value(1).toString();
        QString private_key = query.value(2).toString();
        QString type = query.value(3).toString();

        Asymmetric* algorithm = asymmetric_register[type.toStdString()];

        algorithm->set_public(public_key.toStdString());
        algorithm->set_private(private_key.toStdString());
        std::string certificate = algorithm->generate_self_signed_certificate_X509(id, "exemaple name", "exemaple organization", "exemaple@exemaple.com");

        QSqlQuery queryUpdate;
        queryUpdate.prepare("UPDATE asymmetric_container SET certificate = :certificate WHERE id = :id");
        queryUpdate.bindValue(":certificate", QString::fromStdString(certificate));
        queryUpdate.bindValue(":id",id);
    
        if (queryUpdate.exec()) {
            QSqlQuery selectQuery;

            selectQuery.prepare("SELECT "
                "c.id,"
                "c.name, "
                "a.type "
                "FROM asymmetric_container AS c "
                "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
                "WHERE c.certificate IS NOT NULL AND c.name = :name"
            );

            selectQuery.bindValue(":name", name);

            certificate_table.fill_table(selectQuery);
        } else {
            QMessageBox::critical(nullptr, "Ошибка", "Не удалось выполнить SQL-запрос: " + query.lastError().text());
        }
    }
}