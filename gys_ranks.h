/*!
 * \file    gys_ranks.cpp
 *
 * \author  Max
 * \date    6/24/2014
 *
 */
#ifndef GYS_RANKS_H
#define GYS_RANKS_H

#include "gys_types.h"


namespace GYS
{

class Ranks : public QObject
{
    Q_OBJECT
public:
    Ranks();
    ~Ranks();

    // Gets rank data for given site name
    void getRanksData(QString siteName);

signals:
    // Signals when error occurs
    void errorOccurs(QString err);

    // Signals when ranks is ready
    void ranksReady(GYS::DataRow_Vec ranks);
private:

};

}

#endif // GYS_RANKS_H
