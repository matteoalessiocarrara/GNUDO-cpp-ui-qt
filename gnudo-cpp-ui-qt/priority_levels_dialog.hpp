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

# ifndef PRIORITY_LEVEL_SDIALOG_HPP
# define PRIORITY_LEVEL_SDIALOG_HPP

# include <QDialog>
# include <gnudo-backend/gnudo.hpp>


using namespace gnudo;


namespace Ui
{
	class PriorityLevelsDialog;
}


class PriorityLevelsDialog: public QDialog
{
	Q_OBJECT

	public:
		explicit PriorityLevelsDialog(QWidget *parent, Db *__db);
		~PriorityLevelsDialog();

	private slots:
		// XXX Usare nomi più significativi
		void on_pushButton_clicked();
		void on_pushButton_3_clicked();
		void on_pushButton_2_clicked();
		void __refreshTableContent();
		void on_tableWidget_doubleClicked(const QModelIndex &index);

	private:
		vector<int64_t> __tableIdAssociation;
		Ui::PriorityLevelsDialog *__ui;
		Db *__db;
};


# endif // PRIORITY_LEVEL_SDIALOG_HPP
