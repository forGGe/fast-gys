#ifndef GYSTYPES_H
#define GYSTYPES_H

#include <QtGlobal>
#include <QPair>
#include <QVector>
#include <QString>

namespace GYS {
/**
 * \brief Type of site ID, describing a unique site
 */
using SiteId		= QString;

/**
 * \brief Type of site rank
 */
using SiteRank		= qint32;

/**
 * \brief Type of site complete data
 */
using SiteData		= QPair< QVector< QString >, GYS::SiteRank>;


//static const SiteId SITE_

}

#endif // GYSTYPES_H
