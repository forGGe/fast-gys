#include <QtDebug>

#include "gys_controller.h"
#include "gys_csvfetcher.h"
#include "gys_exceptions.h"

GYS::Controller::Controller(QObject *parent) noexcept
    :QObject(parent)
{
    LOG_ENTRY;
}

GYS::Controller::~Controller() noexcept
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
    try
    {
        GYS::CSVFetcher     fetcher;
        GYS::DataTable_Map  table;
        quint32     rows;
        const int   rowsPerSend = 256; // Amount of rows to send in one chunk
        quint32     iters; // Iterations needed to send all file data

        fetcher.setFile(filePath);
        emit fileLoaded();

        rows = fetcher.getRowsCount();
        iters = rows / rowsPerSend + 1;
        for (quint32 i = 0; i < iters; i++)
        {
            table = fetcher.getData(rowsPerSend);
            emit sendSitesData(table);
        }
    }
    catch (GYS::Exception &e)
    {
        QString errStr;
        errStr = e.what();
        qDebug() << "GYS exception raised";
        qDebug() << errStr;
        emit sendError(errStr);
    }
    catch (std::exception &e)
    {
        QString errStr;
        errStr = e.what();
        qDebug() << "STD exception raised";
        qDebug() << errStr;
        emit sendError(errStr);
    }
    catch (...)
    {
        qDebug() << "Exception occurs";
        emit sendError("Unknown error");
    }

    // TODO: check and handle errors
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
