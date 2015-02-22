#include <QFileDialog>
#include <QMessageBox>
#include <QVariant>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QTableWidgetSelectionRange>
#include <QClipboard>

#include <QSqlTableModel>


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    LOG_ENTRY;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    LOG_ENTRY;
    delete ui;
}

void MainWindow::setModel(QAbstractItemModel *model)
{
    ui->tableView->setModel(model);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    LOG_ENTRY;
    event->ignore();
}

void MainWindow::on_btnExit_clicked()
{
    LOG_ENTRY;
}

void MainWindow::lauchDone()
{
    LOG_ENTRY;
}

void MainWindow::clearTable()
{
    LOG_ENTRY;
}

void MainWindow::fileLoadingDone()
{
    LOG_ENTRY;
}

void MainWindow::recieveSitesData(GYS::DataTable_Map data)
{
    LOG_ENTRY;
}

void MainWindow::errorSlot(QString descr)
{
    LOG_ENTRY;
}

void MainWindow::on_btnLoadFile_clicked()
{
    LOG_ENTRY;
}

void MainWindow::on_btnUpdateAll_clicked()
{
    LOG_ENTRY;
}

void MainWindow::on_mainSitesTable_cellClicked(int row, int column)
{
    LOG_ENTRY;
}

void MainWindow::on_btnUpdateSelected_clicked()
{
    LOG_ENTRY;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    LOG_ENTRY;
}

void MainWindow::on_pushButton_clicked()
{
    LOG_ENTRY;
}
