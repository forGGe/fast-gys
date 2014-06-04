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

void MainWindow::recieveMainSitesdata(QMap< GYS::SiteId, GYS::SiteData > data)
{
	TRACE_ENTRY;

	// Determine amount of rows right now
	int rows			= data.size();
	int row				= 0;
	int col				= 0;
	int oldcols			= 0;
	int newcols			= 0;
	// Item to put into table
	QTableWidgetItem	*newItem;

	ui->mainSitesTable->setRowCount(rows);
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		// Extract needed data
		const GYS::SiteId	 curId			 = it.key();
		const GYS::SiteData	 curData		 = it.value();
		const GYS::CellArray additionalCells = curData.first;
		const GYS::SiteRank	 curRank		 = curData.second;

		// Determine amount of columns right now
		// In general, amount of columns should be
		// enough to represent a table that contains
		// id, rank and all additional cells that may exist
		newcols = additionalCells.size() + 8;
		if (newcols > oldcols)
		{
			ui->mainSitesTable->setColumnCount(newcols);
			oldcols = newcols;
		}

		col = 0;
		newItem = new QTableWidgetItem(curId);
		ui->mainSitesTable->setItem(row, col++, newItem);
		newItem = new QTableWidgetItem(QString::number(curRank));
		ui->mainSitesTable->setItem(row, col++, newItem);

		for (auto it2 = additionalCells.begin(); it2 != additionalCells.end(); ++it2)
		{
			newItem = new QTableWidgetItem(*it2);
			ui->mainSitesTable->setItem(row, col++, newItem);
		}

		row++;
	}
}

void MainWindow::recieveMainSitesRank(QMap< GYS::SiteId, GYS::SiteRank > ranks)
{
	TRACE_ENTRY;

}

void MainWindow::recieveSimilarSites(GYS::SiteId mainSite, QList< GYS::SiteData > similarSites)
{
	TRACE_ENTRY;

}

void MainWindow::on_btnLoadFile_clicked()
{
	TRACE_ENTRY;
	emit requestLoadFile(QString("test_file.csv"));
}

void MainWindow::on_btnUpdateAll_clicked()
{
	emit requestUpdateAll();
}
