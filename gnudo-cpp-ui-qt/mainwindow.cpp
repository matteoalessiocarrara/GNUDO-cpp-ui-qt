#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskinfodialog.h"

# include <vector>
# include <cstdint>
# include <string>

# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>
# include <sqlite3.h>
# include <QFileDialog>
# include <QMessageBox>

using namespace gnudo::sqlite;

using std::vector;
using std::int64_t;
using std::to_string;


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this); 
    db = NULL;
    showCompletedTask = false;

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(showOpenDbDialog()));
    connect(ui->actionAdd_task, SIGNAL(triggered()), this, SLOT(showAddTaskDialog()));
    connect(ui->actionShow_completed, SIGNAL(triggered()), this, SLOT(toggleShowCompletedTask()));

    // FIXME Questa voce deve essere attiva quando c'è una selezione sulla tabella
    connect(ui->actionRemove_task, SIGNAL(triggered()), this, SLOT(removeTask()));
}


bool
MainWindow::requireOpenDb()
{
    if (db == NULL)
    {
        QMessageBox msgBox;
        msgBox.setText("Devi prima aprire un database");
        msgBox.exec();
    }

    return db != NULL;
}


void
MainWindow::showOpenDbDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", ".", "*.db");

    if (fileName != "")
    {
        db = new Db(fileName.toStdString());
        refreshTableContent();
    }
}


// WARNING Tutti i metodi che richiedono un db aperto devono controllare che
// requireOpenDb() restituisca true


// TODO Mostrare spunta sulla voce del menu
void
MainWindow::toggleShowCompletedTask()
{
    if (not requireOpenDb()) return;
    showCompletedTask = !showCompletedTask;
    refreshTableContent();
}


void
MainWindow::showAddTaskDialog()
{
    if (not requireOpenDb()) return;

    TaskInfoDialog *d = new TaskInfoDialog;

    if(d->exec() == QDialog::Accepted)
    {
        // FIXME Aggiungere parametri creation time, modification time, completed
        db->getTasks()->add(d->title.toStdString(), d->description.toStdString());
        refreshTableContent();
    }

    delete d;
}


void
MainWindow::removeTask()
{
    if (not requireOpenDb()) return;

    QModelIndexList indexes = ui->tableWidget->selectionModel()->selection().indexes();

    for (int i = 0; i < indexes.count(); ++i)
    {
        QModelIndex index = indexes.at(i);

        sqlite3_int64 taskId = tableIdAssociation[index.row()];
        Task *t = db->getTasks()->getTask(taskId);
        db->getTasks()->remove(t); // FIXME usare direttamente id
    }

    refreshTableContent();
}


// FIXME A volte si vedono dei simboli strani nella colonna title
void
MainWindow::refreshTableContent()
{
    if (not requireOpenDb()) return;

    vector<sqlite3_int64> idList = db->getTasks()->getIdList();
    unsigned tableRows = 0;

    tableIdAssociation.clear();

    for(unsigned i = 0; i < idList.size(); i++)
    {
        Task *task = db->getTasks()->getTask(idList[i]);

        if ((not showCompletedTask) and (task->isCompleted()))
            continue;

        ui->tableWidget->setRowCount(++tableRows);

        ui->tableWidget->setItem(tableRows - 1, 0, new QTableWidgetItem(QString(task->getTitle().c_str())));
        ui->tableWidget->setItem(tableRows - 1, 1, new QTableWidgetItem(QString(to_string(task->getCreationTime()).c_str())));
        ui->tableWidget->setItem(tableRows - 1, 2, new QTableWidgetItem(QString(to_string(task->getModificationTime()).c_str())));

        tableIdAssociation.insert(tableIdAssociation.end(), task->getId());

        delete task;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    if (not requireOpenDb()) return;

    sqlite3_int64 taskId = tableIdAssociation[index.row()];
    Task *t = db->getTasks()->getTask(taskId);

    TaskInfoDialog *d = new TaskInfoDialog(0, QString(t->getTitle().c_str()), QString(t->getDescription().c_str()),
                                           t->getCreationTime(), t->getModificationTime(), t->isCompleted());

    if(d->exec() == QDialog::Accepted)
    {
        // FIXME Rallenta la gui, usare un thread
        t->setTitle(d->title.toStdString());
        t->setDescription(d->description.toStdString());
        // FIXME manca creation time
        // FIXME manca modification time
        t->setStatus(d->completed);

        refreshTableContent();
    }

    delete d;
}
