#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mainclass.h"

namespace Ui {
class MainWindow;
}

class QAbstractItemModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Copying is not permitted.
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator =(MainWindow &) = delete;

private slots:
    void on_btnExit_clicked();
    void on_btnLoadFile_clicked();
    void on_btnUpdateAll_clicked();
    void on_mainSitesTable_cellClicked(int row, int column);
    void on_btnUpdateSelected_clicked();
    void on_checkBox_stateChanged(int arg1);
    void on_pushButton_clicked();

private:
    Ui::MainWindow 	*ui;
    MainClass 		*m_main;

signals:
    void exiting();
    void requestLoadFile(const QString &filePath);
    void requestUpdateAll();
    void requestFindSimilar(QStringList sites);
    void requestUpdateSimilar(QStringList sites);
    void requestDeleteAllFromDatabase();

public slots:
    void lauchDone();
    void fileLoadingDone();
    void updateDone();
    // empty strings are allowed
    void displayError(QString descr, QString details);
};

#endif // MAINWINDOW_H
