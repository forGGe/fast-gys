#ifndef PARSER_H
#define PARSER_H


class QString;
class QSqlRecord;
class QIODevice;

///
/// \brief Defines interface for parsing data in a form of SQL records
///
class Parser
{
public:
    ///
    /// \brief Destroys parser.
    ///
    virtual ~Parser() {}

    ///
    /// \brief Assign specific source device to this parser
    /// \param[in] device IO device with data inside
    ///
    virtual void setDevice(QIODevice * device) = 0;

    ///
    /// \brief Gets data from a device in a form of a SQL record.
    /// This successfully will replace all data in given record.
    /// \param[in,out] rval Container for SQL record.
    /// \return Itself.
    ///
    virtual Parser& operator >>(QSqlRecord &rval) = 0;

    ///
    /// \brief Checks if no more data left to read.
    /// \return true if no more data left.
    ///
    virtual bool atEnd() const = 0;
};

#endif // PARSER_H
