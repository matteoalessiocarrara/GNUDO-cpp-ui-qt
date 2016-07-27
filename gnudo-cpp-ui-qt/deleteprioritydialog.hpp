#ifndef DELETEPRIORITYDIALOG_HPP
#define DELETEPRIORITYDIALOG_HPP

#include <QDialog>
# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>

using namespace gnudo::sqlite;


namespace Ui {
	class DeletePriorityDialog;
}

class DeletePriorityDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit DeletePriorityDialog(QWidget *parent, Db *db, int64_t id);
		~DeletePriorityDialog();

	private slots:
		void on_buttonBox_accepted();

	private:
		Ui::DeletePriorityDialog *ui;
		Db *db;
		int64_t id;
};

#endif // DELETEPRIORITYDIALOG_HPP
