
#include "gys_csvfetcher.h"


GYS::CSVFetcher::CSVFetcher()
    :m_csvFile()
    ,m_in()
    ,m_rowCount(0)
    ,m_rowNext(0)
{
}

GYS::CSVFetcher::~CSVFetcher()
{
    // TODO: close file here
}

bool GYS::CSVFetcher::setFile(const QString &filePath)
{
    // TODO: close previous file here (if such exist)
    // handle I\O errors, trow exception
    bool    flag = true;
    quint32 rows = 0;
    QString tmp;

    m_csvFile.setFileName(filePath);
    // TODO: check that file have valid format
    // does it should check entire file?
    if (!(flag = m_csvFile.open(QIODevice::ReadOnly)))
    {
        // TODO: exception here if file valid, but I\O error occurs
        return flag;
    }

    // Figure out how may rows are present
    m_in.setDevice(&m_csvFile);
    // TODO: add check for file reading errors, trow exception
    while(!m_in.atEnd())
    {
        // TODO: add check for maximum line length,
        // error code should be enough here (file format is wrong)
        tmp = m_in.readLine();
        rows++;
    }

    m_rowNext = 0;
    m_rowCount = rows;

    // TODO: close file here
    // it should be opened again when reading will be needed
    QString header;
    m_in.seek(0);
    header = m_in.readLine();
    qDebug() << "header: " << header;

    return true;
}

quint32 GYS::CSVFetcher::getRowsCount() const
{
    // TODO: check if file was really opened before
    return m_rowCount;
}


quint32 GYS::CSVFetcher::getNextRowIdx() const
{
    return m_rowNext;
}

GYS::DataTable_Map GYS::CSVFetcher::getData(quint32 rowsAmount)
{
    QString             line;
    QStringList         list;
    QRegExp             regx("\\t");
    GYS::DataTable_Map  table;

    // TODO: Error check!

    regx.setCaseSensitivity(Qt::CaseInsensitive);
    regx.setPatternSyntax(QRegExp::RegExp2);

    while (!m_in.atEnd() && rowsAmount--)
    {
        line = m_in.readLine();
        line.remove('\"');
        list = line.split(regx);

        // HACK!!!!
        if (list.size() < 3)
            break;

        // TODO: check for valid string!!!
        // avoid empty and pamela entries
        if (!list.at(1).isEmpty() && !(list.at(1) == "pamela"))
        {
            GYS::DataItem_Pair clientID(GYS::ItemType::NUM_ID, list.at(0));
            GYS::DataItem_Pair clientName(GYS::ItemType::NAME_ID, list.at(1));
            GYS::DataItem_Pair dateAdded(GYS::ItemType::DATE_ADDED, list.at(2));

            GYS::DataRow_Vec rowData = { clientID, dateAdded };

            table.insert(clientName, rowData);
        }

        m_rowNext++;
    }
    // Format:
    // "Client ID" -->"Login"<-- "Added" "Inviter" "Manager (Publishers)"
    // "Manager (Products)" "Manager (News)" "Manager (Audience Development)"
    // "Manager (Banners)" "Status" "MG-Wallet" "Current balance" "Totally spent"
    // "Totally paid" "Last replenishment date"
    return table;
}

GYS::DataTable_Map GYS::CSVFetcher::getData(quint32 startRow, quint32 endRow) const
{
    // TODO
}


GYS::DataRow_Vec GYS::CSVFetcher::findData(const QString& siteID) const
{
    // TODO
}


