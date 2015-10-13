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
    MainClass(MainWindow *parent);
    ~MainClass();

    // Copying is not permitted.
    MainClass(const MainClass&) = delete;
    MainClass& operator =(const MainClass&) = delete;

    void setupView(QAbstractItemView *view);
    void saveCurrentTableAsCSV(const QString& filePath);
    void copyAllTableData();

private:
    void setupDatabase();

private:
    QSqlDatabase     m_db;
    QSqlTableModel   *m_model;
    MainWindow       *m_mw;
    // Counter used to flush DB periodically. Hack.
    uint64_t          m_hackCounter;

public slots:
    void newData(const QSqlRecord record);
    void updateData(const QSqlRecord record);

private slots:
    // Not thread safe slot since using references, but faster
    void loadFile(const QString &filePath);
    void updateAll();
    void flushDB();
};


#endif // MAINCLASS_H

