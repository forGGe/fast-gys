#include <QFileDialog>
#include <QMessageBox>
#include <QVariant>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QTableWidgetSelectionRange>
#include <QClipboard>

#include <QSqlTableModel>
#include <QSqlField>


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_main(new MainClass(this))
{
    LOG_ENTRY;
    ui->setupUi(this);
    m_main->setupView(ui->tableView);

    // Example code, creating\updating item in database
    // {{
    QSqlField site_key("site_key", QVariant::Int);
    QSqlField site_id("site_id", QVariant::String);
    QSqlField name("name", QVariant::String);
    QSqlField country("country", QVariant::String);
    QSqlField rank("rank", QVariant::Int);

    QSqlRecord rec;

    rec.append(site_key);
    rec.append(site_id);
    rec.append(name);
    rec.append(country);
    rec.append(rank);

    // TODO: do something with these magic strings
    rec.setValue("site_key", 10109);
    rec.setValue("site_id", "hello212com.id");
    rec.setValue("name", "hellmore.com");
    rec.setValue("country", "UA");
    rec.setValue("rank", 12);

    m_main->newData(rec);
    // }}
    // EOF example
}

MainWindow::~MainWindow()
{
    LOG_ENTRY;
    delete ui;
}

void MainWindow::on_btnExit_clicked()
{
    LOG_ENTRY;
}

void MainWindow::lauchDone()
{
    LOG_ENTRY;
}

void MainWindow::fileLoadingDone()
{
    LOG_ENTRY;
}

void MainWindow::displayError(QString descr)
{
    LOG_ENTRY;
}

// TODO: highest priority
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
