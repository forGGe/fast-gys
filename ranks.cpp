/*!
 * \file    ranks.cpp
 *
 * \author  Max
 * \date    6/24/2014
 *
 */

#include <QtNetwork/QNetworkReply>
#include <QXmlStreamReader>
#include <QStack>
#include "ranks.h"
#include "exceptions.h"
#include <QHostAddress>


GYS::Ranks::Ranks(QObject *parent)
    :QObject(parent)
    ,m_mgr(this)
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
    // TODO: error check

    QUrl url;
    url.setScheme("http");
    url.setHost("data.alexa.com");
    url.setPath("/data");
    url.setQuery(QString("cli=10&dat=snbamz&url=") + siteName);

    qint32 res = 0;
    res = (res << 8) | (qrand() % 20 + 172);
    res = (res << 8) | (qrand() % 225 + 31);
    res = (res << 8) | qrand() % 256;
    res = (res << 8) | qrand() % 255;

    QNetworkRequest tmp(url);
    tmp.setRawHeader("X-FORWARDED-FOR", QHostAddress(res).toString().toUtf8());
    tmp.setRawHeader("Via", QHostAddress(res).toString().toUtf8());
    tmp.setRawHeader("CLIENT-IP", QHostAddress(res).toString().toUtf8());
    m_mgr.get(tmp);
}


void GYS::Ranks::dataReady(QNetworkReply *reply)
{
    LOG_ENTRY;
    // TODO: erorr check and send

    QXmlStreamReader xml;
    QStack< QString > stack;
    QStack< QString > globalRankPath;
    QStack< QString > regionRankPath;
    QStack< QString > hostNamePath;
    bool rankGot = false;

    globalRankPath.push("ALEXA");
    globalRankPath.push("SD");
    globalRankPath.push("POPULARITY");

    regionRankPath.push("ALEXA");
    regionRankPath.push("SD");
    regionRankPath.push("COUNTRY");

    hostNamePath.push("ALEXA");
    hostNamePath.push("SD");

    GYS::DataTable_Map data;
    GYS::DataRow_Vec row;
    GYS::DataItem_Pair siteName;

    if (reply->error())
    {
        LOG_STREAM << "Erorr occurs for url "  << reply->url().toString();
        LOG_STREAM << reply->errorString();
        reply->deleteLater();
        return;
    }

    xml.setDevice(reply);

    while (!xml.atEnd())
    {
        QXmlStreamReader::TokenType type = xml.readNext();
        switch (type)
        {
        case QXmlStreamReader::StartElement:
            stack.push(xml.name().toString());
            break;
        case QXmlStreamReader::EndElement:
            stack.pop();
            break;
        default:
            break;
        }

        if (stack == hostNamePath)
        {
            if (xml.attributes().size() > 2)
                LOG_STREAM << "Host found: " << xml.attributes().at(2).value().toString();
        }
        else if (stack == globalRankPath)
        {
            if (xml.attributes().size() > 1)
            {
                GYS::DataItem_Pair rank;
                QString query;
                QString param("&url=");
                int idx;

                QString domain = xml.attributes().at(0).value().toString().remove('/');
                rank.first = GYS::ItemType::WORLD_RANK;
                rank.second = xml.attributes().at(1).value().toString();
                row.push_back(rank);

                LOG_STREAM << "Domain: " + domain + " rank: " + rank.second;
                rankGot = true;

                // Got site name from URL
                query = reply->url().query();
                idx = query.indexOf(param);
                idx += param.size();

                siteName.first = GYS::ItemType::NAME_ID;
                // To lower case - to be sure
                siteName.second = query.mid(idx).toLower();
            }
        }
        else if (stack == regionRankPath)
        {
            if (xml.attributes().size() > 2)
            {
                GYS::DataItem_Pair localRank;
                GYS::DataItem_Pair region;
                localRank.first = GYS::ItemType::REGION_ID;
                localRank.second = xml.attributes().at(0).value().toString();
                row.push_back(localRank);

                region.first = GYS::ItemType::REGION_RANK;
                region.second = xml.attributes().at(2).value().toString();
                row.push_back(region);

                LOG_STREAM << "Region: " << region.second + " rank: " << localRank.second;

            }
        }

    }

    if (xml.hasError()) {
        LOG_STREAM << xml.errorString();
    }

    if (rankGot)
    {
        data.insert(siteName, row);
        emit ranksReady(data);
    }

    reply->deleteLater();
}
