#ifndef MAINWINDOW_H
#define MAINWINDOW_H

# include <QMainWindow>
# include <QModelIndex>
# include <gnudo-cpp-dbdriver-sqlite/gnudo.hpp>
# include <vector>
# include <sqlite3.h>


using namespace gnudo::sqlite;

namespace Ui {
class MainWindow;
}

// TODO Migliorare larghezza automatica colonne
// TODO Permettere ordinamento colonne
// TODO Toolbar
// TODO Database aperti recentemente

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showOpenDbDialog();
    void showAddTaskDialog();
    void removeTask();
    void refreshTableContent();
    void toggleShowCompletedTask();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    Db *db;
    bool showCompletedTask;
    vector<sqlite3_int64> tableIdAssociation;


    // Restituisce true se c'è un db aperto
    bool requireOpenDb();
};

#endif // MAINWINDOW_H
