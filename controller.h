#ifndef GYSCONTROLLER_H
#define GYSCONTROLLER_H

#include <QObject>
#include <QString>
#include <QMap>

#include "types.h"
#include "storage.h"
#include "ranks.h"

namespace GYS
{

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
    void sendSitesData(GYS::DataTable_Map data);

    /**
     * \brief Sends error
     *
     * \param[in] descr Description of an error
     */
    void sendError(QString descr);

    /**
     * \brief Signals that all data was deleted
     */
    void allDataDeleted();

public slots:
    /**
     * \brief Loads a file with a given filename
     */
    void loadFile(QString filePath);

    /**
     * \brief Updates rating of a given site names
     */
    void updateRating(GYS::Sites_List sites);

    /**
     * \brief Updates all records and finds ranks for it
     */
    void updateAll();

    /**
     * \brief Founds similar sites for given sites
     *
     * It will load information from cache, if such exist.
     */
    void findSimilar(GYS::Sites_List sites);

    /**
     * \brief Updates similar sites for given sites
     *
     * It will update cache with newest information.
     */
    void updateSimilar(QList< QString > sites);

    /**
     * }@
     */

    /**
     * \defgroup GeneralConnectivily Connection between Controller and environment
     * @{
     */
signals:

public slots:
    /**
     * \brief Consumes new data that was added outside of UI
     *
     * It will try to update missing data, if such exist
     *
     * \param[in] data
     */
    void consumeData(GYS::DataTable_Map data);

    /**
     * \brief Slot to recieve an error
     * \param[in] String that describes an error
     */
    void errorSlot(QString err);

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
     * \brief Deleting all from Data Base and table
     */
    void deleteAllFromDatabase();


    /**
     * }@
     */
private:
    GYS::Storage m_storage; /**< Storage to contain fetched data */
    GYS::Ranks   m_ranks;   /**< To retrieve ranks */
};


}
#endif // GYSCONTROLLER_H
