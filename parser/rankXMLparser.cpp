#include <QXmlStreamReader>
#include <QVector>
#include <QPair>
#include <QSqlRecord>
#include <QString>
#include <QVariant>

#include "rankXMLparser.h"


RankXMLParser::RankXMLParser()
    :m_xml()
{

}

RankXMLParser::RankXMLParser(QIODevice *device)
    :m_xml(device)
{

}

RankXMLParser::~RankXMLParser()
{

}

void RankXMLParser::setDevice(QIODevice *device)
{
    m_xml.setDevice(device);
}

Parser& RankXMLParser::operator >>(QSqlRecord &rval)
{
    // TODO: this defines desired XML structure, move it elsewhere and
    // add some additional XML structures
    QVector< QPair< QString, QPair< QString, QString >>> dataPairs =
    {
        // Target element: SD, target attribute: HOST,
        // will be placed at column "name"
        { "name",       { "SD",         "HOST" }},

        // Target element: POPULARITY, target attribute: TEXT,
        // will be placed at column "rank"
        { "rank",       { "POPULARITY", "TEXT" }},

        // Target element: COUNTRY, target attribute: RANK,
        // will be placed at column "local_rank"
        { "local_rank", { "COUNTRY",    "RANK" }},

        // Target element: COUNTRY, target attribute: CODE,
        // will be placed at column "country"
        { "country",    { "COUNTRY",    "CODE" }}
    };

    while (!m_xml.atEnd())
    {
        QXmlStreamReader::TokenType type = m_xml.readNext();
        if (type == QXmlStreamReader::StartElement)
        {
            const QString& elem = m_xml.name().toString();
            // Compare input elements and their attributes with
            // defined above.
            for (const auto &p : dataPairs)
            {
                // Target element and attribute
                const auto &target = p.second;
                if (elem == target.first)
                {
                    QStringRef ref = m_xml.attributes().value(target.second);
                    if (!ref.isEmpty())
                    {
                        rval.setValue(p.first, QVariant(ref.toString()));
                    }
                }
            }
        }
    }

    if (m_xml.hasError()) {
    //    LOG_STREAM << xml.errorString();
    }

    return *this;
}

bool RankXMLParser::atEnd() const
{
    return m_xml.atEnd();
}
