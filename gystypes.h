#ifndef GYSTYPES_H
#define GYSTYPES_H

#include <QtGlobal>
#include <QPair>
#include <QVector>
#include <QString>
#include <QDebug>

namespace GYS {

/**
 * \brief Enum describing types of item that could exist
 */
enum class ItemType {
    /**
     * Specifies that given item is a parent key for accociated data
     */
    PARENT_KEY,

    /**
     * Type to represent site name
     */
    NAME_ID,

    /**
     * Type to represent host country of the site
     */
    HOST_COUNTRY,

    /**
     * Type to represent world rank of the site
     */
    WORLD_RANK,

    /**
     * Region where given site has most visitors
     */
    REGION_ID,

    /**
     * Rank of the site in most popular region
     */
    REGION_RANK,
};

/**
 * \brief Type to represent a single data item with specified type
 */
using DataItem_Pair  = QPair< GYS::ItemType, QString >;

/**
 * \brief Type to represent a row of a data
 */
using DataRow_Vec    = QVector< GYS::DataItem_Pair >;

/**
 * \brief Type to represent a table with data, indexed by needed key
 */
using DataTable_Map  = QMap< GYS::DataItem_Pair, GYS::DataRow_Vec >;


}

#define TRACE_ENTRY do { qDebug() << "Entered: " << __func__; } while(0)


#endif // GYSTYPES_H
