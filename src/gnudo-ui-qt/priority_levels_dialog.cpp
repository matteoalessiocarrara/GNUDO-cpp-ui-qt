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

# include <string>

# include <QMessageBox>
# include "../gnudo-backend/src/gnudo-backend/gnudo.hpp"
# include "../gnudo-backend/src/gnudo-backend/dbdefs.hpp"

# include "edit_priority_dialog.hpp"
# include "ui_priority_levels_dialog.h"
# include "delete_priority_dialog.hpp"
# include "priority_levels_dialog.hpp"


using namespace gnudo;
using std::to_string;


// XXX I pulsanti "Ok" e "Cancel" non hanno effetto
// TODO Aggiungere barra dei menu
// TODO Modificare direttamente dalla tabella
// TODO Aggiungere ordinamento tabella
// TODO Larghezza colonne


PriorityLevelsDialog::PriorityLevelsDialog(QWidget *parent, Db *db):
	QDialog(parent), __ui(new Ui::PriorityLevelsDialog)
{
	__ui->setupUi(this);
	__db = db;

	__refreshTableContent();
}


PriorityLevelsDialog::~PriorityLevelsDialog()
{
	delete __ui;
}


void
PriorityLevelsDialog::__refreshTableContent()
{

	vector<int64_t> idList = __db->getPriorityLevels()->getIdList(gnudo::dbdefs::columns::prioritylevel::priority);
	unsigned tableRows = 0;

	__tableIdAssociation.clear();
	__ui->tableWidget->setRowCount(0);

	for(unsigned i = 0; i < idList.size(); i++)
	{
		PriorityLevel pr = __db->getPriorityLevels()->getPriorityLevel(idList[i]);

		__ui->tableWidget->setRowCount(++tableRows);

		__ui->tableWidget->setItem(tableRows - 1, 0, new QTableWidgetItem(QString(to_string(pr.getLevel()).c_str())));
		__ui->tableWidget->setItem(tableRows - 1, 1, new QTableWidgetItem(QString(pr.getName().c_str())));
		__ui->tableWidget->setItem(tableRows - 1, 2, new QTableWidgetItem(QString(pr.getColor().c_str())));

		__tableIdAssociation.insert(__tableIdAssociation.end(), pr.Object<PriorityLevelsManager>::getId());
	}
}


void
PriorityLevelsDialog::on_pushButton_clicked()
{
	EditPriorityDialog e(0, __db);
	e.exec();
	__refreshTableContent(); // TODO Solo se modificato
}


void
PriorityLevelsDialog::on_pushButton_3_clicked()
{
	QModelIndexList indexes = __ui->tableWidget->selectionModel()->selection().indexes();

	if (indexes.count() == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("Nessuna riga selezionata");
		msgBox.exec();
		return;
	}

	EditPriorityDialog e(0, __db, __tableIdAssociation[indexes.at(0).row()]);
	e.exec();
	__refreshTableContent(); // TODO Solo se modificato
}


// TODO Se sono selezionati tutti e non ci sono task, non chiedere di spostare
void
PriorityLevelsDialog::on_pushButton_2_clicked()
{
	QModelIndexList indexes = __ui->tableWidget->selectionModel()->selectedRows();

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

		int64_t prId = __tableIdAssociation[index.row()];
		DeletePriorityDialog d(0, __db, prId);

		if ((__db->getPriorityLevels()->getIdList().size() == 1) and (__db->getTasks()->getIdList().size() != 0))
		{
			QMessageBox msgBox;
			msgBox.setText("Impossibile eliminare l'unico livello di priorità con dei task esistenti");
			msgBox.exec();
			return;
		}
		else if ((__db->getPriorityLevels()->getIdList().size() == 1) and (__db->getTasks()->getIdList().size() == 0))
			__db->getPriorityLevels()->sqlite3pp::objects::Table::remove(prId);
		else
			d.exec();
	}

	__refreshTableContent();
}


void
PriorityLevelsDialog::on_tableWidget_doubleClicked(const QModelIndex &index)
{
	EditPriorityDialog e(0, __db, __tableIdAssociation[index.row()]);
	e.exec();
	__refreshTableContent(); // TODO Solo se modificato
}
