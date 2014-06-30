/*!
 * \file    gys_ranks.cpp
 *
 * \author  Max
 * \date    6/24/2014
 *
 */
#ifndef GYS_RANKS_H
#define GYS_RANKS_H

#include <QtNetwork/QNetworkAccessManager>

#include "gys_types.h"


namespace GYS
{

class Ranks : public QObject
{
    Q_OBJECT
public:
    Ranks(QObject *parent);
    ~Ranks();

    // Gets rank data for given site name
    void getRanksData(QString siteName);

signals:
    // Signals when error occurs
    void errorOccurs(QString err);

    // Signals when ranks is ready
    void ranksReady(GYS::DataRow_Vec ranks);

private slots:
    // To recieve reply from network manager
    void dataReady(QNetworkReply *reply);

private:
    // To maintaing requests
    QNetworkAccessManager *m_mgr;

};

}

#endif // GYS_RANKS_H
