#include "mainwindow.h"

class SqlTable {
private:
    QTableWidget *tableWidget;

public:
    void init_table(QTableWidget *tableWidget, const QStringList &columns) {
        this->tableWidget = tableWidget;

        tableWidget->verticalHeader()->hide();

        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        QString css = "QTableView QTableCornerButton::section { font-size: 8pt; }"
                    "QHeaderView::section { font-size: 8pt; }";
        tableWidget->setStyleSheet(css);

        int column_count = columns.size();
        tableWidget->setColumnCount(column_count);

        tableWidget->setHorizontalHeaderLabels(columns);

        for (int i = 0; i < column_count; ++i) {
            tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        }
    }

    void fill_table(QSqlQuery &query) {
        if (query.exec()) {
            while (query.next()) {
                QStringList rowValues;
                for (int i = 0; i < query.record().count(); ++i) {
                    rowValues << query.value(i).toString();
                }
                add_row(rowValues);
            }
        } else {
            QMessageBox::critical(nullptr, "Ошибка", "Не удалось выполнить SQL-запрос: " + query.lastError().text());
        }
    }

    void add_row(const QStringList &values) {
        int rowCount = tableWidget->rowCount();
        tableWidget->insertRow(rowCount);

        for (int i = 0; i < values.size(); ++i) {
            QTableWidgetItem *item = new QTableWidgetItem(values[i]);
            tableWidget->setItem(rowCount, i, item);
        }
    }

    void clear() {
        while (tableWidget->rowCount() > 0) {
            tableWidget->removeRow(0);
        }
    }

    void search(QSqlQuery query) {
        clear();
        fill_table(query);
    }

    int get_selected_row() {
        QModelIndexList selectedIndexes = tableWidget->selectionModel()->selectedIndexes();
        int row = -1;

        if (!selectedIndexes.isEmpty()) {
            row = selectedIndexes.first().row();
        }

        return row;
    }

    QString get_value(int row, int column) {
        QTableWidgetItem *item = tableWidget->item(row, column);
        QString cellValue = item->text();
        
        return cellValue;
    }

    void delete_selected_row(QSqlQuery query) {
        int selected_row = get_selected_row();
        int index = get_value(selected_row, 0).toInt();

        tableWidget->removeRow(selected_row);

        query.addBindValue(index);
        query.exec();
    }  
};