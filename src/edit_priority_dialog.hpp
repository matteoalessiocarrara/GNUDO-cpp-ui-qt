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

# ifndef EDIT_PRIORITY_DIALOG_HPP
# define EDIT_PRIORITY_DIALOG_HPP

# include <QDialog>
# include <gnudo-backend/gnudo.hpp>


using namespace gnudo;


namespace Ui
{
	class EditPriorityDialog;
}


class EditPriorityDialog: public QDialog
{
	Q_OBJECT

	public:
		explicit EditPriorityDialog(QWidget *parent, Db *__db);
		explicit EditPriorityDialog(QWidget *parent, Db *__db, int64_t id);
		~EditPriorityDialog();

	private slots:
		void on_buttonBox_accepted();
		void on_buttonBox_rejected();

	private:
		Ui::EditPriorityDialog *__ui;
		Db *__db;
		int64_t __prId;
		bool __isNewPriority;

		// Cache
		string __name, __color;
		int __level;

		void __commonInit();
};


# endif // EDIT_PRIORITY_DIALOG_HPP
