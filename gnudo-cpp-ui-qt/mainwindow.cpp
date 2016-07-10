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
# include <QDateTime>
# include <QDir>

using namespace gnudo::sqlite;

using std::vector;
using std::int64_t;
using std::to_string;

// TODO Migliorare larghezza automatica colonne
// TODO Permettere ordinamento colonne
// TODO Toolbar
// TODO Database aperti recentemente
// TODO Ricordare ultima directory usata


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this); 
    db = NULL;
    showCompletedTask = false;

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(showOpenDbDialog()));
    connect(ui->actionAdd_task, SIGNAL(triggered()), this, SLOT(showAddTaskDialog()));
    connect(ui->actionShow_completed, SIGNAL(triggered()), this, SLOT(toggleShowCompletedTask()));
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
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "*.db");

    if (fileName != "")
    {
        db = new Db(fileName.toStdString());
        refreshTableContent();
    }
}


// WARNING Tutti i metodi che richiedono un db aperto devono controllare che
// requireOpenDb() restituisca true



void
MainWindow::toggleShowCompletedTask()
{
    if (not requireOpenDb()) return;
    showCompletedTask = !showCompletedTask;

    // TODO Usare icona
    if(showCompletedTask)
        ui->actionShow_completed->setText("Show completed: v");
    else
        ui->actionShow_completed->setText("Show completed: x");

    refreshTableContent();
}


void
MainWindow::showAddTaskDialog()
{
    if (not requireOpenDb()) return;

    TaskInfoDialog *d = new TaskInfoDialog;

    if(d->exec() == QDialog::Accepted)
    {
        db->getTasks()->add(d->title.toStdString(), d->description.toStdString(),
                            d->creationTime, d->modificationTime, d->completed);
        refreshTableContent();
    }

    delete d;
}


void
MainWindow::removeTask()
{
    if (not requireOpenDb()) return;

    QModelIndexList indexes = ui->tableWidget->selectionModel()->selection().indexes();

    // TODO Questa è una cosa provvisoria, sarebbe meglio disabilitare la voce del menu
    // se non ci sono righe selezionate
    if (indexes.count() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Nessuna riga selezionata");
        msgBox.exec();
        return;
    }


    for (int i = 0; i < indexes.count(); ++i)
    {
        QModelIndex index = indexes.at(i);

        sqlite3_int64 taskId = tableIdAssociation[index.row()];
        Task *t = db->getTasks()->getTask(taskId);
        db->getTasks()->remove(t); // FIXME usare direttamente id
    }

    refreshTableContent();
}


void
MainWindow::refreshTableContent()
{
    if (not requireOpenDb()) return;

    vector<sqlite3_int64> idList = db->getTasks()->getIdList();
    unsigned tableRows = 0;

    tableIdAssociation.clear();
    ui->tableWidget->setRowCount(0);

    for(unsigned i = 0; i < idList.size(); i++)
    {
        Task *task = db->getTasks()->getTask(idList[i]);

        if ((not showCompletedTask) and (task->isCompleted()))
            continue;

        ui->tableWidget->setRowCount(++tableRows);

        ui->tableWidget->setItem(tableRows - 1, 0, new QTableWidgetItem(QString(task->getTitle().c_str())));
        ui->tableWidget->setItem(tableRows - 1, 1, new QTableWidgetItem(QDateTime::fromTime_t(task->getCreationTime()).toString()));
        ui->tableWidget->setItem(tableRows - 1, 2, new QTableWidgetItem(QDateTime::fromTime_t(task->getModificationTime()).toString()));

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

    QString title = t->getTitle().c_str(), description = t->getDescription().c_str();
    time_t creationTime = t->getCreationTime(), modificationTime = t->getModificationTime();
    bool completed = t->isCompleted();

    TaskInfoDialog *d = new TaskInfoDialog(0, title, description, creationTime, modificationTime, completed);

    if(d->exec() == QDialog::Accepted)
    {
        if(d->title != title) t->setTitle(d->title.toStdString());
        if(d->description != description) t->setDescription(d->description.toStdString());
        if(d->creationTime != creationTime) t->setCreationTime(d->creationTime);
        if(d->modificationTime != modificationTime) t->setModificationTime(d->modificationTime);
        if(d->completed != completed) t->setStatus(d->completed);

        refreshTableContent();
    }

    delete d;
}
