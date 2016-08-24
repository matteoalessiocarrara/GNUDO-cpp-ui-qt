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

# ifndef TASKINFODIALOG_H
# define TASKINFODIALOG_H

# include <string>
# include <ctime>

# include <QDialog>
# include <QDateTime>
# include "../lib/gnudo-backend/src/gnudo.hpp"


using namespace gnudo;
using std::string;


namespace Ui
{
	class TaskInfoDialog;
}


using std::time;
using std::time_t;


class TaskInfoDialog: public QDialog
{
	Q_OBJECT

	public:
		// WARNING Ci deve essere almeno un livello di priorità prima di chiamare questi costruttori

		explicit TaskInfoDialog(QWidget *parent, Db *db);
		explicit TaskInfoDialog(QWidget *parent, Db *db, int64_t taskId);
		~TaskInfoDialog();

	private slots:
		void on_buttonBox_accepted();
		void on_buttonBox_rejected();
		void on_toolButton_clicked();

	private:
		Ui::TaskInfoDialog *__ui;
		Db *__db;
		int64_t __taskId;
		bool __isNewTask;

		// Cache
		string __title, __description;
		QDateTime __qcreationTime, __qmodificationTime;
		bool __completed;
		int64_t __priority;

		void __commonInit();
		void __updatePriorityLevelsList();
};


# endif // TASKINFODIALOG_H
