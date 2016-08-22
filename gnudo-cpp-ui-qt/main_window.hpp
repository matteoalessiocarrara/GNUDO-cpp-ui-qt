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

# ifndef MAIN_WINDOW_H
# define MAIN_WINDOW_H

# include <cstdint>
# include <vector>

# include <QMainWindow>
# include <QModelIndex>
# include <gnudo-backend/gnudo.hpp>


using namespace gnudo::managers;
using namespace gnudo;

using std::int64_t;
using std::vector;


// TODO Usare nomi più significativi per gli elementi della gui (in tutto il progetto)


namespace Ui
{
	class MainWindow;
}


class MainWindow: public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void __on_tableWidget_doubleClicked(const QModelIndex &index);
		void __onOrderByColumnChanged(int selection);
		void __onOrderRuleChanged(int rule);

		void __showNewDbDialog();
		void __showOpenDbDialog();
		void __showAddTaskDialog();
		void __showPriorityLevelsDialog();

		void __removeTask();
		void __refreshTableContent();
		void __toggleShowCompletedTask();


	private:
		Ui::MainWindow		*__ui;
		Db					*__db;
		bool				__showCompletedTask;
		vector<int64_t>		__tableIdAssociation;
		string				__orderByColumn;
		bool				__orderAscending;


		// Restituisce true se c'è un db aperto
		bool __requireOpenDb();


		// Il valore è l'indice della combobox

		enum ColumnCombobox
		{
			PRIORITY = 0,
			MODIFICATION_TIME = 1,
			CREATION_TIME = 2,
			TITLE = 3,
			COMPLETED = 4,
			DESCRIPTION = 5
		};

		enum RuleCombobox
		{
			ASCENDING = 0,
			DESCENDING = 1
		};
};


# endif // MAIN_WINDOW_H
