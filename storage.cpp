#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>
#include <QDate>

#include "storage.h"
#include "exceptions.h"


GYS::Storage::Storage()
    :m_db(QSqlDatabase::addDatabase("QSQLITE"))
    ,m_row(0)
{
    LOG_ENTRY;
    // TODO: check for valid database
    // TODO: error checks
    bool ok;

    m_db.setDatabaseName("default.db");
    m_db.setUserName("user");
    m_db.setPassword("pass");
    ok = m_db.open();

    QString createTable(
                "CREATE TABLE Sites "
                "( "
                "site_key INTEGER PRIMARY KEY, "
                "site_id TEXT, "
                "name TEXT UNIQUE, "
                "date TEXT, "
                "rank INTEGER, "
                "country TEXT, "
                "local_rank INTEGER, "
                "date_updated TEXT "
                ");"
                );
    QSqlQuery query;
    ok = query.exec(createTable);

    if (!ok)
    {
        LOG_STREAM << query.lastError().type();
        LOG_STREAM << query.lastError().text();
    }

    while(query.next())
    {
        LOG_STREAM << query.value(0);
    }

    query.finish();
}

GYS::Storage::~Storage()
{
    LOG_ENTRY;
    m_db.commit();
    m_db.close();
}

void GYS::Storage::addRecords(const GYS::DataTable_Map &records)
{
    LOG_ENTRY;

    // Let's not waste any resourses
    if (!records.size())
        return;

    if (!m_db.transaction())
        LOG_STREAM << "Transaction fails!";

    // TODO: too straitforward
    // TODO: erorr checks
    QVariantList siteIDs;
    QVariantList siteNames;
    QVariantList dates;

    QSqlQuery query(m_db);
    query.prepare(
                "INSERT OR IGNORE INTO "
                "Sites (site_id, name, date) "
                "VALUES (?, ?, ?);"
                );

    QString gag = "   ";
    for (auto it = records.begin(); it != records.end(); ++it)
    {
        siteNames << it.key().second;
        GYS::DataRow_Vec row = it.value();

        siteIDs << gag;
        dates << gag;
        for (auto it2 = row.begin(); it2 != row.end(); ++it2)
        {
            if (it2->first == GYS::ItemType::NUM_ID)  {
                QVariant &tmp = siteIDs.last();
                tmp = it2->second;
            }
            if (it2->first == GYS::ItemType::DATE_ADDED)  {
                QVariant &tmp = dates.last();
                tmp = it2->second;
            }
        }
    }

    query.addBindValue(siteIDs);
    query.addBindValue(siteNames);
    query.addBindValue(dates);

    if (!query.execBatch())
    {
        LOG_STREAM << query.lastError().type();
        LOG_STREAM << query.lastError().text();
    }

    query.finish();
}

void GYS::Storage::updateRecords(const GYS::DataTable_Map &records)
{
    LOG_ENTRY;

    // Let's not waste any resourses
    if (!records.size())
        return;

    // TODO: Enable transaction after some changes in controller
    // if (!m_db.transaction())
    //     LOG_STREAM << "Transaction fails!";

    QSqlQuery query(m_db);
    QString queryStr = "UPDATE Sites "
                       "SET ";

    for (auto it1 = records.begin(); it1 != records.end(); ++it1) {
        GYS::DataRow_Vec vec = it1.value();

        bool filled = false;

        for (auto it2 = vec.begin(); it2 != vec.end(); ++it2)
        {
            QString str = it2->second;
            GYS::ItemType type = it2->first;
            switch (type)
            {
            case GYS::ItemType::REGION_ID:
                queryStr += "country=\'" + str + "\',";
                filled = true;
                break;
            case GYS::ItemType::REGION_RANK:
                queryStr += "local_rank=\'" + str + "\',";
                filled = true;
                break;
            case GYS::ItemType::WORLD_RANK:
                queryStr += "rank=\'" + str + "\',";
                filled = true;
                break;
            case GYS::ItemType::DATE_UPDATED:
                queryStr += "date_updated=\'" + str + "\',";
                // This shouldn't be threated as useful data to store
                // in case if there are no other data present
                break;
            default:
                break;
            }
        }

        if (filled)
        {
            // Remove trailing ','
            queryStr.chop(1);

            queryStr += " WHERE name=\'" + it1.key().second + "\';";

            if (!query.exec(queryStr))
            {
                LOG_STREAM << query.lastError().type();
                LOG_STREAM << query.lastError().text();
            }

            query.finish();
        }
    }

    return;
}


