#include <QtDebug>
#include "gyscontroller.h"

GYS::Controller::Controller(QObject *parent) :
	QObject(parent)
{
	TRACE_ENTRY;
}

GYS::Controller::~Controller()
{
	TRACE_ENTRY;

}

void GYS::Controller::launch()
{
	TRACE_ENTRY;
	emit launched();
}

void GYS::Controller::exit()
{
	TRACE_ENTRY;

}

void GYS::Controller::loadFile(QString file_name)
{
	TRACE_ENTRY;
	emit fileLoaded();
}

void GYS::Controller::updateRating(QList< GYS::SiteId > sites)
{
	TRACE_ENTRY;

}

void GYS::Controller::updateAll()
{
	TRACE_ENTRY;
	GYS::SiteRank sr1 = 10;
	GYS::SiteRank sr2 = 105;

	GYS::SiteId sid1 = "OLOLOL.com";
	GYS::SiteId sid2 = "OLOLOL2.com";

	GYS::CellArray sdata1_str = { "BLAH", "BLAH2", "Some cell" };
	GYS::CellArray sdata2_str = { "SPARTAAA", "some sign", "Some again" };

	GYS::SiteData sdata1(sdata1_str, sr1);
	GYS::SiteData sdata2(sdata2_str, sr2);

	QMap< GYS::SiteId, GYS::SiteData > data;
	data.insert(sid1, sdata1);
	data.insert(sid2, sdata2);

	emit sendMainSitesData(data);
}

void GYS::Controller::findSimilar(QList< GYS::SiteId > sites)
{
	TRACE_ENTRY;

}

void GYS::Controller::updateSimilar(QList< GYS::SiteId > sites)
{
	TRACE_ENTRY;

}
