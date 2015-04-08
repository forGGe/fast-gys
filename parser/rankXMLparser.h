#ifndef RANK_XML_PARSER_H
#define RANK_XML_PARSER_H

#include <QXmlStreamReader>

#include "parser.h"

///
/// \brief Parses XML input returned from rank sources.
///
class RankXMLParser : public Parser
{
public:
    ///
    /// \brief Constructs parser without device attached.
    ///
    RankXMLParser();

    ///
    /// \brief Constructs parser for given device.
    /// \param[in] device Device containing XML data to parse.
    ///
    RankXMLParser(QIODevice *device);

    ///
    /// \brief Destroys parser.
    ///
    virtual ~RankXMLParser();

    ///
    /// \brief Assign specific source device to this parser.
    /// \param[in] device IO device with data inside.
    ///
    virtual void setDevice(QIODevice * device);

    ///
    /// \brief Gets data from a device in a form of a SQL record.
    /// This successfully will replace all data in given record.
    /// \param[in,out] rval Container for SQL record.
    /// \return Itself.
    ///
    virtual Parser& operator >>(QSqlRecord &rval);

    ///
    /// \brief Checks if no more data left to read.
    /// \return true if no more data left.
    ///
    virtual bool atEnd() const;

private:
    QXmlStreamReader m_xml; ///< Allows read XML in a form of stream

};


#endif // RANK_XML_PARSER_H

