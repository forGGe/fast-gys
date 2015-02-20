#ifndef GYS_CSVFETCHER_H
#define GYS_CSVFETCHER_H

#include <QtGlobal>
#include <QFile>

#include "types.h"

class QString;

namespace GYS
{

/**
 * \brief Fetches data present in CVS file
 */
class CSVFetcher
{
public:
    CSVFetcher();
    ~CSVFetcher();

    /**
     * \brief Sets file to operate on
     *
     * \param[in] filePath Path of the CSV file.
     * \return status of the operation.
     */
    void setFile(const QString& filePath);

    /**
     * \brief Gets total amount of rows in the file
     *
     * \retval positive  Total amount of rows of the data present in the file.
     * \retval 0         File wasn't set or recognized.
     */
    quint32 getRowsCount() const;

    /**
     * \brief Gets next row that will be returned on demand
     *
     * \return Next row that will be returned.
     */
    quint32 getNextRowIdx() const;

    /**
     * \brief Gets table with the most of requested rows amount,
     *        started from the next row for reading
     *
     * It will try to return as many rows as possible (not more than specified)
     * even if actual rows amount in file is less than expected.
     *
     * \param[in] amount Amount of rows that should be returned.
     * \return Table containing data.
     */
    GYS::DataTable_Map getData(quint32 rowsAmount);

    /**
     * \brief Gets table from start row till end row
     *
     * If amount of data is less than queried then returned table will contain
     * as many rows as possible.
     *
     * \param[in] startRow Start row, from which data should be get.
     * \param[in] endRow   If more than zero this is a end row,
     *                     till which data should be get. If zero -
     *                     end row will be the last row in a file.
     * \return Table containing data.
     */
    GYS::DataTable_Map getData(quint32 startRow, quint32 endRow) const;


    /**
     * \brief Finds data associated with particular site ID
     *
     * \param[in] siteID Id of the site to lookup
     *
     * \return Row containing needed data, or empty row if data wasn't found
     */
    GYS::DataRow_Vec findData(const QString& siteID) const;

private:
    QFile       m_csvFile;    /**< Target file */
    QTextStream m_in;         /**< Stream for reading */
    quint64     m_rowCount;   /**< Amount of rows inside a file */
    quint64     m_rowNext;    /**< Next row for readin, excluding header */
    quint64     m_nextChar;   /**< Next char to read. Need here to seek streams */
};

}


#endif // GYS_CSVFETCHER_H
