#include "prioritylevelsdialog.hpp"
#include "ui_prioritylevelsdialog.h"
# include "editprioritydialog.hpp"

# include <QMessageBox>
# include <string>
# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>


using namespace gnudo::sqlite;
using std::to_string;


// TODO Aggiungere barra dei menu
// TODO Modificare direttamente dalla tabella
// TODO Aggiungere ordinamento tabella
// TODO Larghezza colonne

PriorityLevelsDialog::PriorityLevelsDialog(QWidget *parent, Db *db): QDialog(parent), ui(new Ui::PriorityLevelsDialog)
{
	ui->setupUi(this);
	this->db = db;

	refreshTableContent();
}


PriorityLevelsDialog::~PriorityLevelsDialog()
{
	delete ui;
}


void PriorityLevelsDialog::refreshTableContent()
{

	vector<int64_t> idList = db->getPriorityLevels()->getIdList();
	unsigned tableRows = 0;

	tableIdAssociation.clear();
	ui->tableWidget->setRowCount(0);

	for(unsigned i = 0; i < idList.size(); i++)
	{
		PriorityLevel *pr = db->getPriorityLevels()->getPriorityLevel(idList[i]);

		ui->tableWidget->setRowCount(++tableRows);

		ui->tableWidget->setItem(tableRows - 1, 0, new QTableWidgetItem(QString(to_string(pr->getLevel()).c_str())));
		ui->tableWidget->setItem(tableRows - 1, 1, new QTableWidgetItem(QString(pr->getName().c_str())));
		ui->tableWidget->setItem(tableRows - 1, 2, new QTableWidgetItem(QString(pr->getColor().c_str())));

		tableIdAssociation.insert(tableIdAssociation.end(), pr->sqlite3pp::objects::Row::getId());

		delete pr;
	}
}

void PriorityLevelsDialog::on_pushButton_clicked()
{
	EditPriorityDialog e(0, db);
	e.exec();
	refreshTableContent();
}


void PriorityLevelsDialog::on_pushButton_3_clicked()
{
	QModelIndexList indexes = ui->tableWidget->selectionModel()->selection().indexes();

	if (indexes.count() == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("Nessuna riga selezionata");
		msgBox.exec();
		return;
	}


	EditPriorityDialog e(0, db, tableIdAssociation[indexes.at(0).row()]);
	e.exec();
	refreshTableContent();
}


void PriorityLevelsDialog::on_pushButton_2_clicked()
{
	QModelIndexList indexes = ui->tableWidget->selectionModel()->selection().indexes();

	if (indexes.count() == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("Nessuna riga selezionata");
		msgBox.exec();
		return;
	}


	for (int i = 0; i < indexes.count(); ++i)
	{
		QModelIndex index = indexes.at(i);

		int64_t prId = tableIdAssociation[index.row()];
		db->getPriorityLevels()->remove(prId); // FIXME Va usato l altro remove, bisogna modificare i task
	}

	refreshTableContent();
}


void PriorityLevelsDialog::on_tableWidget_doubleClicked(const QModelIndex &index)
{
	EditPriorityDialog e(0, db, tableIdAssociation[index.row()]);
	e.exec();
	refreshTableContent();
}
