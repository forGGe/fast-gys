#ifndef GYSTYPES_H
#define GYSTYPES_H

#include <QDebug>

#define LOG_ENTRY do { qDebug() << "Entered: " << Q_FUNC_INFO; } while(0)
#define LOG_STREAM qDebug()

#endif // GYSTYPES_H
