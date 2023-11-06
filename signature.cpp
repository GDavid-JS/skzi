void MainWindow::init_signature() {
    QStringList columns = {"Контейнер", "Имя субъекта", "Алгоритм"};

    QSqlQuery signature_query;
    signature_query.prepare("SELECT "
        "c.id,"
        "c.name, "
        "a.type "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.private_key IS NOT NULL"
    );
    
    signature_table.init_table(ui->signature_table, columns);
    signature_table.fill_table(signature_query);
}

void MainWindow::signature_search(const QString &text) {
    QSqlQuery query;

    query.prepare("SELECT "
        "c.id,"
        "c.name, "
        "a.type "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.private_key IS NOT NULL AND c.name LIKE ?"
    );
    
    query.addBindValue(text + "%");
    
    signature_table.search(query);
}

void MainWindow::signature_file_open() {
    QString file_path = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "Файлы (*.*)");

    if (!file_path.isEmpty()) {
        ui->signature_filepath_open->setText(file_path);
        ui->signature_filepath_close->setText(file_path + ".sig");
    }
}

void MainWindow::signature_file_close() {
    QString file_path = QFileDialog::getSaveFileName(nullptr, "Сохранить файл", "", "Текстовые файлы (*.txt);;Все файлы (*)");

    ui->signature_filepath_close->setText(file_path);
}

void MainWindow::signature_save() {
    int selected_row = signature_table.get_selected_row();
    int id = signature_table.get_value(selected_row, 0).toInt();

    QSqlQuery query;
    query.prepare("SELECT c.private_key, a.type "
        "FROM asymmetric_container AS c "
        "JOIN asymmetric AS a ON c.asymmetric_id = a.id "
        "WHERE c.id = ?;"
    );
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        QString private_key = query.value(0).toString();
        QString type = query.value(1).toString();

        Asymmetric* algorithm = asymmetric_register[type.toStdString()];


        algorithm->set_private(private_key.toStdString());

        QString file_path_open = ui->signature_filepath_open->text();
        QString file_path_close = ui->signature_filepath_close->text();

        QFile file_open(file_path_open);

        if (file_open.exists()) {
            if (!file_path_close.isEmpty()) {
                if (Signature* signature = dynamic_cast<Signature*>(algorithm)) {
                    signature->sign_file(file_path_open.toStdString(), file_path_close.toStdString());

                    ui->signature_filepath_open->clear();
                    ui->signature_filepath_close->clear();
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