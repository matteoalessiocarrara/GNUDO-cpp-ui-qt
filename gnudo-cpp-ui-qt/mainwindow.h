#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <QMainWindow>
# include <QModelIndex>
# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>
# include <vector>
# include <sqlite3.h>


using namespace gnudo::sqlite;

namespace Ui
{
	class MainWindow;
}


class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

		enum ColumnCombobox
		{
			TITLE = 0,
			DESCRIPTION = 4,
			CREATION_TIME = 1,
			MODIFICATION_TIME = 2,
			COMPLETED = 3
		};

		enum RuleCombobox
		{
			ASCENDING = 0,
			DESCENDING = 1
		};

	private slots:
		void onOrderByColumnChanged(int selection);
		void onOrderRuleChanged(int rule);
		void showNewDbDialog();
		void showOpenDbDialog();
		void showAddTaskDialog();
		void removeTask();
		void refreshTableContent();
		void toggleShowCompletedTask();

		void on_tableWidget_doubleClicked(const QModelIndex &index);

	private:
		Ui::MainWindow *ui;
		Db *db;
		bool showCompletedTask;
		vector<sqlite3_int64> tableIdAssociation;

		TasksManager::Order orderByColumn;
		bool orderAscending;

		// Restituisce true se c'è un db aperto
		bool requireOpenDb();
};

#endif // MAINWINDOW_H
