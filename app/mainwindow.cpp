#include <QFileDialog>
#include <QMessageBox>
#include <QVariant>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QTableWidgetSelectionRange>
#include <QClipboard>

#include <QSqlTableModel>
#include <QSqlField>
#include <QThread>
#include <QThreadPool>


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filefetcher.h"
#include "httpfetcher.h"
#include "rankXMLparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_main(new MainClass(this))
{
    LOG_ENTRY;
    ui->setupUi(this);
    m_main->setupView(ui->tableView);

    // TODO: find better place for it
    qRegisterMetaType < QSqlRecord > ("QSqlRecord");
}

MainWindow::~MainWindow()
{
    LOG_ENTRY;
    delete ui;
}

void MainWindow::on_btnExit_clicked()
{
    LOG_ENTRY;
    QCoreApplication::exit(0);
}

void MainWindow::lauchDone()
{
    LOG_ENTRY;
}

void MainWindow::fileLoadingDone()
{
    LOG_ENTRY;
    QMessageBox msg;
    msg.setText("File loaded");
    msg.exec();
}

void MainWindow::updateDone()
{
    LOG_ENTRY;
    QMessageBox msg;
    msg.setText("Update done");
    msg.exec();
}

void MainWindow::displayError(QString descr, QString details)
{
    // TODO: pixmap here?
    QMessageBox msg;
    msg.setText("Error occurs.");

    if (!descr.isEmpty())
        msg.setInformativeText(descr);

    if (!details.isEmpty())
        msg.setDetailedText(details);

    msg.exec();
}

void MainWindow::on_btnLoadFile_clicked()
{
    LOG_ENTRY;
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open text file",
                                                    ".");

    emit requestLoadFile(filePath);
}

void MainWindow::on_btnUpdateAll_clicked()
{
    LOG_ENTRY;

    emit requestUpdateAll();
}

void MainWindow::on_mainSitesTable_cellClicked(int row, int column)
{
    (void) row;
    (void) column;

    LOG_ENTRY;
}

void MainWindow::on_btnUpdateSelected_clicked()
{
    LOG_ENTRY;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    (void) arg1;
    LOG_ENTRY;
}

void MainWindow::on_pushButton_clicked()
{
    LOG_ENTRY;
}

void MainWindow::on_btnExportFile_clicked()
{
    LOG_ENTRY;
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    "Directory for saving",
                                                    "./untitled.csv",
                                                    "Text files (*.csv)");
    m_main->saveCurrentTableAsCSV(filePath);
}

void MainWindow::on_btnCopyAll_clicked()
{
    try
    {
        LOG_ENTRY;
        m_main->copyAllTableData();
    }

    catch(...)
    {
        displayError("Copy to clipboard failed.", "");
    }
}

//the aim of function is to copy selected data to clipboard
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event == QKeySequence::Copy)
    {
        LOG_STREAM << "Copy event occurs";

        QString text;
        QClipboard *clipboard = QApplication::clipboard();
        int columnsCount = ui->tableView->model()->columnCount();

        //copy process is based on the ability to select only an entire row
        //SelectionBehavior = SelectRows
        QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

        for (auto row : selectedRows)
        {
            for(int i = 0; i < columnsCount; i++)
            {
                QModelIndex cell = ui->tableView->selectionModel()->model()->index(row.row(),i);
                text += cell.data().toString() + "\t";
            }
            text += "\n";
        }
        clipboard->setText(text);
    }
}

