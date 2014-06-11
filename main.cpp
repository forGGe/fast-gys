#include <QApplication>
#include <QThread>

#include "mainwindow.h"
#include "gys_controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    GYS::Controller ctrl;
    QThread modelThread;

    // Registering meta types to make possible use new types in queued connections
    qRegisterMetaType< GYS::DataItem_Pair >();
    qRegisterMetaType< GYS::DataRow_Vec >();
    qRegisterMetaType< GYS::DataTable_Map >();

    ctrl.moveToThread(&modelThread);

    // Setup connections UI -> Controller
    QObject::connect(&w, &MainWindow::launching,
                     &ctrl, &GYS::Controller::launch);
    QObject::connect(&w, &MainWindow::exiting,
                     &ctrl, &GYS::Controller::exit);
    QObject::connect(&w, &MainWindow::requestLoadFile,
                     &ctrl, &GYS::Controller::loadFile);
    QObject::connect(&w, &MainWindow::requestUpdateRating,
                     &ctrl, &GYS::Controller::updateRating);
    QObject::connect(&w, &MainWindow::requestUpdateAll,
                     &ctrl, &GYS::Controller::updateAll);
    QObject::connect(&w, &MainWindow::requestFindSimilar,
                     &ctrl, &GYS::Controller::findSimilar);
    QObject::connect(&w, &MainWindow::requestUpdateSimilar,
                     &ctrl, &GYS::Controller::updateSimilar);

    // Setup connections Controller -> UI
    QObject::connect(&ctrl, &GYS::Controller::launched,
                     &w, &MainWindow::lauchDone);
    QObject::connect(&ctrl, &GYS::Controller::fileLoaded,
                     &w, &MainWindow::fileLoadingDone);
    QObject::connect(&ctrl, &GYS::Controller::sendSitesData,
                     &w, &MainWindow::recieveSitesData);
    QObject::connect(&ctrl, &GYS::Controller::sendError,
                     &w, &MainWindow::errorSlot);

    w.show();
    modelThread.start();

    return a.exec();
}
