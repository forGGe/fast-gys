#ifndef GYSTYPES_H
#define GYSTYPES_H

#include <QtGlobal>
#include <QPair>
#include <QVector>
#include <QString>
#include <QDebug>

namespace GYS
{

// TODO: make it familiar with next stuff:
// "Client ID" "Login" "Added" "Inviter" "Manager (Publishers)"
// "Manager (Products)" "Manager (News)" "Manager (Audience Development)"
// "Manager (Banners)" "Status" "MG-Wallet" "Current balance" "Totally spent"
// "Totally paid" "Last replenishment date"

/**
 * \brief Enum describing types of item that could exist
 */
enum class ItemType
{
    /** Obvious, to represent invalid type */
    INVALID_TYPE,

    /** Specifies that given item is an associated key for main key defined */
    ASSOC_KEY,

    /** Type to represent client id, numerical form */
    NUM_ID,

    /** Type to represent client id, string form (site name actually) */
    NAME_ID,

    /** Type to represent client e-mail, string form */
    CONTACT_EMAIL,

    /** Type to represent date when site was added */
    DATE_ADDED,

    /** Type to represent host country of the site */
    REGION_ID,

    /** Type to represent world rank of the site */
    WORLD_RANK,

    /** Rank of the site in most popular region */
    REGION_RANK,

    /** Last date when rank was updated */
    DATE_UPDATED,
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

/**
 * \brief Type used to store site names without any additional data
 */
using Sites_List     = QStringList;

}

#define LOG_ENTRY do { qDebug() << "Entered: " << Q_FUNC_INFO; } while(0)
#define LOG_STREAM qDebug()

// Declarations to make possible use new types in queued connections
Q_DECLARE_METATYPE(GYS::DataItem_Pair)
Q_DECLARE_METATYPE(GYS::DataRow_Vec)
Q_DECLARE_METATYPE(GYS::DataTable_Map)
Q_DECLARE_METATYPE(GYS::Sites_List)


#endif // GYSTYPES_H
