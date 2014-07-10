#include <QtDebug>

#include "gys_controller.h"
#include "gys_csvfetcher.h"
#include "gys_exceptions.h"

GYS::Controller::Controller(QObject *parent) noexcept
    :QObject(parent)
    ,m_storage()
    ,m_ranks(this)
{
    LOG_ENTRY;
    QObject::connect(&m_ranks, &GYS::Ranks::errorOccurs,
                     this, &GYS::Controller::errorSlot);

    QObject::connect(&m_ranks, &GYS::Ranks::ranksReady,
                     this, &GYS::Controller::consumeData,
                     Qt::QueuedConnection);
}

GYS::Controller::~Controller() noexcept
{
    LOG_ENTRY;
}

void GYS::Controller::consumeData(DataTable_Map data)
{
    LOG_ENTRY;
    m_storage.updateRecords(data);
    emit sendSitesData(data);
}

void GYS::Controller::errorSlot(QString err)
{
    LOG_ENTRY;
    emit sendError(err);
}

void GYS::Controller::launch() noexcept
{
    // TODO: improve error checks
    LOG_ENTRY;

    try
    {
        GYS::DataTable_Map toSend;
        const quint64 amount = 1024;
        m_storage.resetGetPosition();

        while ((toSend = m_storage.getNextRecords(amount)).size() > 0)
            emit sendSitesData(toSend);

        emit launched();
    }
    catch (...)
    {
        emit sendError("Lauch preparation failed");
    }
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
        const int   rowsPerSend = 32; // Amount of rows to send in one chunk
        quint32     iters; // Iterations needed to send all file data

        fetcher.setFile(filePath);

        rows = fetcher.getRowsCount();
        iters = rows / rowsPerSend + 1;
        m_storage.clearStorage();
        emit allDataDeleted();

        for (quint32 i = 0; i < iters; i++)
        {
            table = fetcher.getData(rowsPerSend);
            if (table.size())
            {
                m_storage.addRecords(table);

                emit sendSitesData(table);
            }
        }

        m_storage.flush();
        emit fileLoaded();
    }
    catch (GYS::Exception &e)
    {
        QString errStr;
        errStr = e.what();
        LOG_STREAM << "GYS exception raised";
        LOG_STREAM << errStr;
        emit sendError(errStr);
    }
    catch (std::exception &e)
    {
        QString errStr;
        errStr = e.what();
        LOG_STREAM << "STD exception raised";
        LOG_STREAM << errStr;
        emit sendError(errStr);
    }
    catch (...)
    {
        LOG_STREAM << "Exception occurs";
        emit sendError("Unknown error");
    }

    // TODO: check and handle errors
}

void GYS::Controller::updateRating(Sites_List sites) noexcept
{
    LOG_ENTRY;
    //emit sendError("Updating not implemented");
    for (auto it = sites.begin(); it != sites.end(); ++it)
    {
        m_ranks.getRanksData(*it);
    }
}

void GYS::Controller::updateAll() noexcept
{
    LOG_ENTRY;

    GYS::DataTable_Map items;
    const quint64 amount = 512;

    m_storage.resetGetPosition();

    while ((items = m_storage.getNextRecords(amount)).size() > 0)
    {
        for (auto it = items.begin(); it != items.end(); ++it)
            m_ranks.getRanksData(it.key().second);
    }
}

void GYS::Controller::findSimilar(Sites_List sites) noexcept
{
    LOG_ENTRY;
    emit sendError("Finding not implemented");
}

void GYS::Controller::updateSimilar(QList< QString > sites) noexcept
{
    LOG_ENTRY;
    emit sendError("Updating not implemented");

}
