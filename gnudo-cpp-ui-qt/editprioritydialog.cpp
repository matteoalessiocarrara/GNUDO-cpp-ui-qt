#include "editprioritydialog.hpp"
#include "ui_editprioritydialog.h"


// TODO Selettore colori
// TODO Warning se livello già usato



EditPriorityDialog::EditPriorityDialog(QWidget *parent, Db *db, int64_t id): QDialog(parent), ui(new Ui::EditPriorityDialog)
{
	ui->setupUi(this);
	this->db = db;
	prId = id;
	isNewPriority = false;
	commonInit();
}


EditPriorityDialog::EditPriorityDialog(QWidget *parent, Db *db): QDialog(parent), ui(new Ui::EditPriorityDialog)
{
	ui->setupUi(this);
	this->db = db;
	prId = db->getPriorityLevels()->add("bho", 0);
	isNewPriority = true;
	commonInit();
}


void
EditPriorityDialog::commonInit()
{
	PriorityLevel *p = db->getPriorityLevels()->getPriorityLevel(prId);

	name = p->getName();
	color = p->getColor();
	level = p->getLevel();

	delete p;

	ui->lineEdit->setText(QString(name.c_str()));
	ui->lineEdit_2->setText(QString(color.c_str()));
	ui->spinBox->setValue(level);
}


EditPriorityDialog::~EditPriorityDialog()
{
	delete ui;
}


void EditPriorityDialog::on_buttonBox_accepted()
{
	PriorityLevel *p = db->getPriorityLevels()->getPriorityLevel(prId);

	if(ui->lineEdit->text() != QString(name.c_str())) p->setName(ui->lineEdit->text().toStdString());
	if(ui->lineEdit_2->text() != QString(color.c_str())) p->setColor(ui->lineEdit_2->text().toStdString());
	if(ui->spinBox->value() != level) p->setLevel(ui->spinBox->value());

	delete p;
}

void EditPriorityDialog::on_buttonBox_rejected()
{
	if (isNewPriority)
		db->getPriorityLevels()->remove(prId);
}
