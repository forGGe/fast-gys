#include <QApplication>

#include "mainwindow.h"
#include "gyscontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    GYS::Controller ctrl(&w);

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

    w.show();

    return a.exec();
}
