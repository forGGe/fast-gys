#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    TRACE_ENTRY;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    TRACE_ENTRY;
    delete ui;
}

void MainWindow::on_btnExit_clicked()
{
    TRACE_ENTRY;
    exit(0);
}

void MainWindow::lauchDone()
{
    TRACE_ENTRY;

}

void MainWindow::fileLoadingDone()
{
    TRACE_ENTRY;

}

void MainWindow::recieveSitesData(GYS::DataTable_Map data)
{
    TRACE_ENTRY;
    int rows = data.size();
    int row  = 0;
    QTableWidgetItem *newItem = NULL;

    ui->mainSitesTable->setRowCount(rows);
    ui->mainSitesTable->setColumnCount(6);
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        GYS::DataItem_Pair siteName = it.key();
        GYS::DataRow_Vec siteRow = it.value();

        newItem = new QTableWidgetItem(siteName.second);
        ui->mainSitesTable->setItem(row, 0, newItem);

        for (auto it2 = siteRow.begin(); it2 != siteRow.end(); ++it2)
        {
            GYS::DataItem_Pair cell = *it2;
            int col = 0;
            switch(cell.first)
            {
            case GYS::ItemType::NUM_ID:
                col = 1;
                break;
            case GYS::ItemType::DATE_ADDED:
                col = 2;
                break;
            case GYS::ItemType::REGION_RANK:
                col = 3;
                break;
            case GYS::ItemType::WORLD_RANK:
                col = 4;
                break;
            case GYS::ItemType::NAME_ID:
            case GYS::ItemType::PARENT_KEY:
            default:
                qDebug() << "Spurious action!";
                break;
            }

            if (col)
                newItem = new QTableWidgetItem(cell.second);

            ui->mainSitesTable->setItem(row, col, newItem);
        }
        row++;
    }
}


void MainWindow::on_btnLoadFile_clicked()
{
    TRACE_ENTRY;
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open CSV",
                                                    ".",
                                                    "Comma separated value (*.csv)");

    emit requestLoadFile(filePath);
}

void MainWindow::on_btnUpdateAll_clicked()
{
    emit requestUpdateAll();
}
