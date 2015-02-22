#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

#include "mainclass.h"
#include "mainwindow.h"
#include "exceptions.h"

MainClass::MainClass(QObject *parent)
    :QObject(parent)
    ,m_db(QSqlDatabase::addDatabase("QSQLITE"))
    ,m_model(nullptr)
    ,m_w(nullptr)
{
    LOG_ENTRY;
    // TODO: error check!
    // Need to create table ONLY if such doesn't exist
    m_db.setDatabaseName("default.db");
    m_db.setUserName("user");
    m_db.setPassword("pass");

    if (m_db.open() == false)
        throw GYS::Exception("Cannot open database");

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

    QSqlTableModel *model = new QSqlTableModel(this, m_db);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("Sites");
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Domain");
    model->select();

    m_model = model;

    m_w = new MainWindow;
    m_w->setModel(m_model);
    m_w->show();
}

MainClass::~MainClass()
{
    LOG_ENTRY;
    delete m_w;
    m_db.close();
}
