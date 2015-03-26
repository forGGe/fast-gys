#include <QApplication>
#include <QSqlField>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mw;
    mw.show();

    return a.exec();
}
