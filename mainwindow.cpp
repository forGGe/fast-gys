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
    LOG_ENTRY;
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
                // Make sure row isn't hidden
                if (!ui->mainSitesTable->isRowHidden(row))
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
    QMessageBox msgBox;
    ui->mainSitesTable->setRowCount(0);
    msgBox.setText("Data has been removed!");
    msgBox.exec();
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
    ui->mainSitesTable->setColumnCount(6);
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        GYS::DataItem_Pair siteName = it.key();
        GYS::DataRow_Vec siteRow = it.value();

        newItem = new QTableWidgetItem(siteName.second);
        // Find if such item already exist
        // TODO: use list containing particular name and its row,
        // to quickly find a dublicates
        for (targetRow = 0; targetRow < filledRowCount; ++targetRow)
        {
            if (ui->mainSitesTable->item(targetRow, 1) == nullptr ||
                    (ui->mainSitesTable->item(targetRow, 1)->text() == siteName.second))
            {
                break;
            }
        }

        if (targetRow == filledRowCount)
            filledRowCount++;

        newItem->setForeground(QBrush(Qt::blue));
        newItem->setFont(urlFont);
        newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
        ui->mainSitesTable->setItem(targetRow, 1, newItem);

        bool outdated = false;

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
            case GYS::ItemType::DATE_UPDATED:
                // Item that was updated one day ago should be marked as outdated
                if (!cell.second.isEmpty())
                {
                    QDate itemTime = QDate::fromString(cell.second);
                    if (itemTime.addDays(1) <= QDate::currentDate())
                        outdated = true;
                }
                break;
            case GYS::ItemType::DATE_ADDED:
                col = 5;
                break;
            case GYS::ItemType::NAME_ID:
            case GYS::ItemType::ASSOC_KEY:
            default:
                LOG_STREAM << "Field ignored";
                break;
            }

            if (col >= 0)
            {
                newItem = new QTableWidgetItem(cell.second);
                newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
                ui->mainSitesTable->setItem(targetRow, col, newItem);
            }
        }

        if (outdated)
        {
            for (int col = 0; col < ui->mainSitesTable->columnCount(); ++col)
            {
                QTableWidgetItem *item = ui->mainSitesTable->item(targetRow, col);
                if (item)
                {
                    // Light-cyan... or something like that
                    item->setBackgroundColor(QColor(196, 244, 255));
                }
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

void MainWindow::on_btnUpdateSelected_clicked()
{
    LOG_ENTRY;
    QList< QTableWidgetSelectionRange > ranges = ui->mainSitesTable->selectedRanges();
    QList< QString > sites;

    for (auto it = ranges.begin(); it != ranges.end(); ++it)
    {
        for (int row = it->topRow(); row < it->bottomRow() + 1; row++)
        {
            // Mark empty item with whitespace
            QTableWidgetItem *item = ui->mainSitesTable->item(row, 1);
            if (item && !item->text().isEmpty())
                sites.append(item->text());
        }
    }

    if (sites.size())
        emit requestUpdateRating(sites);
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    LOG_ENTRY;
    int rowCount = ui->mainSitesTable->rowCount();

    if (arg1) // Checked
    {
        QDateTime hiTime; // Highest time
        QDateTime tmpTime;

        // Find the latest time of items in the table
        for (int row = 0; row < rowCount; ++row)
        {
            QTableWidgetItem *item = ui->mainSitesTable->item(row, 5);
            if (item)
            {
                tmpTime = QDateTime::fromString(item->text());

                if (tmpTime > hiTime)
                    hiTime = tmpTime;
            }
        }

        // Now, hide needed rows
        for (int row = 0; row < rowCount; ++row)
        {
            QTableWidgetItem *item = ui->mainSitesTable->item(row, 5);
            if (item)
            {
                tmpTime = QDateTime::fromString(item->text());
                if (tmpTime < hiTime)
                {
                    ui->mainSitesTable->hideRow(row);
                }
            }
        }

        LOG_STREAM << "hiTime " << hiTime.toString();
    }
    else
    {
        // Show all rows
        for (int row = 0; row < rowCount; ++row)
        {
            if (ui->mainSitesTable->isRowHidden(row))
                ui->mainSitesTable->showRow(row);
        }

    }
}

void MainWindow::on_pushButton_clicked()
{
    emit requestDeleteAllFromDatabase();
}
