#ifndef FILEFETCHER_H
#define FILEFETCHER_H

/// \TODO: commentsWW
#include <QSqlRecord>
#include <QString>
#include <QFile>
#include <QSqlField>

#include "fetcher.h"
#include "exceptions.h"

///
/// \brief Simple file fetcher.
///
template < class DataParser >
class FileFetcher : public Fetcher
{
public:
    ///
    /// \brief Constructs fetcher with given file.
    /// \param[in] filePath File path to fetch data from.
    /// \param[in] parent   Parent QObject.
    ///
    FileFetcher(const QString &filePath, QObject *parent = 0);
    ~FileFetcher();

protected:
    ///
    /// \brief Handles start signal from superclass.
    ///
    virtual void handleStart() noexcept;

    ///
    /// \brief Processes already existing record.
    /// Doesn't make sence for File Fetcher, so it contains no implementation.
    /// \param[in] rec Existing record.
    ///
    virtual void handleProcess(QSqlRecord &rec) noexcept;

    ///
    /// \brief Provides empty implementation.
    /// See handleProcess()
    ///
    virtual void handleComplete() noexcept;

private:
    QFile       m_file;     ///< File to parse
    DataParser  m_parser;   ///< Template parser
};

//------------------------------------------------------------------------------
// Public methods

template < class DataParser >
FileFetcher < DataParser >::FileFetcher(const QString &filePath, QObject *parent)
    :Fetcher(parent)
    ,m_file(filePath)
    ,m_parser()
{
    if (!m_file.open(QIODevice::ReadOnly))
    {
        QString err = QString("Erorr during opening: ") + m_file.errorString();

        throw Exception(err);
        return;
    }

    m_parser.setDevice(&m_file);
}

template < class DataParser >
FileFetcher < DataParser >::~FileFetcher()
{
    m_file.close();
}

//------------------------------------------------------------------------------
// Protected methods

template < class DataParser >
void FileFetcher < DataParser >::handleStart() noexcept
{
    try
    {
        QSqlRecord rec;
        rec.append(QSqlField{"name", QVariant::String});
        rec.append(QSqlField{"email", QVariant::String});

        while (!m_parser.atEnd())
        {
            m_parser >> rec;
            emit send(rec);
        }

        emit end();
    }
    catch (Exception &e)
    {
        emit notifyError("Failed to parse data from file", e.what());
        emit end();
    }
    catch (...)
    {
        // Leave any other error as is
        emit notifyError("Unexpected error occurs",
                         QString("First time noted in:\n") +
                         Q_FUNC_INFO);
        emit end();
    }
}

template < class DataParser >
void FileFetcher < DataParser >::handleProcess(QSqlRecord &rec) noexcept
{
    (void) rec;
    // See header for explanation why it is empty
}

template < class DataParser >
void FileFetcher < DataParser >::handleComplete() noexcept
{
    // See header for explanation why it is empty
}



#endif // FILEFETCHER_H
