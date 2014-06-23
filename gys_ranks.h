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
    GYS::DataRow_Vec getRanksData(QString siteName);
private:

};

}

#endif // GYS_RANKS_H
