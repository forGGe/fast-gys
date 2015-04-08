#ifndef TEXTFILEPARSER_H
#define TEXTFILEPARSER_H

#include <QTextStream>
#include <QRegularExpression>

/// TODO: comments

#include "parser.h"


class QSqlRecord;
class QIODevice;

///
/// \brief Parsers records from file
///
class TextFileParser : public Parser
{
public:
    ///
    /// \brief Construct parser wihout device attached
    ///
    TextFileParser();

    ///
    /// \brief Constructs parser using device with source data.
    /// \param[in] device A device which contains records.
    ///
    TextFileParser(QIODevice *device);

    ///
    /// \brief Destroys parser, flushing stream.
    ///
    virtual ~TextFileParser();

    ///
    /// \brief Assign specific source device to this parser
    /// \param[in] device IO device with data inside
    ///
    virtual void setDevice(QIODevice * device);

    ///
    /// \brief Gets data from a device in a form of a SQL record.
    /// This successfully will replace all data in given record.
    /// \param[in,out] rval Container for SQL record.
    /// \return Itself.
    ///
    virtual TextFileParser& operator >>(QSqlRecord &rval);

    ///
    /// \brief Checks if no more data left to read.
    /// \return true if no more data left.
    ///
    virtual bool atEnd() const;

private:
    /// @{
    /// Regex used to get email and sitenames
    const QRegularExpression m_nameRegx;
    const QRegularExpression m_emailRegx;
    /// }@

    QTextStream m_in; ///< Stream for reading
};



#endif // TEXTFILEPARSER_H
