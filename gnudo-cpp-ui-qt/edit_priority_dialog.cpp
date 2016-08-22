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

# include <gnudo-backend/dbdefs.hpp>

# include "edit_priority_dialog.hpp"
# include "ui_edit_priority_dialog.h"


using namespace gnudo;


// TODO Selettore colori



EditPriorityDialog::EditPriorityDialog(QWidget *parent, Db *db, int64_t id):
	QDialog(parent), __ui(new Ui::EditPriorityDialog)
{
	__ui->setupUi(this);
	__db = db;
	__prId = id;
	__isNewPriority = false;

	__commonInit();
}


EditPriorityDialog::EditPriorityDialog(QWidget *parent, Db *db):
	QDialog(parent), __ui(new Ui::EditPriorityDialog)
{
	__ui->setupUi(this);
	__db = db;
	int64_t tmpid = 1;

	for(const auto &id: db->getPriorityLevels()->getIdList(dbdefs::columns::prioritylevel::priority, true))
	{
		if(tmpid == id)
			tmpid++;
		else
			break;
	}

	__prId = db->getPriorityLevels()->add("bho", tmpid);
	__isNewPriority = true;

	__commonInit();
}


void
EditPriorityDialog::__commonInit()
{
	PriorityLevel p = __db->getPriorityLevels()->getPriorityLevel(__prId);

	__name = p.getName();
	__color = p.getColor();
	__level = p.getLevel();

	__ui->lineEdit->setText(QString(__name.c_str()));
	__ui->lineEdit_2->setText(QString(__color.c_str()));
	__ui->spinBox->setValue(__level);
}


EditPriorityDialog::~EditPriorityDialog()
{
	delete __ui;
}


void EditPriorityDialog::__on_buttonBox_accepted()
{
	PriorityLevel p = __db->getPriorityLevels()->getPriorityLevel(__prId);

	if(__ui->lineEdit->text() != QString(__name.c_str())) p.setName(__ui->lineEdit->text().toStdString().c_str());
	if(__ui->lineEdit_2->text() != QString(__color.c_str())) p.setColor(__ui->lineEdit_2->text().toStdString().c_str());
	// FIXME RIMUOVERE
	//	if(__ui->spinBox->value() != __level) p.setLevel(__ui->spinBox->value());
}


void EditPriorityDialog::__on_buttonBox_rejected()
{
	if (__isNewPriority)
		__db->getPriorityLevels()->sqlite3pp::objects::Table::remove(__prId);
}
