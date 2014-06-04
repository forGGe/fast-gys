#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <gyscontroller.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_btnExit_clicked();
	void on_btnLoadFile_clicked();

	void on_btnUpdateAll_clicked();

private:
	Ui::MainWindow *ui;

signals:
	void launching();
	void exiting();
	void requestLoadFile(QString file_name);
	void requestUpdateRating(QList< GYS::SiteId > sites);
	void requestUpdateAll();
	void requestFindSimilar(QList< GYS::SiteId > sites);
	void requestUpdateSimilar(QList< GYS::SiteId > sites);

public slots:
	void lauchDone();
	void fileLoadingDone();
	void recieveMainSitesdata(QMap< GYS::SiteId, GYS::SiteData > data);
	void recieveMainSitesRank(QMap< GYS::SiteId, GYS::SiteRank > ranks);
	void recieveSimilarSites(GYS::SiteId mainSite, QList< GYS::SiteData > similarSites);
};

#endif // MAINWINDOW_H
