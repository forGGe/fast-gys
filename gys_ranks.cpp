/*!
 * \file    gys_ranks.cpp
 *
 * \author  Max
 * \date    6/24/2014
 *
 */


#include "gys_ranks.h"
#include "gys_exceptions.h"

GYS::Ranks::Ranks()
{
    LOG_ENTRY;

}

GYS::Ranks::~Ranks()
{
    LOG_ENTRY;

}


GYS::DataRow_Vec GYS::Ranks::getRanksData(QString siteName)
{
    LOG_ENTRY;
    throw GYS::NotImplemented(Q_FUNC_INFO);
}
