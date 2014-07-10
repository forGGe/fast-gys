#include <QFileDialog>
#include <QMessageBox>
#include <QVariant>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QTableWidgetSelectionRange>
#include <QClipboard>

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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event == QKeySequence::Copy)
    {
        LOG_STREAM << "Copy event occurs";
        QList< QTableWidgetSelectionRange > ranges = ui->mainSitesTable->selectedRanges();
        for (auto it = ranges.begin(); it != ranges.end(); ++it)
        {
            // For now - iterate and overwrite clipboard with latest selected data
            // In order to paste items in Excel clipboard should contain
            // cells separated with tabs and rows separated by new line
            QClipboard *clipboard = QApplication::clipboard();
            QString text;
            for (int row = it->topRow(); row < it->bottomRow() + 1; row++)
            {
                for (int col = it->leftColumn(); col < it->rightColumn() + 1; col++)
                {
                    // Mark empty item with whitespace
                    QTableWidgetItem *item = ui->mainSitesTable->item(row, col);
                    if (item)
                        text += item->text();
                    else
                        text += " ";

                    text += '\t';
                }

                text += '\n';
            }

            LOG_STREAM << text;

            clipboard->setText(text);
        }
        event->accept();
        return;
    }

    event->ignore();
}

void MainWindow::on_btnExit_clicked()
{
    LOG_ENTRY;
    exit(0);
}

void MainWindow::lauchDone()
{
    LOG_ENTRY;
    QMessageBox msgBox;
    msgBox.setText("Database loaded!");
    msgBox.exec();
}

void MainWindow::clearTable()
{
    LOG_ENTRY;
    LOG_STREAM << ui->mainSitesTable->rowCount();
    ui->mainSitesTable->setRowCount(0);
}

void MainWindow::fileLoadingDone()
{
    LOG_ENTRY;
    QMessageBox msgBox;
    msgBox.setText("CSV File loaded!");
    msgBox.exec();
}

void MainWindow::recieveSitesData(GYS::DataTable_Map data)
{
    LOG_ENTRY;
    int gotRows = data.size();
    int targetRow;
    int filledRowCount;

    QTableWidgetItem *newItem = NULL;
    QFont urlFont;

    urlFont.setUnderline(true);

    // Preallocate maximum expected amount of rows
    // decrese it after if needed
    // TODO: error check
    filledRowCount = ui->mainSitesTable->rowCount();
    ui->mainSitesTable->setRowCount(gotRows + filledRowCount);
    ui->mainSitesTable->setColumnCount(5);
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        GYS::DataItem_Pair siteName = it.key();
        GYS::DataRow_Vec siteRow = it.value();

        newItem = new QTableWidgetItem(siteName.second);
        // Find if such item already exist
        // TODO: use list containing particular name and its row,
        // to quickly find a dublicates
        for (targetRow = 0; targetRow < filledRowCount; ++targetRow)
            if (ui->mainSitesTable->item(targetRow, 1) == nullptr ||
                    (ui->mainSitesTable->item(targetRow, 1)->text() == siteName.second))
            {
                break;
            }

        if (targetRow == filledRowCount)
            filledRowCount++;

        newItem->setForeground(QBrush(Qt::blue));
        newItem->setFont(urlFont);
        ui->mainSitesTable->setItem(targetRow, 1, newItem);

        for (auto it2 = siteRow.begin(); it2 != siteRow.end(); ++it2)
        {
            GYS::DataItem_Pair cell = *it2;
            int col = -1;
            switch(cell.first)
            {
            case GYS::ItemType::NUM_ID:
                col = 0;
                break;
            case GYS::ItemType::WORLD_RANK:
                col = 2;
                break;
            case GYS::ItemType::REGION_ID:
                col = 3;
                break;
            case GYS::ItemType::REGION_RANK:
                col = 4;
                break;
            case GYS::ItemType::DATE_ADDED:
            case GYS::ItemType::NAME_ID:
            case GYS::ItemType::ASSOC_KEY:
            default:
                LOG_STREAM << "Field ignored";
                break;
            }

            if (col >= 0) {
                newItem = new QTableWidgetItem(cell.second);
                ui->mainSitesTable->setItem(targetRow, col, newItem);
            }
        }
    }
    ui->mainSitesTable->setRowCount(filledRowCount);
}


void MainWindow::errorSlot(QString descr)
{
    LOG_ENTRY;
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

void MainWindow::on_mainSitesTable_cellClicked(int row, int column)
{
    LOG_ENTRY;
    if (column == 1)
    {
        QString name = ui->mainSitesTable->item(row, column)->text();
        LOG_STREAM << "clicked " << name;
        QUrl url;
        url.setScheme("http");
        url.setHost(name);
        QDesktopServices::openUrl(url);
    }
}
