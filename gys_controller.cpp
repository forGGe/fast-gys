#include <QtDebug>

#include "gys_controller.h"
#include "gys_csvfetcher.h"
#include "gys_exceptions.h"

GYS::Controller::Controller(QObject *parent) noexcept
    :QObject(parent)
    ,m_storage()
    ,m_ranks()
{
    LOG_ENTRY;
    QObject::connect(&m_ranks, &GYS::Ranks::errorOccurs,
                     this, &GYS::Controller::errorSlot);

    QObject::connect(&m_ranks, &GYS::Ranks::ranksReady,
                     this, &GYS::Controller::consumeData);
}

GYS::Controller::~Controller() noexcept
{
    LOG_ENTRY;
}

void GYS::Controller::consumeData(GYS::DataRow_Vec data)
{
    LOG_ENTRY;
    emit sendError("Data consuming is not implemented");
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

                // To make sure items are already present inside database
                // ranks data should be fetched after items was added to database
                // for (auto it = table.begin(); it != table.end(); ++it)
                //    m_ranks.getRanksData(it.key());
            }
        }

        m_storage.flush();
        emit fileLoaded();
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
