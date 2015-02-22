#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>
#include <QSqlDatabase>

class MainWindow;
class QAbstractItemModel;

class MainClass : public QObject
{
    Q_OBJECT
public:
    MainClass(QObject *parent = nullptr);
    ~MainClass();

private:
    QSqlDatabase        m_db;
    QAbstractItemModel  *m_model;
    MainWindow          *m_w;
};


#endif // MAINCLASS_H

