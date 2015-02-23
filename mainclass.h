#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>

class MainWindow;
class QSqlTableModel;
class QAbstractItemView;


class MainClass : public QObject
{
    Q_OBJECT
public:
    MainClass(QObject *parent = nullptr);
    ~MainClass();

    void setupView(QAbstractItemView *view);

private:
    void setupDatabase();

private:
    QSqlDatabase        m_db;
    QSqlTableModel      *m_model;

public slots:
    void newData(const QSqlRecord record);
    void updateData(const QSqlRecord record);
};


#endif // MAINCLASS_H

