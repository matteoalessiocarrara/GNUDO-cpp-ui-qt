#ifndef TASKINFODIALOG_H
#define TASKINFODIALOG_H

# include <QDialog>
# include <QDateTime>
# include <ctime>

# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>

using namespace gnudo::sqlite;

namespace Ui
{
	class TaskInfoDialog;
}

using std::time;
using std::time_t;

class TaskInfoDialog : public QDialog
{
		Q_OBJECT



	public:
		// WARNING Ci deve essere almeno un livello di priorità prima di chiamare questo costruttore
		explicit TaskInfoDialog(QWidget *parent, Db *db);
		explicit TaskInfoDialog(QWidget *parent, Db *db, int64_t taskId);
		~TaskInfoDialog();

	private slots:
		void on_buttonBox_accepted();
		void on_buttonBox_rejected();

		void on_toolButton_clicked();

	private:
		Ui::TaskInfoDialog *ui;
		Db *__db;
		int64_t __taskId;
		bool isNewTask;

		string title, description;
		QDateTime qcreationTime, qmodificationTime;
		bool completed;
		int64_t priorityId;

		void commonInit();
		void updatePriorityLevelsList();
};

#endif // TASKINFODIALOG_H
