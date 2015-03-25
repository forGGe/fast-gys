#ifndef GYS_CSVFETCHER_H
#define GYS_CSVFETCHER_H

#include <QtGlobal>
#include <QFile>
#include <QSqlRecord>
#include <QRegularExpression>

/// TODO: comments

#include "types.h"

class QString;

namespace GYS
{

///
/// \brief Fetches records from file
///
class FileFetcher
{
public:
    ///
    /// \brief Constructs file fetcher using file locating at given path.
    /// \param[in] filePath A file which contains records.
    ///
    FileFetcher(const QString& filePath);
    ~FileFetcher();

    ///
    /// \brief Sets file to operate on.
    /// \param[in] filePath Path of the file containing records.
    ///
    void setFile(const QString& filePath);

    ///
    /// \brief Fetches data from file to a SQL record.
    /// This successfully will replace all data in given record
    /// \param[in,out] rval Container for SQL record.
    /// \return Itself.
    ///
    FileFetcher& operator >>(QSqlRecord &rval);

    ///
    /// \brief Checks if fetcher reached end of the file.
    /// \return true if in the end of the file.
    ///
    bool atEnd() const;

private:
    QFile         m_file;       ///< Target file
    QTextStream   m_in;         ///< Stream for reading

    /// @{
    /// Regex used to get email and sitenames
    const QRegularExpression  m_nameRegx;
    const QRegularExpression  m_emailRegx;
    /// }@

};

}


#endif // GYS_CSVFETCHER_H