GYS::DataTable_Map GYS::Storage::getNextRecords(quint64 amount)
{
    LOG_ENTRY;

    // TODO: error checks
    QSqlQuery query(m_db);
    GYS::DataTable_Map table;
    quint64 got = 0;
    query.setForwardOnly(true);
    query.prepare(
                "SELECT site_id, name, date, country, local_rank, rank, date_updated "
                "FROM Sites "
                "LIMIT :amount OFFSET :offset;"
                );

    query.bindValue(":amount", amount);
    query.bindValue(":offset", m_row);

    if (!query.exec())
    {
        LOG_STREAM << query.lastError().type();
        LOG_STREAM << query.lastError().text();
    }

    while (query.next())
    {
        GYS::DataItem_Pair site_id =
        { GYS::ItemType::NUM_ID, query.value(0).toString() };
        GYS::DataItem_Pair key =
        { GYS::ItemType::NAME_ID, query.value(1).toString() };
        GYS::DataItem_Pair date =
        { GYS::ItemType::DATE_ADDED, query.value(2).toString() };
        GYS::DataItem_Pair country =
        { GYS::ItemType::REGION_ID, query.value(3).toString() };
        GYS::DataItem_Pair local_rank =
        { GYS::ItemType::REGION_RANK, query.value(4).toString() };
        GYS::DataItem_Pair rank =
        { GYS::ItemType::WORLD_RANK, query.value(5).toString() };
        GYS::DataItem_Pair dateUpdated =
        { GYS::ItemType::DATE_UPDATED, query.value(6).toString() };

        GYS::DataRow_Vec row = { site_id, date, country, local_rank, rank, dateUpdated };
        table.insert(key, row);
        got++;
    }

    m_row += got;

    query.finish();

    return table;

}

void GYS::Storage::resetGetPosition()
{
    LOG_ENTRY;
    m_row = 0;
}

void GYS::Storage::flush()
{
    LOG_ENTRY;
    m_db.commit();
}

bool GYS::Storage::addRecord(const GYS::DataItem_Pair   &record,
                             const GYS::DataRow_Vec     &data)
{
    LOG_ENTRY;
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

bool GYS::Storage::overwriteRecord(const GYS::DataItem_Pair &record,
                                   const GYS::DataRow_Vec   &data)
{
    LOG_ENTRY;
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

GYS::DataRow_Vec GYS::Storage::getRecordData(const GYS::DataItem_Pair &record) const
{
    LOG_ENTRY;
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

GYS::DataTable_Map GYS::Storage::getRecordsData(const GYS::DataRow_Vec &records) const
{
    LOG_ENTRY;
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

bool GYS::Storage::referRecords(const GYS::DataItem_Pair &record,
                                const QString            &refName,
                                const GYS::DataRow_Vec   &references)
{
    LOG_ENTRY;
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

GYS::DataRow_Vec GYS::Storage::getRefers(const GYS::DataItem_Pair &record,
                                         const QString            &refname) const
{
    LOG_ENTRY;
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

void GYS::Storage::clearStorage()
{
    LOG_ENTRY;
    m_db.commit();

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Sites");
    if (!query.exec())
    {
        LOG_STREAM << query.lastError().type();
        LOG_STREAM << query.lastError().text();
    }
}

