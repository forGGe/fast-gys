#include <QFileDialog>
#include <QMessageBox>

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

void MainWindow::on_btnExit_clicked()
{
    LOG_ENTRY;
    exit(0);
}

void MainWindow::lauchDone()
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
    int gotRows = data.size();
    int targetRow;
    QTableWidgetItem *newItem = NULL;

    // Preallocate maximum expected amount of rows
    // decrese it after if needed
    // TODO: error check
    ui->mainSitesTable->setRowCount(gotRows + ui->mainSitesTable->rowCount());
    ui->mainSitesTable->setColumnCount(6);
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        GYS::DataItem_Pair siteName = it.key();
        GYS::DataRow_Vec siteRow = it.value();

        newItem = new QTableWidgetItem(siteName.second);
        // Find if such item already exist
        // TODO: use list containing particular name and its row,
        // to quickly find a dublicates
        for (targetRow = 0; targetRow < ui->mainSitesTable->rowCount(); ++targetRow)
            if (ui->mainSitesTable->item(targetRow, 0) == nullptr ||
                    (ui->mainSitesTable->item(targetRow, 0)->text() == siteName.second))
                break;

        ui->mainSitesTable->setItem(targetRow, 0, newItem);

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

            ui->mainSitesTable->setItem(targetRow, col, newItem);
        }
    }
}


void MainWindow::errorSlot(QString descr)
{
    // TODO: Message box
    QMessageBox msgBox;
    msgBox.setText(descr);
    msgBox.exec();
}


void MainWindow::on_btnLoadFile_clicked()
{
    LOG_ENTRY;
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
