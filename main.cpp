#include <QApplication>
#include "mainclass.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainClass *mc = new MainClass;
    mc->setParent(&a);

    return a.exec();
}
