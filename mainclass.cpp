#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QAbstractItemView>

#include "mainclass.h"
#include "mainwindow.h"
#include "exceptions.h"

MainClass::MainClass(QObject *parent)
    :QObject(parent)
    ,m_db(QSqlDatabase::addDatabase("QSQLITE"))
    ,m_model(nullptr)
{
    LOG_ENTRY;

    setupDatabase();

    m_model = new QSqlTableModel(this, m_db);
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_model->setTable("Sites");

    // TODO: do something with those magic numbers
    //m_model->setHeaderData(0, Qt::Horizontal, "ID");
    //m_model->setHeaderData(1, Qt::Horizontal, "Domain");
    m_model->select();
}

MainClass::~MainClass()
{
    LOG_ENTRY;
    m_db.close();
}

void MainClass::setupView(QAbstractItemView *view)
{
    view->setModel(m_model);
}

void MainClass::setupDatabase()
{
    LOG_ENTRY;
    // TODO: error check!
    // Need to create table ONLY if such doesn't exist
    m_db.setDatabaseName("default.db");
    m_db.setUserName("user");
    m_db.setPassword("pass");

    if (m_db.open() == false)
        throw GYS::Exception("Cannot open database");

    // TODO: do something with these magic strings
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

    if (query.exec(createTable) == false)
    {
        LOG_STREAM << query.lastError().type();
        LOG_STREAM << query.lastError().text();
        // Need to create table ONLY if such doesn't exist
        // uncomment it, when things will be changed
        // throw GYS::Exception("Cannot query database");
    }

    while(query.next())
    {
        LOG_STREAM << query.value(0);
    }

    query.finish();
}

// TODO: extend this with argument (custom) that contains
// a row in which data should be placed
// If this argument will be provided, then data
// should be placed in that row using setRecord() method
void MainClass::newData(const QSqlRecord record)
{
    // Check if the same site exists in database
    // If so - copy info from incoming record to the target record
    // If not - insert new record

    LOG_ENTRY;
    // TODO: do something with these magic strings
    int search_column = m_model->record().indexOf("name");

    // Starting search for unique column
    QModelIndex start = m_model->index(0, search_column);
    QModelIndexList list = m_model->match(start,
                                          Qt::DisplayRole,
                                          record.value("name"));

    // No items found - means data might be new
    if (list.isEmpty()) {
        if (m_model->insertRecord(-1, record) == false) {
            LOG_STREAM << "Failed to insert record";
        }
        return;
    }

    // TODO: avoid using ugly indexing
    if (m_model->setRecord(list.at(0).row(), record) == false) {
        LOG_STREAM << "Failed to set record";
    }
}

void MainClass::updateData(const QSqlRecord record)
{
    LOG_ENTRY;
    if (m_model->setRecord(0, record) == false) {
        LOG_STREAM << "Failed to set record";
    }
}
