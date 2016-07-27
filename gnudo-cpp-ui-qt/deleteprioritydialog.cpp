#include "deleteprioritydialog.hpp"
#include "ui_deleteprioritydialog.h"
# include <gnudo-cpp-dbdriver-abstract/gnudo.hpp>
# include <vector>

using std::vector;


DeletePriorityDialog::DeletePriorityDialog(QWidget *parent, Db *db, int64_t id) :
	QDialog(parent),
	ui(new Ui::DeletePriorityDialog)
{
	ui->setupUi(this);
	this->db = db;
	this->id = id;

	vector<int64_t> p = db->getPriorityLevels()->getIdList(gnudo::sqlite::PriorityLevelsManager::Order::PRIORITY);

	ui->comboBox->clear();

	// TODO Impostare come predefinita una di livello simile

	for(unsigned i = 0; i < p.size(); i++)
	{
		PriorityLevel *pl = db->getPriorityLevels()->getPriorityLevel(p[i]);

		if (not (pl->sqlite3pp::objects::Row::getId() == id))
			ui->comboBox->insertItem(i, QString(pl->getName().c_str()));

		delete pl;
	}
}

DeletePriorityDialog::~DeletePriorityDialog()
{
	delete ui;
}

void DeletePriorityDialog::on_buttonBox_accepted()
{
	int64_t moveTo = db->getPriorityLevels()->getIdList(gnudo::sqlite::PriorityLevelsManager::Order::PRIORITY)[ui->comboBox->currentIndex()];
	db->getPriorityLevels()->remove(id, moveTo);
}
