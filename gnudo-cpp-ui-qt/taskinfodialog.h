#ifndef TASKINFODIALOG_H
#define TASKINFODIALOG_H

#include <QDialog>
#include <ctime>

namespace Ui {
class TaskInfoDialog;
}

using std::time;
using std::time_t;

class TaskInfoDialog : public QDialog
{
    Q_OBJECT



public:
    explicit TaskInfoDialog(QWidget *parent = 0, QString title="untitled", QString description="",
                            time_t creationTime=time(NULL), time_t modificationTime=time(NULL),
                            bool completed=false);
    ~TaskInfoDialog();

    QString title, description;
    time_t creationTime, modificationTime;
    bool completed;


private slots:
    void on_buttonBox_accepted();

private:
    Ui::TaskInfoDialog *ui;
};

#endif // TASKINFODIALOG_H
