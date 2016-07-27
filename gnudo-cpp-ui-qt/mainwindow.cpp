#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskinfodialog.h"
# include "prioritylevelsdialog.hpp"
# include <vector>
# include <cstdint>
# include <string>

# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>
# include <sqlite3.h>
# include <QFileDialog>
# include <QMessageBox>
# include <QDateTime>
# include <QDir>
# include <QHeaderView>
# include <stdexcept>
# include <iostream>

using namespace gnudo::sqlite;

using std::vector;
using std::int64_t;
using std::to_string;


// TODO Installazione
// TODO Cambiare formato data
// TODO Ridurre a icona nella barra delle notifiche
// TODO Colore livelli priorità
// TODO Eliminare sqlite3_int64, essendo indipendente la gui deve usare int64_t
// TODO Toolbar
// TODO Permettere ridimensionamento colonne?
// TODO Database aperti recentemente
// TODO Ricordare ultima directory usata


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this); 
    db = NULL;
    showCompletedTask = false;


    ui->comboBox->insertItem(ColumnCombobox::TITLE, "Titolo");
    ui->comboBox->insertItem(ColumnCombobox::DESCRIPTION, "Descrizione");
    ui->comboBox->insertItem(ColumnCombobox::CREATION_TIME, "Data di creazione");
    ui->comboBox->insertItem(ColumnCombobox::MODIFICATION_TIME, "Data di modifica");
	ui->comboBox->insertItem(ColumnCombobox::PRIORITY, "Priorità");
    ui->comboBox->insertItem(ColumnCombobox::COMPLETED, "Completato");

    ui->comboBox_2->insertItem(RuleCombobox::ASCENDING, "Crescente");
    ui->comboBox_2->insertItem(RuleCombobox::DESCENDING, "Decrescente");

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onOrderByColumnChanged(int)));
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onOrderRuleChanged(int)));

	connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(showNewDbDialog()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(showOpenDbDialog()));
    connect(ui->actionAdd_task, SIGNAL(triggered()), this, SLOT(showAddTaskDialog()));
    connect(ui->actionShow_completed, SIGNAL(triggered()), this, SLOT(toggleShowCompletedTask()));
	connect(ui->actionRemove_task, SIGNAL(triggered()), this, SLOT(removeTask()));
	connect(ui->actionEdit_priority_levels, SIGNAL(triggered(bool)), this, SLOT(showPriorityLevelsDialog()));

	ui->comboBox->setCurrentIndex(ColumnCombobox::PRIORITY);
    ui->comboBox_2->setCurrentIndex(RuleCombobox::DESCENDING);

	// TODO Migliorare
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
}


void
MainWindow::showNewDbDialog()
{
	QString fileName = QFileDialog::getSaveFileName(this, "New database", QDir::homePath());

	if (fileName != "")
	{
		db = new Db(fileName.toStdString());
		refreshTableContent();
	}
}


void
MainWindow::onOrderByColumnChanged(int selection)
{
    switch(selection)
    {
        case ColumnCombobox::TITLE:
            orderByColumn = TasksManager::Order::TITLE;
            break;
        case ColumnCombobox::DESCRIPTION:
            orderByColumn = TasksManager::Order::DESCRIPTION;
            break;
        case ColumnCombobox::CREATION_TIME:
            orderByColumn = TasksManager::Order::CREATION_TIME;
            break;
        case ColumnCombobox::MODIFICATION_TIME:
            orderByColumn = TasksManager::Order::MODIFICATION_TIME;
            break;
		case ColumnCombobox::PRIORITY:
			orderByColumn = TasksManager::Order::PRIORITY;
			break;
        case ColumnCombobox::COMPLETED:
            orderByColumn = TasksManager::Order::COMPLETED;
            break;
        default:
            throw std::runtime_error("Selezionato ordinamento per colonna non prevista");
    }

    if (db != NULL) refreshTableContent();
}


void
MainWindow::onOrderRuleChanged(int rule)
{
    orderAscending = (rule == RuleCombobox::ASCENDING? true : false);
    if (db != NULL) refreshTableContent();
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

	if (db->getPriorityLevels()->getIdList().size() == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("Prima devi creare almeno un livello di priorità");
		msgBox.exec();

		PriorityLevelsDialog *pd = new PriorityLevelsDialog(0, db);
		pd->exec();

		if(db->getPriorityLevels()->getIdList().size() < 1)
			return;
	}

	TaskInfoDialog *d = new TaskInfoDialog(0, db);

    if(d->exec() == QDialog::Accepted)
        refreshTableContent();

    delete d;
}


void
MainWindow::removeTask()
{
    if (not requireOpenDb()) return;

	QModelIndexList indexes = ui->tableWidget->selectionModel()->selectedRows();

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
		db->getTasks()->remove(taskId);
    }

    refreshTableContent();
}


void
MainWindow::refreshTableContent()
{
    if (not requireOpenDb()) return;

	vector<int64_t> idList = db->getTasks()->getIdList(orderByColumn, orderAscending);
    unsigned tableRows = 0;

    tableIdAssociation.clear();
    ui->tableWidget->setRowCount(0);

    for(unsigned i = 0; i < idList.size(); i++)
    {
        Task *task = db->getTasks()->getTask(idList[i]);

        if ((not showCompletedTask) and (task->isCompleted()))
            continue;

		// TODO Cambiare colore font, a seconda dello sfondo
        ui->tableWidget->setRowCount(++tableRows);

		QTableWidgetItem *priorityCell = new QTableWidgetItem(QString( (task->getPriorityLevel()->getName() + " (" + to_string(task->getPriorityLevel()->getLevel()) + ")" ).c_str() ));
		QColor bgcolor = QColor(QString(task->getPriorityLevel()->getColor().c_str()));
		priorityCell->setBackground(QBrush(bgcolor));
		priorityCell->setTextColor((( ((bgcolor.rgb() & 0xFF0000) >> 16) + ((bgcolor.rgb() & 0x00FF00) >> 8) + ((bgcolor.rgb() & 0x0000FF)) ) > (0x0000FF * 3/2))? Qt::black : Qt::white);

		ui->tableWidget->setItem(tableRows - 1, 0, priorityCell);
		ui->tableWidget->setItem(tableRows - 1, 1, new QTableWidgetItem(QString(task->getTitle().c_str())));
		ui->tableWidget->setItem(tableRows - 1, 2, new QTableWidgetItem(QDateTime::fromTime_t(task->getCreationTime()).toString()));
		ui->tableWidget->setItem(tableRows - 1, 3, new QTableWidgetItem(QDateTime::fromTime_t(task->getModificationTime()).toString()));

		tableIdAssociation.insert(tableIdAssociation.end(), task->sqlite3pp::objects::Row::getId());

        delete task;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
	// TODO Controllare se c'è altro da eliminare
}


void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    if (not requireOpenDb()) return;

    sqlite3_int64 taskId = tableIdAssociation[index.row()];
	TaskInfoDialog *d = new TaskInfoDialog(0, db, taskId);

    if(d->exec() == QDialog::Accepted)
        refreshTableContent();

    delete d;
}

void
MainWindow::showPriorityLevelsDialog()
{
	if (not requireOpenDb()) return;

	PriorityLevelsDialog *pd = new PriorityLevelsDialog(0, db);
	pd->exec();

	refreshTableContent();
}
