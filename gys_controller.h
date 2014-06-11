#ifndef GYSCONTROLLER_H
#define GYSCONTROLLER_H

#include <QObject>
#include <QString>
#include <QMap>

#include "gys_types.h"

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
    explicit Controller(QObject *parent = 0) noexcept;

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
    void sendSitesData(GYS::DataTable_Map data);

    /**
     * \brief Sends error
     *
     * \param[in] descr Description of an error
     */
    void sendError(QString descr);

public slots:
    /**
     * \brief Notify about program to launch
     *
     */
    void launch() noexcept;

    /**
     * \brief Notify about program to exit
     */
    void exit() noexcept;

    /**
     * \brief Loads a file with a given filename
     */
    void loadFile(QString filePath) noexcept;

    /**
     * \brief Updates rating of a given site names
     */
    void updateRating(QList< QString > sites) noexcept;

    /**
     * \brief Updates all records and finds ranks for it
     */
    void updateAll() noexcept;

    /**
     * \brief Founds similar sites for given sites
     *
     * It will load information from cache, if such exist.
     */
    void findSimilar(QList< QString > sites) noexcept;

    /**
     * \brief Updates similar sites for given sites
     *
     * It will update cache with newest information.
     */
    void updateSimilar(QList< QString > sites) noexcept;

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
