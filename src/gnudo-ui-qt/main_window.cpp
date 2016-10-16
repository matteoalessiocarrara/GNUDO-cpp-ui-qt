/*
 * Copyright 2016 Matteo Alessio Carrara <sw.matteoac@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

# include <stdexcept>
# include <iostream>
# include <cstdint>
# include <utility>
# include <vector>
# include <string>

# include <QFileDialog>
# include <QMessageBox>
# include <QHeaderView>
# include <QDateTime>
# include <QDir>
# include "../gnudo-backend/src/dbdefs.hpp"
# include "../gnudo-backend/src/gnudo.hpp"

# include "main_window.hpp"
# include "ui_main_window.h"
# include "task_info_dialog.hpp"
# include "priority_levels_dialog.hpp"


using namespace gnudo::managers;
using namespace gnudo;

using std::pair;
using std::vector;
using std::string;
using std::int64_t;
using std::to_string;


// FIXME L'ordinamento per data non funziona
// TODO Installazione
// TODO Ridurre a icona nella barra delle notifiche
// TODO Toolbar
// TODO Permettere ridimensionamento colonne?
// TODO Database aperti recentemente
// TODO Ricordare ultima directory usata


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), __ui(new Ui::MainWindow)
{
	__ui->setupUi(this);
	__db = NULL;
	__showCompletedTask = false;

	__columnComboboxOptions.insert(__columnComboboxOptions.end(), pair<string, string>("Titolo", dbdefs::columns::task::title));
	__columnComboboxOptions.insert(__columnComboboxOptions.end(), pair<string, string>("Descrizione", dbdefs::columns::task::description));
	__columnComboboxOptions.insert(__columnComboboxOptions.end(), pair<string, string>("Data di creazione", dbdefs::columns::task::creationTime));
	__columnComboboxOptions.insert(__columnComboboxOptions.end(), pair<string, string>("Data di modifica", dbdefs::columns::task::modificationTime));
	__columnComboboxOptions.insert(__columnComboboxOptions.end(), pair<string, string>("Priorità", dbdefs::columns::task::priority));
	__columnComboboxOptions.insert(__columnComboboxOptions.end(), pair<string, string>("Completato", dbdefs::columns::task::completed));


	for(unsigned i = 0; i < __columnComboboxOptions.size(); i++)
		__ui->columnComboBox->insertItem(i, QString(__columnComboboxOptions[i].first.c_str()));

	__ui->ruleComboBox->insertItem(0, "Crescente");
	__ui->ruleComboBox->insertItem(1, "Decrescente");


	connect(__ui->columnComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(__onOrderByColumnChanged(int)));
	connect(__ui->ruleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(__onOrderRuleChanged(int)));

	connect(__ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(__showNewDbDialog()));
	connect(__ui->actionOpen, SIGNAL(triggered()), this, SLOT(__showOpenDbDialog()));
	connect(__ui->actionAdd_task, SIGNAL(triggered()), this, SLOT(__showAddTaskDialog()));
	connect(__ui->actionShow_completed, SIGNAL(triggered()), this, SLOT(__toggleShowCompletedTask()));
	connect(__ui->actionRemove_task, SIGNAL(triggered()), this, SLOT(__removeTask()));
	connect(__ui->actionEdit_priority_levels, SIGNAL(triggered(bool)), this, SLOT(__showPriorityLevelsDialog()));

	__ui->columnComboBox->setCurrentIndex(4);
	__ui->ruleComboBox->setCurrentIndex(1);

	// TODO Migliorare, l'utente adesso non può modificare la larghezza
	__ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	__ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	__ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	__ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
}


// WARNING Tutti i metodi che richiedono un db aperto devono controllare che
// requireOpenDb() restituisca true


bool
MainWindow::__requireOpenDb()
{
	if (__db == NULL)
	{
		QMessageBox msgBox;
		msgBox.setText("Devi prima aprire un database");
		msgBox.exec();
	}

	return __db != NULL;
}


void
MainWindow::__showNewDbDialog()
{
	QString fileName = QFileDialog::getSaveFileName(this, "New database", QDir::homePath(), "*.db");

	if (fileName != "")
	{
		// TODO Gestire se esiste già
		__db = new Db(fileName.toStdString().c_str());
		__refreshTableContent();
	}
}


void
MainWindow::__showOpenDbDialog()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), "*.db");

	if (fileName != "")
	{
		__db = new Db(fileName.toStdString().c_str());
		__refreshTableContent();
	}
}


void
MainWindow::__onOrderByColumnChanged(int selection)
{
	__orderByColumn = __columnComboboxOptions[selection].second;
	if (__db != NULL) __refreshTableContent();
}


void
MainWindow::__onOrderRuleChanged(int rule)
{
	__orderAscending = (rule == 0? true : false);
	if (__db != NULL) __refreshTableContent();
}


void
MainWindow::__toggleShowCompletedTask()
{
	__showCompletedTask = !__showCompletedTask;

	// TODO Usare icona
	__ui->actionShow_completed->setText(QString("Show completed: ") +
										(__showCompletedTask? QString("v") : QString("x")));

	if (__db != NULL) __refreshTableContent();
}


void
MainWindow::__showAddTaskDialog()
{
    if (not __requireOpenDb()) return;

	if (__db->getPriorityLevels()->getIdList().size() == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("Prima devi creare almeno un livello di priorità");
		msgBox.exec();

		PriorityLevelsDialog pd(0, __db);
		pd.exec();

		// Se ha annullato la creazione
		if(__db->getPriorityLevels()->getIdList().size() == 0)
			return;
	}

	TaskInfoDialog d(0, __db);

	if(d.exec() == QDialog::Accepted)
        __refreshTableContent();
}


void
MainWindow::__removeTask()
{
    if (not __requireOpenDb()) return;

	QModelIndexList indexes = __ui->tableWidget->selectionModel()->selectedRows();

	// XXX Questa è una cosa provvisoria, sarebbe meglio disabilitare la voce del menu
    // se non ci sono righe selezionate
    if (indexes.count() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Nessuna riga selezionata");
        msgBox.exec();
        return;
    }

    for (int i = 0; i < indexes.count(); ++i)
		__db->getTasks()->remove(__tableIdAssociation[indexes.at(i).row()]);

    __refreshTableContent();
}


void
MainWindow::__refreshTableContent()
{
    if (not __requireOpenDb()) return;

	vector<int64_t> idList = __db->getTasks()->getIdList(__orderByColumn, __orderAscending);
    unsigned tableRows = 0;

	__tableIdAssociation.clear();
	__ui->tableWidget->setRowCount(tableRows);

    for(unsigned i = 0; i < idList.size(); i++)
    {
		Task task = __db->getTasks()->getTask(idList[i]);

		if ((not __showCompletedTask) and (task.isCompleted()))
            continue;

		__ui->tableWidget->setRowCount(++tableRows);


		QString priorityString((task.getPriorityLevel().getName() + " (" + to_string(task.getPriorityLevel().getLevel()) + ")").c_str());
		QTableWidgetItem *priorityCell = new QTableWidgetItem(priorityString);
		QColor bgcolor = QColor(QString(task.getPriorityLevel().getColor().c_str()));
		priorityCell->setBackground(QBrush(bgcolor));
		priorityCell->setTextColor((( ((bgcolor.rgb() & 0xFF0000) >> 16) + ((bgcolor.rgb() & 0x00FF00) >> 8) + ((bgcolor.rgb() & 0x0000FF)) ) > (0x0000FF * 3/2))? Qt::black : Qt::white);


		__ui->tableWidget->setItem(tableRows - 1, 0, priorityCell);
		__ui->tableWidget->setItem(tableRows - 1, 1, new QTableWidgetItem(QString(task.getTitle().c_str())));
		__ui->tableWidget->setItem(tableRows - 1, 2, new QTableWidgetItem(QDateTime::fromTime_t(task.getCreationTime()).toString( QString("d/M/yyyy hh:mm:ss"))));
		__ui->tableWidget->setItem(tableRows - 1, 3, new QTableWidgetItem(QDateTime::fromTime_t(task.getModificationTime()).toString( QString("d/M/yyyy hh:mm:ss"))));


		__tableIdAssociation.insert(__tableIdAssociation.end(), task.Object<TasksManager>::getId());
    }
}


MainWindow::~MainWindow()
{
	delete __ui;
	delete __db;
}


void
MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    if (not __requireOpenDb()) return;

	int64_t taskId = __tableIdAssociation[index.row()];
	TaskInfoDialog d(0, __db, taskId);

	if(d.exec() == QDialog::Accepted)
        __refreshTableContent();
}


void
MainWindow::__showPriorityLevelsDialog()
{
	if (not __requireOpenDb()) return;

	PriorityLevelsDialog pd(0, __db);

	pd.exec();
	__refreshTableContent();
}
