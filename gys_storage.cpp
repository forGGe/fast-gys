#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>

#include "gys_storage.h"
#include "gys_exceptions.h"


GYS::Storage::Storage() noexcept
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
    qDebug() << ok;

    QString createTable(
                "CREATE TABLE Sites "
                "( "
                "site_key INTEGER PRIMARY KEY, "
                "site_id TEXT, "
                "name TEXT UNIQUE, "
                "date TEXT "
                ") "
                );
    QSqlQuery query;
    ok = query.exec(createTable);
    qDebug() << ok;

    if (!ok)
    {
        qDebug() << query.lastError().type();
        qDebug() << query.lastError().text();
    }

    while(query.next())
    {
        qDebug() << query.value(0);
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

    // TODO: too straitforward
    // TODO: erorr checks
    QVariantList siteIDs;
    QVariantList siteNames;
    QVariantList dates;

    QSqlQuery query(m_db);
    query.prepare(
                "INSERT INTO "
                "Sites (site_id, name, date) "
                "VALUES (?, ?, ?) "
                );
    for (auto it = records.begin(); it != records.end(); ++it)
    {
        GYS::DataRow_Vec row = it.value();
        siteIDs << row.at(0).second;
        dates << row.at(1).second;
        siteNames << it.key().second;
    }

    query.addBindValue(siteIDs);
    query.addBindValue(siteNames);
    query.addBindValue(dates);

    if (!query.execBatch())
    {
        qDebug() << query.lastError().type();
        qDebug() << query.lastError().text();
    }

    query.finish();
}

GYS::DataTable_Map GYS::Storage::getNextRecords(quint64 amount)
{
    LOG_ENTRY;

    // TODO: error checks
    // TODO: BUG HERE!
    // It executes and sents not more than 3800 items of overall 3970
    QSqlQuery query(m_db);
    GYS::DataTable_Map table;
    quint64 got = 0;
    query.setForwardOnly(true);
    query.prepare(
                "SELECT site_id, name, date "
                "FROM Sites "
                "LIMIT :amount OFFSET :offset "
                );

    query.bindValue(":amount", amount);
    query.bindValue(":offset", m_row);

    if (!query.exec())
    {
        qDebug() << query.lastError().type();
        qDebug() << query.lastError().text();
    }

    while (query.next())
    {
        GYS::DataItem_Pair site_id =
        { GYS::ItemType::NUM_ID, query.value(0).toString() };
        GYS::DataItem_Pair key =
        { GYS::ItemType::NAME_ID, query.value(1).toString() };
        GYS::DataItem_Pair date =
        { GYS::ItemType::DATE_ADDED, query.value(2).toString() };

        GYS::DataRow_Vec row = { site_id, date };
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
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Sites");
    if (!query.exec())
    {
        qDebug() << query.lastError().type();
        qDebug() << query.lastError().text();
    }
}

