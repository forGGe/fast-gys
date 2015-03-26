#ifndef FETCHER_H
#define FETCHER_H

#include <QObject>
#include <QSqlRecord>

/// \TODO: comments

class Fetcher : public QObject
{
    Q_OBJECT
public:
    explicit Fetcher(QObject *parent = 0);
    virtual ~Fetcher();

signals:
    void send(QSqlRecord rec);
    void end();

public slots:
    void start();

protected:
    virtual void handleStart() = 0;
};

#endif // FETCHER_H
