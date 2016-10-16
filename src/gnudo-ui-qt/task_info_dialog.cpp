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

# include "../gnudo-backend/src/dbdefs.hpp"
# include "../gnudo-backend/src/gnudo.hpp"

# include "priority_levels_dialog.hpp"
# include "task_info_dialog.hpp"
# include "ui_task_info_dialog.h"


using namespace gnudo;
using std::vector;


TaskInfoDialog:: TaskInfoDialog(QWidget *parent, Db *db):
	QDialog(parent), __ui(new Ui::TaskInfoDialog)
{
	__ui->setupUi(this);
	__db = db;
	__taskId = db->getTasks()->add(db->getPriorityLevels()->getIdList(dbdefs::columns::prioritylevel::priority, false)[0]);
	__isNewTask = true;

	__commonInit();
}


TaskInfoDialog::TaskInfoDialog(QWidget *parent, Db *db, int64_t taskId): QDialog(parent), __ui(new Ui::TaskInfoDialog)
{
    __ui->setupUi(this);
	__db = db;
	__taskId = taskId;
	__isNewTask = false;

	__commonInit();
}


void
TaskInfoDialog::__commonInit()
{
	Task t = __db->getTasks()->getTask(__taskId);

	__title = t.getTitle();
	__description = t.getDescription();
	__qcreationTime.setTime_t(t.getCreationTime());
	__qmodificationTime.setTime_t(t.getModificationTime());
	__completed = t.isCompleted();
	__priority = t.getPriorityLevel().getLevel();

	__ui->lineEdit->setText(QString(__title.c_str()));
	__ui->plainTextEdit->setPlainText(QString(__description.c_str()));
	__ui->dateTimeEdit->setDateTime(__qcreationTime);
	__ui->dateTimeEdit_2->setDateTime(__qmodificationTime);
	__ui->checkBox->setChecked(__completed);

	__updatePriorityLevelsList();
}


TaskInfoDialog::~TaskInfoDialog()
{
    delete __ui;
}


void
TaskInfoDialog::on_buttonBox_accepted()
{
	Task t = __db->getTasks()->getTask(__taskId);


	if (__ui->lineEdit->text() != QString(__title.c_str())) t.setTitle(__ui->lineEdit->text().toStdString().c_str());
	if (__ui->plainTextEdit->toPlainText() != QString(__description.c_str())) t.setDescription(__ui->plainTextEdit->toPlainText().toStdString().c_str());
	if (__ui->dateTimeEdit->dateTime() != __qcreationTime) t.setCreationTime(__ui->dateTimeEdit->dateTime().toTime_t());
	if (__ui->dateTimeEdit_2->dateTime() != __qmodificationTime) t.setModificationTime(__ui->dateTimeEdit_2->dateTime().toTime_t());
	if (__ui->checkBox->isChecked() != __completed) t.setStatus(__ui->checkBox->isChecked());

	vector<int64_t> p = __db->getPriorityLevels()->getIdList(dbdefs::columns::prioritylevel::priority);
	int currentId = p[__ui->comboBox->currentIndex()];
	if (currentId != __priority) t.setPriorityLevel(currentId);
}



void
TaskInfoDialog::on_buttonBox_rejected()
{
	if (__isNewTask) __db->getTasks()->remove(__taskId);
}


void
TaskInfoDialog::on_toolButton_clicked()
{
	PriorityLevelsDialog pd(0, __db);
	pd.exec();
	__updatePriorityLevelsList();
}


void
TaskInfoDialog::__updatePriorityLevelsList()
{
	vector<int64_t> p = __db->getPriorityLevels()->getIdList(dbdefs::columns::prioritylevel::priority);

	__ui->comboBox->clear();

	for(unsigned i = 0; i < p.size(); i++)
	{
		PriorityLevel pl = __db->getPriorityLevels()->getPriorityLevel(p[i]);
		__ui->comboBox->insertItem(i, QString(pl.getName().c_str()));

		if(p[i] == __priority)
			__ui->comboBox->setCurrentIndex(i); // TODO Errore se non trovata
	}
}
