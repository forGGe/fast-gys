#include <QtDebug>

#include "controller.h"
#include "csvfetcher.h"
#include "exceptions.h"

GYS::Controller::Controller(QObject *parent)
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

GYS::Controller::~Controller()
{
    LOG_ENTRY;
}

void GYS::Controller::consumeData(DataTable_Map data)
{
    LOG_ENTRY;
    QString date = QDate::currentDate().toString();
    GYS::DataItem_Pair dataItem = { GYS::ItemType::DATE_UPDATED, date };

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        it.value().push_back(dataItem);
    }

    m_storage.updateRecords(data);
    emit sendSitesData(data);
}

void GYS::Controller::errorSlot(QString err)
{
    LOG_ENTRY;
    emit sendError(err);
}

void GYS::Controller::launch()
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

void GYS::Controller::exit()
{
    LOG_ENTRY;
    emit sendError("Exit preparation not implemented");
}

void GYS::Controller::loadFile(QString filePath)
{
    LOG_ENTRY;
    try
    {
        GYS::CSVFetcher     fetcher;
        GYS::DataTable_Map  table;
        GYS::DataItem_Pair  curTime;
        quint32     rows;
        const uint  rowsPerRead = 64; // Amount of rows to get
        const uint  rowsPerSend = 512;
        quint32     iters; // Iterations needed to send all file data
        QString     time;

        time = QDateTime::currentDateTime().toString();
        curTime = { GYS::ItemType::DATE_ADDED, time };

        fetcher.setFile(filePath);

        rows = fetcher.getRowsCount();
        iters = rows / rowsPerRead + 1;

        for (quint32 i = 0; i < iters; i++)
        {
            table = fetcher.getData(rowsPerRead);
            if (table.size())
            {
                // Append records with time of file loading
                for (auto it = table.begin(); it != table.end(); ++it)
                {
                    it->append(curTime);
                }

                // Storage will filter records up to its logic
                m_storage.addRecords(table);
            }
        }

        m_storage.flush();
        table.clear();

        // Send filtered data from the storage to view
        while ((table = m_storage.getNextRecords(rowsPerSend)).size() > 0)
            emit sendSitesData(table);

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

void GYS::Controller::updateRating(Sites_List sites)
{
    LOG_ENTRY;
    //emit sendError("Updating not implemented");
    for (auto it = sites.begin(); it != sites.end(); ++it)
    {
        m_ranks.getRanksData(*it);
    }
}

void GYS::Controller::updateAll()
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

void GYS::Controller::findSimilar(Sites_List sites)
{
    LOG_ENTRY;
    emit sendError("Finding not implemented");
}

void GYS::Controller::updateSimilar(QList< QString > sites)
{
    LOG_ENTRY;
    emit sendError("Updating not implemented");

}

void GYS::Controller::deleteAllFromDatabase()
{
    LOG_ENTRY;
    try
    {
        m_storage.clearStorage();
        emit allDataDeleted();
    }
    catch(...)
    {
        emit sendError("Delete operation has failed");
    }
}
