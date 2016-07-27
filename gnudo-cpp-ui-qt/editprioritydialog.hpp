#ifndef EDITPRIORITYDIALOG_HPP
#define EDITPRIORITYDIALOG_HPP

#include <QDialog>
# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>

using namespace gnudo::sqlite;

namespace Ui {
	class EditPriorityDialog;
}

class EditPriorityDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit EditPriorityDialog(QWidget *parent, Db *db);
		explicit EditPriorityDialog(QWidget *parent, Db *db, int64_t id);
		~EditPriorityDialog();

	private slots:
		void on_buttonBox_accepted();

		void on_buttonBox_rejected();

	private:
		Ui::EditPriorityDialog *ui;
		Db *db;
		int64_t prId;
		bool isNewPriority;

		string name, color;
		int level;

		void commonInit();
};

#endif // EDITPRIORITYDIALOG_HPP
