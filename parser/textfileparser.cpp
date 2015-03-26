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
    :m_nameRegx("(([\\w-]+)\\.)+[a-zA-Z]{2,3}")
    ,m_emailRegx("[a-zA-Z0-9\\._%+-]+@[a-zA-Z0-9\\.-]+\\.[a-zA-Z]{2,6}")
    ,m_in(device)
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
        throw GYS::Exception("Called in the end of the stream!");

    while (!m_in.atEnd()) {
        QString line = m_in.readLine();
        line.remove(QChar('\000'));

        if (!line.isEmpty())
        {
            QRegularExpressionMatch nameMatch = m_nameRegx.match(line);

            if (nameMatch.hasMatch())
            {
                rval.clearValues();
                QRegularExpressionMatch emailMatch = m_emailRegx.match(line);

                if (emailMatch.hasMatch())
                    rval.setValue("email", emailMatch.captured().toLower());

                rval.setValue("name", nameMatch.captured().toLower());
                break;
            }
            else
            {
                LOG_STREAM << line << " is not a site name!";
            }
        }
    }
    return *this;
}

bool TextFileParser::atEnd() const
{
    return m_in.atEnd();
}
