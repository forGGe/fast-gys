#include "gys_storage.h"
#include "gys_exceptions.h"

GYS::Storage::Storage() noexcept
{
}

GYS::Storage::~Storage()
{
}

void GYS::Storage::addRecords(const GYS::DataTable_Map &records)
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

GYS::DataTable_Map GYS::Storage::getNextRecords(quint64 amount) const
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

void GYS::Storage::resetGetPosition()
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

bool GYS::Storage::addRecord(const GYS::DataItem_Pair   &record,
                             const GYS::DataRow_Vec     &data)
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

bool GYS::Storage::overwriteRecord(const GYS::DataItem_Pair &record,
                                   const GYS::DataRow_Vec   &data)
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

GYS::DataRow_Vec GYS::Storage::getRecordData(const GYS::DataItem_Pair &record) const
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

GYS::DataTable_Map GYS::Storage::getRecordsData(const GYS::DataRow_Vec &records) const
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

bool GYS::Storage::referRecords(const GYS::DataItem_Pair &record,
                                const QString            &refName,
                                const GYS::DataRow_Vec   &references)
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}

GYS::DataRow_Vec GYS::Storage::getRefers(const GYS::DataItem_Pair &record,
                                         const QString            &refname) const
{
    throw GYS::NotImplemented(Q_FUNC_INFO);
}
