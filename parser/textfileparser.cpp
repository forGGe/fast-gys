#include <QRegularExpressionMatch>
#include <QSqlField>
#include <QSqlRecord>

#include "exceptions.h"
#include "textfileparser.h"
#include "types.h"

TextFileParser::TextFileParser()
    :TextFileParser(nullptr)

{
    LOG_ENTRY;
}

TextFileParser::TextFileParser(QIODevice *device)
    :m_nameRegx("\\s(([\\w-]+)\\.)+[a-zA-Z]{2,3}\\s")
    ,m_emailRegx("[a-zA-Z0-9\\._%+-]+@[a-zA-Z0-9\\.-]+\\.[a-zA-Z]{2,6}")
    ,m_in(device)
    ,m_current()
    ,m_it()
{
    LOG_ENTRY;
}

TextFileParser::~TextFileParser()
{
    LOG_ENTRY;
    m_in.flush();
}

void TextFileParser::setDevice(QIODevice *device)
{
    m_in.setDevice(device);
}

TextFileParser& TextFileParser::operator >>(QSqlRecord &rval)
{
    LOG_ENTRY;

    if (m_in.atEnd())
        throw Exception("Called in the end of the stream!");

#if 0
    // Populate string
    while (!m_in.atEnd()) {
        if (!m_it.hasNext()) {
            m_current = m_in.readLine();
            m_current.remove(QChar('\000'));

            if (m_current.isEmpty()) {
                continue;
            }

            m_it = m_nameRegx.globalMatch(m_current);
        }

        if (m_it.hasNext()) {
            QRegularExpressionMatch nameMatch = m_it.next();
            if (nameMatch.hasMatch())
            {
                rval.clearValues();
                QRegularExpressionMatch emailMatch = m_emailRegx.match(m_current);

                if (emailMatch.hasMatch())
                    rval.setValue("email", emailMatch.captured().toLower());

                // Note that whitespace at start must be removed
                rval.setValue("name", nameMatch.captured().toLower().trimmed());

                break;
            }
        }
    }


#else
    while (!m_in.atEnd())
    {
        QString line = m_in.readLine();
        line.remove(QChar('\000'));

        if (!line.isEmpty())
        {

            auto nameMatch = m_nameRegx.match(line);


            // Do not include records without domain names
            if (nameMatch.hasMatch())
            {
                rval.clearValues();
                auto emailMatch = m_emailRegx.match(line);

                if (emailMatch.hasMatch())
                    rval.setValue("email", emailMatch.captured().toLower());

                // Note that whitespace at start and the end must be removed
                rval.setValue("name", nameMatch.captured().toLower().trimmed());
                break;
            }
        }
    }
#endif
    return *this;
}

bool TextFileParser::atEnd() const
{
    return m_in.atEnd();
}
