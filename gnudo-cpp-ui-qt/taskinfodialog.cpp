#include "taskinfodialog.h"
#include "ui_taskinfodialog.h"

TaskInfoDialog::TaskInfoDialog(QWidget *parent, QString title, QString description, time_t creationTime,
                               time_t modificationTime, bool completed): QDialog(parent), ui(new Ui::TaskInfoDialog)
{
    ui->setupUi(this);
    this->title = title;
    this->description = description;
    this->creationTime = creationTime;
    this->modificationTime = modificationTime;
    this->completed = completed;

    ui->lineEdit->setText(title);
    ui->plainTextEdit->setPlainText(description);

    QDateTime qcreationTime, qmodificationTime;
    qcreationTime.setTime_t(creationTime);
    qmodificationTime.setTime_t(modificationTime);
    ui->dateTimeEdit->setDateTime(qcreationTime);
    ui->dateTimeEdit_2->setDateTime(qmodificationTime);

    ui->checkBox->setChecked(completed);
}

TaskInfoDialog::~TaskInfoDialog()
{
    delete ui;
}

void TaskInfoDialog::on_buttonBox_accepted()
{
    title = ui->lineEdit->text();
    description = ui->plainTextEdit->toPlainText();
    creationTime = ui->dateTimeEdit->dateTime().toTime_t();
    modificationTime = ui->dateTimeEdit_2->dateTime().toTime_t();
    completed = ui->checkBox->isChecked();
}
