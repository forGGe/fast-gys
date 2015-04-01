#ifndef HTTP_FETCHER_H
#define HTTP_FETCHER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QUrl>

#include "fetcher.h"
#include "exceptions.h"

///
/// \brief Class used to fetch data from HTTP responce.
/// \tparam DataParser Data parser class.
///
template < typename DataParser >
class HTTPfetcher : public Fetcher
{
public:
    ///
    /// \brief Constructor.
    /// \param[in] parent Parent QObject.
    ///
    HTTPfetcher(QObject *parent = 0);

    ///
    /// \brief Destroys fetcher.
    ///
    ~HTTPfetcher();

protected:
    ///
    /// \brief Empty implementation.
    /// \TODO Should be supported as soon as url will be customizable.
    ///
    virtual void handleStart();

    ///
    /// \brief Delegates processing of existing records to a subclass.
    /// \param[in] rec Exising record.
    ///
    virtual void handleProcess(QSqlRecord rec);

private slots:
    ///
    /// \brief Handles reply produced by previously submitted request.
    /// \param[in] reply Reply itself.
    ///
    void replyReady(QNetworkReply *reply);

private:
    DataParser            m_parser;  ///< Parses incoming responces.
    QNetworkAccessManager *m_mgr;    ///< Holds request\responce management.
};


//------------------------------------------------------------------------------
// Public methods

template < typename DataParser >
HTTPfetcher < DataParser >::HTTPfetcher(QObject *parent)
    :Fetcher(parent)
    ,m_parser()
    ,m_mgr(nullptr)
{
    m_mgr = new QNetworkAccessManager(this);

    QObject::connect(m_mgr, &QNetworkAccessManager::finished,
                     this, &HTTPfetcher::replyReady);
}

//------------------------------------------------------------------------------
// Protected methods

template < typename DataParser >
HTTPfetcher < DataParser >::handleStart()
{
    // TODO:
}

template < typename DataParser >
HTTPfetcher < DataParser >::handleProcess(QSqlRecord rec)
{
    // TODO: make link customizable
    QUrl url;
    url.setScheme("http");
    url.setHost("data.alexa.com");
    url.setPath("/data");
    url.setQuery(QString("cli=10&dat=snbamz&url=") + rec.value("name").toString());
    m_mgr->get(QNetworkRequest(url));

}

//------------------------------------------------------------------------------
// Private methods

template < typename DataParser >
HTTPfetcher < DataParser >::replyReady(QNetworkReply *reply)
{
    m_parser.setDevice(reply);

    QSqlRecord rec;

    // TODO: move it to parser
    rec.append(QSqlField{"name", QVariant::String});
    rec.append(QSqlField{"email", QVariant::String});
    rec.append(QSqlField{"rank", QVariant::String});
    rec.append(QSqlField{"local_rank", QVariant::String});
    rec.append(QSqlField{"country", QVariant::String});

    while (!m_parser.atEnd())
    {
        m_parser >> rec;
        emit send(rec);
    }
}

#endif // HTTP_FETCHER_H

