#ifndef GYSTYPES_H
#define GYSTYPES_H

#include <QtGlobal>
#include <QPair>
#include <QVector>
#include <QString>
#include <QDebug>

namespace GYS {
/**
 * \brief Type of site ID, describing a unique site
 */
using SiteId        = QString;

/**
 * \brief Type of site rank
 */
using SiteRank      = qint32;

/**
 * \brief Cell array
 */
using CellArray     = QVector< QString >;

/**
 * \brief Type of site complete data
 */
using SiteData      = QPair< GYS::CellArray, GYS::SiteRank>;

}

#define TRACE_ENTRY do { qDebug() << "Entered: " << __func__; } while(0)


#endif // GYSTYPES_H
