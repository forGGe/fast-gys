#ifndef GYS_STORAGE_H
#define GYS_STORAGE_H

#include <QObject>
#include <QFile>
#include <QSqlDatabase>

#include "types.h"

namespace GYS
{

// To store data on disk, manages cache
class Storage
{
public:
    Storage();
    ~Storage();

    // Adds records and their data using map key as record key
    void addRecords(const GYS::DataTable_Map &records);

    // Updates records with given info
    // Records must be present inside storage
    void updateRecords(const GYS::DataTable_Map &records);

    // Gets next bunch of records with given amount
    // It will start reading records from storage start,
    // each next call will return records placed after last previous record got
    // Table contains less or 0 elements, if there are no records left
    GYS::DataTable_Map getNextRecords(quint64 amount);

    // Resets get position, so next call to getNextRecords() will start from
    // beginning of storage
    void resetGetPosition();

    // Flushes all caches into disk storage
    void flush();

    // Adds record and extends missing data if it was previously created
    // return true if record already existed
    bool addRecord(const GYS::DataItem_Pair &record, const GYS::DataRow_Vec &data);

    // Overwrites record with provided data, removing data that wasn't provided
    // if record wasn't existed - it will create new one
    // return true if record already existed
    bool overwriteRecord(const GYS::DataItem_Pair &record, const DataRow_Vec &data);

    // Removes records and its data using key
    // return true if found
    bool removeRecord(const GYS::DataItem_Pair &record);

    // Gets data associated with given record
    // Default contructed vector returned if record wasn't found
    GYS::DataRow_Vec getRecordData(const GYS::DataItem_Pair &record) const;

    // Gets data associated with given records
    // Default contructed map returned if no records found
    // All records must have the same type
    GYS::DataTable_Map getRecordsData(const GYS::DataRow_Vec &records) const;

    // Create a named references in the given record
    // This is useful to imply one-to-many relation
    // All records must have the same type
    // returns true if record found and updated with references
    bool referRecords(const GYS::DataItem_Pair  &record,
                      const QString             &refName,
                      const GYS::DataRow_Vec    &references);

    // Gets references with given name from given record
    // Default contructed vector returned if record wasn't found
    // or no such reference exist
    GYS::DataRow_Vec getRefers(const GYS::DataItem_Pair &record,
                               const QString            &refname) const;

    // Clears storage, deleting all items
    void clearStorage();

private:
    QSqlDatabase m_db;  // Database to hold values
    quint64      m_row; // Current row that storage operates on
};


}


#endif // GYS_STORAGE_H
