#include "taskinfodialog.h"
#include "ui_taskinfodialog.h"
# include <vector>
# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>
# include <prioritylevelsdialog.hpp>


using namespace gnudo::sqlite;
using std::vector;


TaskInfoDialog:: TaskInfoDialog(QWidget *parent, Db *db): QDialog(parent), ui(new Ui::TaskInfoDialog)
{
	ui->setupUi(this);
	__db = db;
	__taskId = db->getTasks()->add(db->getPriorityLevels()->getIdList(gnudo::sqlite::PriorityLevelsManager::Order::PRIORITY)[0]);
	isNewTask = true;

	commonInit();
}


TaskInfoDialog::TaskInfoDialog(QWidget *parent, Db *db, int64_t taskId): QDialog(parent), ui(new Ui::TaskInfoDialog)
{
    ui->setupUi(this);
	__db = db;
	__taskId = taskId;
	isNewTask = false;

	commonInit();
}


void
TaskInfoDialog::commonInit()
{
	Task *t = __db->getTasks()->getTask(__taskId);

	title = t->getTitle();
	description = t->getDescription();
	qcreationTime.setTime_t(t->getCreationTime());
	qmodificationTime.setTime_t(t->getModificationTime());
	completed = t->isCompleted();
	priorityId = t->getPriorityLevel()->sqlite3pp::objects::Row::getId();

	ui->lineEdit->setText(QString(title.c_str()));
	ui->plainTextEdit->setPlainText(QString(description.c_str()));
	ui->dateTimeEdit->setDateTime(qcreationTime);
	ui->dateTimeEdit_2->setDateTime(qmodificationTime);
	ui->checkBox->setChecked(completed);


	updatePriorityLevelsList();
}

TaskInfoDialog::~TaskInfoDialog()
{
    delete ui;
}


void TaskInfoDialog::on_buttonBox_accepted()
{
	Task *t = __db->getTasks()->getTask(__taskId);


	if (ui->lineEdit->text() != QString(title.c_str())) t->setTitle(ui->lineEdit->text().toStdString());
	if (ui->plainTextEdit->toPlainText() != QString(description.c_str())) t->setDescription(ui->plainTextEdit->toPlainText().toStdString());
	if (ui->dateTimeEdit->dateTime() != qcreationTime) t->setCreationTime(ui->dateTimeEdit->dateTime().toTime_t());
	if (ui->dateTimeEdit_2->dateTime() != qmodificationTime) t->setModificationTime(ui->dateTimeEdit_2->dateTime().toTime_t());
	if (ui->checkBox->isChecked() != completed) t->setStatus(ui->checkBox->isChecked());

	vector<int64_t> p = __db->getPriorityLevels()->getIdList(gnudo::sqlite::PriorityLevelsManager::Order::PRIORITY);
	int currentId = p[ui->comboBox->currentIndex()];
	if (currentId != priorityId) t->setPriorityLevel(currentId);
}



void TaskInfoDialog::on_buttonBox_rejected()
{
	if (isNewTask) __db->getTasks()->remove(__taskId);
}


void TaskInfoDialog::on_toolButton_clicked()
{
	PriorityLevelsDialog *pd = new PriorityLevelsDialog(0, __db);
	pd->exec();
	updatePriorityLevelsList();
}



void TaskInfoDialog::updatePriorityLevelsList()
{
	vector<int64_t> p = __db->getPriorityLevels()->getIdList(gnudo::sqlite::PriorityLevelsManager::Order::PRIORITY);

	ui->comboBox->clear();

	for(unsigned i = 0; i < p.size(); i++)
	{
		PriorityLevel *pl = __db->getPriorityLevels()->getPriorityLevel(p[i]);

		ui->comboBox->insertItem(i, QString(pl->getName().c_str()));

		if(pl->sqlite3pp::objects::Row::getId() == priorityId)
			ui->comboBox->setCurrentIndex(i); // TODO Errore se non trovata

		delete pl;
	}
}
