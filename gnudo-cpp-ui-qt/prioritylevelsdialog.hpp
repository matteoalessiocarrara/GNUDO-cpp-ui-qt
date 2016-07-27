#ifndef PRIORITYLEVELSDIALOG_HPP
#define PRIORITYLEVELSDIALOG_HPP

#include <QDialog>

# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>
using namespace gnudo::sqlite;


namespace Ui {
	class PriorityLevelsDialog;
}

class PriorityLevelsDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit PriorityLevelsDialog(QWidget *parent, Db *db);
		~PriorityLevelsDialog();

	private slots:
		void on_pushButton_clicked();
		void on_pushButton_3_clicked();
		void on_pushButton_2_clicked();
		void refreshTableContent();

		void on_tableWidget_doubleClicked(const QModelIndex &index);

	private:
		vector<int64_t> tableIdAssociation;
		Ui::PriorityLevelsDialog *ui;
		Db *db;

};

#endif // PRIORITYLEVELSDIALOG_HPP
