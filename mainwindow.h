#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "controller.h"
#include "types.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_btnExit_clicked();
    void on_btnLoadFile_clicked();
    void on_btnUpdateAll_clicked();
    void on_mainSitesTable_cellClicked(int row, int column);

    void on_btnUpdateSelected_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_mainSitesTable_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

signals:
    void exiting();
    void requestLoadFile(QString file_name);
    void requestUpdateRating(GYS::Sites_List sites);
    void requestUpdateAll();
    void requestFindSimilar(QList< QString > sites);
    void requestUpdateSimilar(QList< QString > sites);
    void requestDeleteAllFromDatabase();

public slots:
    void lauchDone();
    void fileLoadingDone();
    void clearTable();
    void recieveSitesData(GYS::DataTable_Map data);
    void errorSlot(QString descr);
};

#endif // MAINWINDOW_H
