#include <QRegularExpressionMatch>
#include <QSqlField>

#include "exceptions.h"
#include "filefetcher.h"


GYS::FileFetcher::FileFetcher(const QString &filePath)
    :m_file(filePath) // I really hope that QFile checks if given string isn't empty
    ,m_in()
    ,m_nameRegx("(([\\w-]+)\\.)+[a-zA-Z]{2,3}")
    ,m_emailRegx("[a-zA-Z0-9\\._%+-]+@[a-zA-Z0-9\\.-]+\\.[a-zA-Z]{2,6}")
{
    LOG_ENTRY;

    if (!m_file.open(QIODevice::ReadOnly))
    {
        QString err = QString("Erorr during opening: ") + m_file.errorString();
        throw GYS::Exception(err);
    }

    m_in.setDevice(&m_file);
}

GYS::FileFetcher::~FileFetcher()
{
    LOG_ENTRY;
    m_in.flush();
    m_file.close();
}

GYS::FileFetcher& GYS::FileFetcher::operator >>(QSqlRecord &rval)
{
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

bool GYS::FileFetcher::atEnd() const
{
    return m_in.atEnd();
}
