#include "mainwindow.h"
#include "ui_mainwindow.h"

# include <vector>
# include <cstdint>
# include <string>

# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>
# include <sqlite3.h>

using namespace gnudo::sqlite;

using std::vector;
using std::int64_t;
using std::to_string;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new Db("/home/richard/testdb.db");
    db->getTasks()->add("Titolo", "Descrizione");
    refreshTableContent();
}

void
MainWindow::refreshTableContent()
{
    vector<sqlite3_int64> tasks = db->getTasks()->getIdList();

    ui->tableWidget->setRowCount(tasks.size());

    for(unsigned i = 0; i < tasks.size(); i++)
    {
        Task *tmp = db->getTasks()->getTask(tasks[i]);

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString(tmp->getTitle().c_str())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString(to_string(tmp->getCreationTime()).c_str())));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString(to_string(tmp->getModificationTime()).c_str())));

        delete tmp;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
