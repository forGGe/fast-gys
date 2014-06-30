/*!
 * \file    gys_ranks.cpp
 *
 * \author  Max
 * \date    6/24/2014
 *
 */

#include <QtNetwork/QNetworkReply>

#include "gys_ranks.h"
#include "gys_exceptions.h"

GYS::Ranks::Ranks(QObject *parent)
    :QObject(parent)
    ,m_mgr(parent)
{
    LOG_ENTRY;

    QObject::connect(&m_mgr, &QNetworkAccessManager::finished,
                     this, &GYS::Ranks::dataReady);

}

GYS::Ranks::~Ranks()
{
    LOG_ENTRY;

}


void GYS::Ranks::getRanksData(QString siteName)
{
    LOG_ENTRY;
    throw GYS::NotImplemented(Q_FUNC_INFO);
}


void GYS::Ranks::dataReady(QNetworkReply *reply)
{
    LOG_ENTRY;

}
