#ifndef FETCHER_H
#define FETCHER_H

/// \TODO: comments

#include <QObject>
#include <QSqlRecord>


///
/// \brief The Fetcher class defines signal-slot interface for fetching data.
///
class Fetcher : public QObject
{
    Q_OBJECT
public:
    ///
    /// \brief Constructor.
    /// \param[in] parent Parent object.
    ///
    explicit Fetcher(QObject *parent = 0);

    ///
    /// \brief Desctructor.
    ///
    virtual ~Fetcher();

signals:
    ///
    /// \brief Emits to send data.
    /// \param[in] rec Record containing data.
    ///
    void send(QSqlRecord rec);

    ///
    /// \brief Emits when no data left to fecth.
    ///
    void end();

public slots:
    ///
    /// \brief Starts data fetching.
    /// This can be called just once.
    /// \note Multiple calls will lead to undefined behaviour.
    ///
    void start();

    ///
    /// \brief Processes exising record.
    /// \param[in] rec Exising record.
    ///
    void process(QSqlRecord rec);

    ///
    /// \brief Completes data processing.
    /// This should be used in conjuction with process() slot.
    /// If fetcher's client will finish with providing data, this slot
    /// should be executed.
    /// \note   If client will continue providing data after this call
    ///         then behavior is undefined.
    ///
    void complete();

protected:
    ///
    /// \brief Delegates starting of data fetching to a subclass.
    ///
    virtual void handleStart() = 0;

    ///
    /// \brief Delegates processing of existing records to a subclass.
    /// \param[in] rec Exising record.
    ///
    virtual void handleProcess(QSqlRecord &rec) = 0;

    ///
    /// \brief Delegates completition to a subclass.
    ///
    virtual void handleComplete() = 0;
};

#endif // FETCHER_H
