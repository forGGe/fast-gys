#include <QtDebug>

#include "gys_controller.h"
#include "gys_csvfetcher.h"

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

void GYS::Controller::loadFile(QString filePath)
{
	TRACE_ENTRY;
    GYS::CSVFetcher fetcher;
    GYS::DataTable_Map table;
    fetcher.setFile(filePath);
    table = fetcher.getData(100);
	emit fileLoaded();
    emit sendSitesData(table);
}

void GYS::Controller::updateRating(QList< QString > sites)
{
	TRACE_ENTRY;

}

void GYS::Controller::updateAll()
{
	TRACE_ENTRY;

    GYS::DataRow_Vec site1_row =
    {
        { GYS::ItemType::REGION_ID, "US" },
        { GYS::ItemType::REGION_RANK, "10" },
    };

    GYS::DataRow_Vec site2_row =
    {
        { GYS::ItemType::REGION_ID, "RU" },
        { GYS::ItemType::REGION_RANK, "210" },
    };

    GYS::DataTable_Map data =
    {
        { { GYS::ItemType::NAME_ID, "ololoshka.com" }, site2_row },
        { { GYS::ItemType::NAME_ID, "facetable.com" }, site1_row },
    };

    emit sendSitesData(data);
}

void GYS::Controller::findSimilar(QList< QString > sites)
{
	TRACE_ENTRY;

}

void GYS::Controller::updateSimilar(QList< QString > sites)
{
	TRACE_ENTRY;

}
