#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>

using namespace gnudo::sqlite;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void refreshTableContent();

private:
    Ui::MainWindow *ui;
    Db *db;
};

#endif // MAINWINDOW_H
