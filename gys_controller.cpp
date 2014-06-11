#include <QtDebug>

#include "gys_controller.h"
#include "gys_csvfetcher.h"

GYS::Controller::Controller(QObject *parent) noexcept
    :QObject(parent)
{
    LOG_ENTRY;
}

GYS::Controller::~Controller()
{
    LOG_ENTRY;
}

void GYS::Controller::launch() noexcept
{
    LOG_ENTRY;
    emit sendError("Lauch preparation not implemented");
}

void GYS::Controller::exit() noexcept
{
    LOG_ENTRY;
    emit sendError("Exit preparation not implemented");
}

void GYS::Controller::loadFile(QString filePath) noexcept
{
    LOG_ENTRY;
    GYS::CSVFetcher fetcher;
    GYS::DataTable_Map table;
    // TODO: check and handle errors
    fetcher.setFile(filePath);
    emit fileLoaded();
    quint32 rows = fetcher.getRowsCount();


    quint32 iters = rows / 128 + 1;
    for (quint32 i = 0; i < iters; i++)
    {
        table = fetcher.getData(128);
        emit sendSitesData(table);
    }
}

void GYS::Controller::updateRating(QList< QString > sites) noexcept
{
    LOG_ENTRY;
    emit sendError("Updating not implemented");
}

void GYS::Controller::updateAll() noexcept
{
    LOG_ENTRY;

#if 0
    // TODO: next code is used as an example
    // should be removed
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
#endif
    emit sendError("Updating not implemented");
}

void GYS::Controller::findSimilar(QList< QString > sites) noexcept
{
    LOG_ENTRY;
    emit sendError("Finding not implemented");
}

void GYS::Controller::updateSimilar(QList< QString > sites) noexcept
{
    LOG_ENTRY;
    emit sendError("Updating not implemented");

}
