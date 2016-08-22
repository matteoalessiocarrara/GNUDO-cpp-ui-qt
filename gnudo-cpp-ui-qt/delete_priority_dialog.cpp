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

# include <vector>

# include <gnudo-backend/dbdefs.hpp>
# include <gnudo-backend/gnudo.hpp>

# include "delete_priority_dialog.hpp"
# include "ui_delete_priority_dialog.h"


using std::vector;


DeletePriorityDialog::DeletePriorityDialog(QWidget *parent, Db *db, int64_t id) :
	QDialog(parent), __ui(new Ui::DeletePriorityDialog)
{
	__ui->setupUi(this);
	__db = db;
	__id = id;

	vector<int64_t> p = db->getPriorityLevels()->getIdList(dbdefs::columns::prioritylevel::priority);

	__ui->comboBox->clear();
	int comboboxIndex = 0;

	// TODO Impostare come predefinita una di livello simile
	for(const auto &tmpid: p)
	{
		if (tmpid != id)
		{
			PriorityLevel pl = db->getPriorityLevels()->getPriorityLevel(tmpid);
			__ui->comboBox->insertItem(comboboxIndex++, QString(pl.getName().c_str()));
		}

	}
}


DeletePriorityDialog::~DeletePriorityDialog()
{
	delete __ui;
}


void DeletePriorityDialog::__on_buttonBox_accepted()
{
	int64_t moveTo = __db->getPriorityLevels()->getIdList(dbdefs::columns::prioritylevel::priority)[__ui->comboBox->currentIndex()];
	__db->getPriorityLevels()->remove(__id, moveTo);
}
