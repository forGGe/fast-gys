#include "gys_exceptions.h"
#include "gys_csvfetcher.h"


GYS::CSVFetcher::CSVFetcher()
    :m_csvFile()
    ,m_in()
    ,m_rowCount(0)
    ,m_rowNext(0)
    ,m_nextChar(0)
{
}

GYS::CSVFetcher::~CSVFetcher() noexcept
{
    LOG_ENTRY;
    m_in.flush();
    m_csvFile.close();
}

void GYS::CSVFetcher::setFile(const QString &filePath)
{
    LOG_ENTRY;
    try
    {
        const int RowLength = 1024;  // Really, why file will contain
        // entries larger than this?
        const int valuesCount = 15;  // Count of valid values in one row

        quint64   rows = 0;          // Total amount of rows
        quint64   headerSize;        // Size of header line (1st line in file)
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
        headerSize = 0; // Use it as a flag as well
        while(!m_in.atEnd())
        {
            int count;
            tmp = m_in.readLine(RowLength);
            if (tmp.isNull())
            {
                err = QString("Reading failed in row: ")
                        + QString::number(rows);
                throw GYS::Exception(err);
            }
            if (tmp.length() == RowLength)
            {
                err = QString("File contain too long record in row: ")
                        + QString::number(rows);
                throw GYS::Exception(err);
            }
            if ((count = tmp.count(QRegExp("\"[^\\s]"))) != valuesCount)
            {
                err = QString("Row contains invalid values count. "
                              "Expected: ") + QString::number(valuesCount) +
                        QString("Got: ") + QString::number(count) +
                        QString(" Row num: ") + QString::number(rows);
                throw GYS::Exception(err);
            }
            if (!headerSize)
                headerSize = m_in.pos();

            rows++;
        }

        m_rowNext = 0;
        m_rowCount = rows;
        // Will start reading from next character folowing header
        m_nextChar = headerSize;
        m_in.seek(m_nextChar);

    }
    catch (...)
    {
        m_in.flush();
        m_csvFile.close();
        m_nextChar = 0;
        m_rowNext = 0;
        m_rowCount = 0;
        throw;
    }
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

        // TODO: Error check!

        while (!m_in.atEnd() && rowsAmount--)
        {
#if 1

            line = m_in.readLine();
            line.remove(QChar('\000'));

            QStringList list = line.split(QChar('\t'));
            for (auto it = list.begin(); it != list.end(); ++it)
                it->remove(QChar('\"'));

            GYS::DataItem_Pair clientID;

            if (!list.at(1).isEmpty() && list.at(1) != "pamela")
            {
                clientID.first = GYS::ItemType::NAME_ID;
                clientID.second = list.at(1);

                GYS::DataRow_Vec rowData =
                {
                  { GYS::ItemType::NUM_ID, list.at(0) },
                  { GYS::ItemType::DATE_ADDED, list.at(2) },
                };

                table.insert(clientID, rowData);
            }

#else

            GYS::DataItem_Pair  clientID;
            GYS::DataRow_Vec    rowData;

            bool    nameFound = false;
            bool    lineParsed = false;
            int     idx = 0;

            while (idx < 15)
            {
                GYS::DataItem_Pair item;
                GYS::ItemType      type;
                QString            itemStr;

                // Left quote
                int left = line.indexOf(QChar('\"'));
                // Right quote
                int right = line.indexOf(QChar('\"'), left + 1);
                int n = right - left;

                if (right == line.length() - 1)
                    lineParsed = true;

                itemStr = line.left(n + 1);
                line.remove(left, n + 1);

                itemStr.remove("\"");
                itemStr = itemStr.simplified();
                line = line.simplified();

                if (!itemStr.isNull() && !itemStr.isEmpty()) {
                    switch (idx)
                    {
                    case 0:
                        type = GYS::ItemType::NUM_ID;
                        break;
                    case 1:
                        type = GYS::ItemType::NAME_ID;
                        break;
                    case 2:
                        type = GYS::ItemType::DATE_ADDED;
                        break;
                        // Ignore for a while
                    default:
                        type = GYS::ItemType::INVALID_TYPE;
                        break;
                    }
                    if (type == GYS::ItemType::NAME_ID)
                    {
                        clientID.first = type;
                        clientID.second = itemStr;
                        nameFound = true;
                    }
                    else if (type != GYS::ItemType::INVALID_TYPE)
                    {
                        item.first = type;
                        item.second = itemStr;
                        rowData.push_back(item);
                    }
                }
                idx++;
            }

            if (nameFound)
            {
                table.insert(clientID, rowData);
            }
#endif
            m_rowNext++;
        }

        // Format:
        // "Client ID" -->"Login"<-- "Added" "Inviter" "Manager (Publishers)"
        // "Manager (Products)" "Manager (News)" "Manager (Audience Development)"
        // "Manager (Banners)" "Status" "MG-Wallet" "Current balance" "Totally spent"
        // "Totally paid" "Last replenishment date"

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


