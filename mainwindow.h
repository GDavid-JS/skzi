// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // void pushButton();
    // void pushButton_3();
    // void pushButton_5();
    // void pushButton_6();
    // void pushButton_8();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
