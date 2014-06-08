
#include "gys_csvfetcher.h"


GYS::CSVFetcher::CSVFetcher()
    :m_csvFile()
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
    QTextStream in(&m_csvFile);
    // TODO: add check for file reading errors, trow exception
    while(!in.atEnd())
    {
        // TODO: add check for maximum line length,
        // error code should be enough here (file format is wrong)
        tmp = in.readLine();
        rows++;
    }

    m_rowNext = 0;
    m_rowCount = rows;

    return m_csvFile.seek(0);
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
    QTextStream         in(&m_csvFile);
    QString             header;
    QString             line;
    QStringList         list;
    QRegExp             regx("\\t");
    GYS::DataTable_Map  table;

    regx.setCaseSensitivity(Qt::CaseInsensitive);
    regx.setPatternSyntax(QRegExp::RegExp2);

    header = in.readLine();


    while (!in.atEnd() && rowsAmount--)
    {
        line = in.readLine();
        line.remove(QRegExp("\""));
        list = line.split(regx);

        // TODO
        // avoid empty and pamela entries
        if (!list.at(1).isEmpty() && !(list.at(1) == "pamela"))
        {
            GYS::DataItem_Pair clientID(GYS::ItemType::NUM_ID, list.at(0));
            GYS::DataItem_Pair clientName(GYS::ItemType::NAME_ID, list.at(1));
            GYS::DataItem_Pair dateAdded(GYS::ItemType::DATE_ADDED, list.at(2));

            GYS::DataRow_Vec rowData = { clientID, dateAdded };

            table.insert(clientName, rowData);

        }
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


