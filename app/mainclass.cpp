#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QAbstractItemView>
#include <QThread>

#include <QMessageBox>

#include "mainclass.h"
#include "mainwindow.h"
#include "exceptions.h"
#include "filefetcher.h"
#include "httpfetcher.h"
#include "rankXMLparser.h"
#include "textfileparser.h"

MainClass::MainClass(MainWindow *parent)
    :QObject(parent)
    ,m_db(QSqlDatabase::addDatabase("QSQLITE"))
    ,m_model(nullptr)
    ,m_mw(parent)
{
    LOG_ENTRY;

    setupDatabase();

    m_model = new QSqlTableModel(this, m_db);
    // TODO: may have performance impact, should be used 'on sumbit'
    // technique
    m_model->setEditStrategy(QSqlTableModel::OnRowChange);
    m_model->setTable("Sites");

    // TODO: do something with those magic numbers
    //m_model->setHeaderData(0, Qt::Horizontal, "ID");
    //m_model->setHeaderData(1, Qt::Horizontal, "Domain");

    QObject::connect(m_mw, &MainWindow::requestUpdateAll,
                     this, &MainClass::updateAll);
    QObject::connect(m_mw, &MainWindow::requestLoadFile,
                     this, &MainClass::loadFile);
}

MainClass::~MainClass()
{
    LOG_ENTRY;
    m_db.close();
}

void MainClass::setupView(QAbstractItemView *view)
{
    view->setModel(m_model);
    m_model->select();
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
        throw Exception("Cannot open database");

    // TODO: do something with these magic strings
    QString createTable(
                "CREATE TABLE IF NOT EXISTS Sites "
                "( "
                "site_key INTEGER PRIMARY KEY, "
                "site_id TEXT, "
                "name TEXT UNIQUE, "
                "email TEXT, "
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
    }

    // columns' names from CREATE TABLE block
    // this hardcode should be modified every time while altering the table
    QVector<QString> columnsNames = {"site_key",
                                     "site_id",
                                     "name",
                                     "email",
                                     "date",
                                     "rank",
                                     "country",
                                     "local_rank",
                                     "date_updated"};

    // checking the structure of a table
    QSqlRecord columns = m_db.record("Sites");
    for(auto column : columnsNames)
    {
        if (columns.indexOf(column) == -1)
            throw Exception("Table Sites has incorrect structure");
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
        return;
    }
}

void MainClass::updateData(const QSqlRecord record)
{
    LOG_ENTRY;
    if (m_model->setRecord(0, record) == false) {
        LOG_STREAM << "Failed to set record";
    }
}

void MainClass::loadFile(const QString &filePath)
{
    Fetcher *fetcher = new FileFetcher< TextFileParser > (filePath);

    QThread *fetcher_thread = new QThread;

    QObject::connect(fetcher, &Fetcher::send,
                     this, &MainClass::newData);

    // Avoiding leaks by using 'deleteLater()' slot
    QObject::connect(fetcher, &Fetcher::end,
                     m_mw, &MainWindow::fileLoadingDone);
    QObject::connect(fetcher, &Fetcher::end,
                     fetcher_thread, &QThread::quit);
    QObject::connect(fetcher, &Fetcher::end,
                     fetcher, &Fetcher::deleteLater);
    QObject::connect(fetcher, &Fetcher::end,
                     fetcher_thread, &QThread::deleteLater);

    fetcher->moveToThread(fetcher_thread);
    fetcher_thread->start();

    QMetaObject::invokeMethod(fetcher, "start");
}

void MainClass::updateAll()
{
    // TODO: delegate this to the MainClass
    // Get all data from table
    // feed fetcher with that data
    // signal about end

    // EXAMPLE CODE
    //QSqlRecord rec;
    //QSqlField name("name", QVariant::String);
    //rec.append(name);
    //rec.setValue("name", "ebay.com");

    Fetcher *fetcher = new HTTPfetcher< RankXMLParser >;
    QThread *fetcher_thread = new QThread;

    QObject::connect(fetcher, &Fetcher::send,
                     this, &MainClass::newData);
    QObject::connect(fetcher, &Fetcher::end,
                     m_mw, &MainWindow::updateDone);

    // Avoiding leaks by using 'deleteLater()' slot
    QObject::connect(fetcher, &Fetcher::end,
                     fetcher_thread, &QThread::quit);
    QObject::connect(fetcher, &Fetcher::end,
                     fetcher, &Fetcher::deleteLater);
    QObject::connect(fetcher, &Fetcher::end,
                     fetcher_thread, &QThread::deleteLater);

    fetcher->moveToThread(fetcher_thread);
    fetcher_thread->start();

    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        QSqlRecord rec = m_model->record(i);
        QMetaObject::invokeMethod(fetcher, "process", Qt::QueuedConnection,
                                  Q_ARG(QSqlRecord, rec));
    }

    // Now when fetcher knows that last data was provided
    // it will emit end() signal after work will be done
    QMetaObject::invokeMethod(fetcher, "complete");
}


void MainClass::saveCurrentTableAsCSV(const QString& filePath)
{
    QMessageBox msgBox;
    msgBox.setText(filePath);
    msgBox.exec();
    // read entries from DB in loop
    // Save it as CSV using path

     return;
}
