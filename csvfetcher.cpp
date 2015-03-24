#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "exceptions.h"
#include "csvfetcher.h"


GYS::CSVFetcher::CSVFetcher()
    :m_csvFile()
    ,m_in()
    ,m_rowCount(0)
    ,m_rowNext(0)
{
}

GYS::CSVFetcher::~CSVFetcher()
{
    LOG_ENTRY;
    m_in.flush();
    m_csvFile.close();
}

void GYS::CSVFetcher::setFile(const QString &filePath)
{
    LOG_ENTRY;
    const int RowLength = 1024;  // Really, why file will contain
    // entries larger than this?
    quint64   rows = 0;          // Total amount of rows
    QString   tmp;               // Temporary string
    QString   err;               // Contains string describing error


    if (filePath.isEmpty())
        throw GYS::Exception("File path is invalid");

    // Close old one here
    m_in.flush();
    if (m_csvFile.isOpen())
        m_csvFile.close();

    m_csvFile.setFileName(filePath);
    if (!m_csvFile.open(QIODevice::ReadOnly))
    {
        err = QString("Erorr during opening: ") + m_csvFile.errorString();
        throw GYS::Exception(err);
    }

    // Figure out how may rows are present
    m_in.setDevice(&m_csvFile);

    while(!m_in.atEnd())
    {
        tmp = m_in.readLine(RowLength);
        if(tmp.isNull())    {
            err = QString("Reading failed in row: ")
                    + QString::number(rows);
            throw GYS::Exception(err);
        }
        if(tmp.length() == RowLength)   {
            err = QString("File contain too long record in a row: ")
                    + QString::number(rows);
            throw GYS::Exception(err);
        }
        rows++;
    }
    //m_rowNext = 0;
    m_rowCount = rows;
    m_in.seek(0);
}

quint32 GYS::CSVFetcher::getRowsCount() const
{
    LOG_ENTRY;
    // TODO: check if file was really opened before
    return m_rowCount;
}


quint32 GYS::CSVFetcher::getNextRowIdx() const
{
    LOG_ENTRY;
    return m_rowNext;
}

GYS::DataTable_Map GYS::CSVFetcher::getData(quint32 rowsAmount)
{
    LOG_ENTRY;
    // TODO: improve error checks

    try
    {
        QString             line;
        GYS::DataTable_Map  table;

        // TODO: Error check & exceptions!

        while (!m_in.atEnd() && rowsAmount--)
        {
            line = m_in.readLine();
            line.remove(QChar('\000'));

            GYS::DataItem_Pair clientID;
            GYS::DataItem_Pair clientEm;

            if (!line.isEmpty())
            {
                QRegularExpression nameRegx("(([\\w-]+)\\.)+[a-zA-Z]{2,3}");
                QRegularExpressionMatch nameMatch = nameRegx.match(line);

                if (nameMatch.hasMatch())
                {
                    clientID.first = GYS::ItemType::NAME_ID;
                    // Only lower case is allowed
                    clientID.second = nameMatch.captured().toLower();

                    GYS::DataRow_Vec rowData =
                    {
                        { GYS::ItemType::NUM_ID, nameMatch.captured() },
                        // TODO: For now it doesn't needed
                        // should be removed
                        // { GYS::ItemType::DATE_ADDED, list.at(2) },
                    };

                    //table.insert(clientID, rowData);

                    QRegularExpression nameRegxEm("[a-zA-Z0-9\\._%+-]+@[a-zA-Z0-9\\.-]+\\.[a-zA-Z]{2,6}");
                    QRegularExpressionMatch nameMatchEm = nameRegxEm.match(line);

                    if (nameMatchEm.hasMatch())
                    {
                        clientEm.first = GYS::ItemType::CONTACT_EMAIL;
                        clientEm.second = nameMatchEm.captured().toLower();

                        rowData.push_back(clientEm);

                        table.insert(clientID, rowData);
                    }

                    if (!nameMatchEm.hasMatch())
                    {
                        if (nameMatch.hasMatch())   {
                            clientID.first = GYS::ItemType::NAME_ID;
                            clientID.second = nameMatch.captured().toLower();

                            GYS::DataRow_Vec rowData =
                            {
                              { GYS::ItemType::NUM_ID, nameMatch.captured() },
                            };

                            table.insert(clientID, rowData);
                        }
                    }
                }
                else
                {
                    LOG_STREAM << line << " is not a site name!";
                }
            }

            m_rowNext++;
        }

        return table;
    }
    catch (...)
    {
        throw;
    }
}

GYS::DataTable_Map GYS::CSVFetcher::getData(quint32 startRow, quint32 endRow) const
{
    LOG_ENTRY;

    throw GYS::NotImplemented(Q_FUNC_INFO);
    // TODO
}


GYS::DataRow_Vec GYS::CSVFetcher::findData(const QString& siteID) const
{
    LOG_ENTRY;

    throw GYS::NotImplemented(Q_FUNC_INFO);
    // TODO
}


