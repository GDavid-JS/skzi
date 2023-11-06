void MainWindow::init_verification() {
    QStringList columns = {"Контейнер", "Имя субъекта", "Алгоритм"};

    QSqlQuery verification_query;
    verification_query.prepare("SELECT "
        "c.id,"
        "c.name, "
        "a.type "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.public_key IS NOT NULL"
    );
    
    verification_table.init_table(ui->verification_table, columns);
    verification_table.fill_table(verification_query);
}

void MainWindow::verification_search(const QString &text) {
    QSqlQuery query;

    query.prepare("SELECT "
        "c.id,"
        "c.name, "
        "a.type "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.public_key IS NOT NULL AND c.name LIKE ?"
    );
    
    query.addBindValue(text + "%");
    
    verification_table.search(query);
}

void MainWindow::verification_file_open() {
    QString file_path = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "Файлы (*.*)");

    ui->verification_filepath_open->setText(file_path);
}

void MainWindow::verification_file_close() {
    QString file_path = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "Файлы (*.*)");

    ui->verification_filepath_close->setText(file_path);
}

void MainWindow::verification_test() {
    int selected_row = verification_table.get_selected_row();
    int id = verification_table.get_value(selected_row, 0).toInt();

    QSqlQuery query;
    query.prepare("SELECT c.public_key, a.type "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.id = ?;"
    );
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        QString public_key = query.value(0).toString();
        QString type = query.value(1).toString();

        Asymmetric* algorithm = asymmetric_register[type.toStdString()];


        algorithm->set_public(public_key.toStdString());

        QString file_path_open = ui->verification_filepath_open->text();
        QString file_path_close = ui->verification_filepath_close->text();

        QFile file_open(file_path_open);

        if (file_open.exists()) {
            if (!file_path_close.isEmpty()) {
                if (Signature* signature = dynamic_cast<Signature*>(algorithm)) {
                    if (signature->verify_signature(file_path_open.toStdString(), file_path_close.toStdString())) {
                        QMessageBox::information(nullptr, "Подпись верифицирована", "Подпись файла успешно верифицирована.");
                    } else {
                        QMessageBox::warning(nullptr, "Подпись не верифицирована", "Подпись файла не верифицирована. Файл может быть изменен.");
                    }
                }
            } else {
                // Обработка ситуации, когда file_path_close пустая
                QMessageBox::critical(this, "Ошибка", "Путь к закрытому файлу пуст.");
            }
        } else {
            // Обработка ситуации, когда файл file_path_open не существует
            QMessageBox::critical(this, "Ошибка", "Файл не существует: " + file_path_open);
        }


    } else {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось выполнить SQL-запрос: " + query.lastError().text());
    }
}