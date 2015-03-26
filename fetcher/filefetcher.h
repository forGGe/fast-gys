#ifndef FILEFETCHER_H
#define FILEFETCHER_H

#include <QSqlRecord>
#include <QString>
#include <QFile>
/// \TODO: comments
/// \TODO: better hierarchy here

#include "fetcher.h"
#include "exceptions.h"

///
/// \brief Simple file fetcher
///
template < class DataParser >
class FileFetcher : public Fetcher
{
public:
    FileFetcher(const QString &filePath, QObject *parent = 0);
    ~FileFetcher();

protected:
    virtual void handleStart();

private:
    QFile       m_file;
    DataParser  m_parser;
};

template < class DataParser >
FileFetcher < DataParser >::FileFetcher(const QString &filePath, QObject *parent)
    :Fetcher(parent)
    ,m_file(filePath)
    ,m_parser()
{
    if (!m_file.open(QIODevice::ReadOnly))
    {
        QString err = QString("Erorr during opening: ") + m_file.errorString();
        throw GYS::Exception(err);
        return;
    }

    m_parser.setDevice(&m_file);
}

template < class DataParser >
FileFetcher < DataParser >::~FileFetcher()
{
    m_file.close();
}

template < class DataParser >
void FileFetcher < DataParser >::handleStart()
{
    QSqlRecord rec;
    rec.append(QSqlField{"name", QVariant::String});
    rec.append(QSqlField{"email", QVariant::String});

    while (!m_parser.atEnd())
    {
        m_parser >> rec;
        emit send(rec);
    }
}

#endif // FILEFETCHER_H
