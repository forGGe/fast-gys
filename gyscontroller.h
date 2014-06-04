#ifndef GYSCONTROLLER_H
#define GYSCONTROLLER_H

#include <QObject>
#include <QString>
#include <QMap>

#include "gystypes.h"

namespace GYS {

/**
 * \brief Main and only controller
 *
 * This will interconnect UI and model.
 * Object of this call should run in other thread than UI
 */
class Controller : public QObject
{
    Q_OBJECT
public:
    /**
     * \brief The constructor
     */
    explicit Controller(QObject *parent = 0);

    /**
     * \brief The destructor
     */
    ~Controller();

    /**
     * \defgroup UIConnectivity Connection between UI and Controller
     *
     * @{
     */

signals:
    /**
     * \brief Emits in case of successfull lauching
     */
    void launched();

    /**
     * \brief Emits in case of successfull file loading
     */
    void fileLoaded();

    /**
     * \brief Emits to send bunch of new data to UI
     *
     * \param[in] data Contains site data associated with site id's.
     */
    void sendMainSitesData(QMap< GYS::SiteId, GYS::SiteData > data);

    /**
     * \brief Emits to send ranks of main sites
     *
     * \param[in] ranks Contains ranks for listed sites.
     */
    void sendMainSitesRank(QMap< GYS::SiteId, GYS::SiteRank > ranks);


    /**
     * \brief Emits to send similar sites (and their data) of main site
     *
     * \param[in] mainSite      Site which simiraties should be found
     * \param[in] similarSites  Contains all similar sites and their data
     */
    void sendSimilarSites(GYS::SiteId mainSite, QList< GYS::SiteData > similarSites);

public slots:
    /**
     * \brief Notify about program to launch
     *
     */
    void launch();

    /**
     * \brief Notify about program to exit
     */
    void exit();

    /**
     * \brief Loads a file with a given filename
     */
    void loadFile(QString file_name);

    /**
     * \brief Updates rating of a given site names
     */
    void updateRating(QList< GYS::SiteId > sites);

    /**
     * \brief Updates all records and finds ranks for it
     */
    void updateAll();

    /**
     * \brief Founds similar sites for given sites
     *
     * It will load information from cache, if such exist.
     */
    void findSimilar(QList< GYS::SiteId > sites);

    /**
     * \brief Updates similar sites for given sites
     *
     * It will update cache with newest information.
     */
    void updateSimilar(QList< GYS::SiteId > sites);

    /**
     * }@
     */

    /**
     * \defgroup ModelConnectivity Connection between Model and Controller
     * @{
     */
signals:

public slots:
    /**
     *  }@
     */
};


}
#endif // GYSCONTROLLER_H
