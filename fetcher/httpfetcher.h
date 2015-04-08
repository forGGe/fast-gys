#ifndef HTTP_FETCHER_H
#define HTTP_FETCHER_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QHostAddress>

#include "fetcher.h"
#include "exceptions.h"
#include "types.h"

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

    // Copying is not permitted.
    HTTPfetcher(HTTPfetcher &) = delete;
    HTTPfetcher& operator =(HTTPfetcher &) = delete;

protected:
    ///
    /// \brief Empty implementation.
    /// \TODO Should be supported as soon as url will be customizable.
    ///
    virtual void handleStart();

    ///
    /// \brief Fetches Web data for existing record.
    /// \param[in] rec Exising record.
    ///
    virtual void handleProcess(QSqlRecord &rec);

    ///
    /// \brief Completes data processing.
    ///
    virtual void handleComplete();

private slots:
    ///
    /// \brief Handles reply produced by previously submitted request.
    /// \param[in] reply Reply itself.
    ///
    void replyReady(QNetworkReply *reply);

private:
    DataParser            m_parser;     ///< Parses incoming responces.
    QNetworkAccessManager *m_mgr;       ///< Holds request\responce management.
    quint32               m_pending;    ///< Amount of pending requests.
    /// Indicates that no more input will be provided to this fetcher.
    bool                  m_noMoreInput;
};


//------------------------------------------------------------------------------
// Public methods

template < typename DataParser >
HTTPfetcher < DataParser >::HTTPfetcher(QObject *parent)
    :Fetcher(parent)
    ,m_parser()
    ,m_mgr(nullptr)
    ,m_pending(0)
    ,m_noMoreInput(false)
{
    m_mgr = new QNetworkAccessManager(this);

    QObject::connect(m_mgr, &QNetworkAccessManager::finished,
                     this, &HTTPfetcher::replyReady);
}

template < typename DataParser >
HTTPfetcher < DataParser >::~HTTPfetcher()
{

}

//------------------------------------------------------------------------------
// Protected methods

template < typename DataParser >
void HTTPfetcher < DataParser >::handleStart()
{
    // TODO:
}

template < typename DataParser >
void HTTPfetcher < DataParser >::handleProcess(QSqlRecord &rec)
{
    // TODO: make link customizable
    // TODO: move head of link to the ctor
    QUrl url;
    url.setScheme("http");
    url.setHost("data.alexa.com");
    url.setPath("/data");
    url.setQuery(QString("cli=10&dat=snbamz&url=") + rec.value("name").toString());

    // Generated _public_ IP, according to IANA restrictions,
    // in range  from 173.0.0.1 to 191.255.255.254
    // excluding multicast and broadcast addresses
    qint32 ip = 0;
    ip = (ip << 8) | (qrand() % 29 + 173);  // MSB from 173 to 191
    ip = (ip << 8) | (qrand() % 255);       // 2nd byte from 0 to 255
    ip = (ip << 8) | (qrand() % 255);       // 3rd byte from 0 to 255
    ip = (ip << 8) | (qrand() % 253 + 1);   // LSB from 1 to 254
    // TODO: Apply more strict boundaries according to
    // https://en.wikipedia.org/wiki/Reserved_IP_addresses

    const QByteArray &header = QHostAddress(ip).toString().toUtf8();

    QNetworkRequest req(url);
    req.setRawHeader("X-FORWARDED-FOR", header);
    req.setRawHeader("Via", header);
    req.setRawHeader("CLIENT-IP", header);

    m_mgr->get(req);

    ++m_pending;
}

template < typename DataParser >
void HTTPfetcher < DataParser >::handleComplete()
{
    m_noMoreInput = true;
}

//------------------------------------------------------------------------------
// Private methods

template < typename DataParser >
void HTTPfetcher < DataParser >::replyReady(QNetworkReply *reply)
{
    // TODO: error check && catch exceptions
    if (reply->error())
    {
        LOG_STREAM << "Erorr occurs for url " << reply->url().toString();
        LOG_STREAM << reply->errorString();
        reply->deleteLater();
        // TODO: BUG: if error occurs, pending counter will not be decreased
        // and object will not emit done() signal
        return;
    }

    QSqlRecord rec;


    // TODO: customasible URL
    QString query; // URL query string
    QString param("&url=");
    int idx;

    // Strip out domain from URL.
    // This is necessary because a data retrieved after fetching
    // will not contain target domain OR
    // it will contain normalized or redirected form.
    // I.e. 'somesite.com' instead of 'www.somesite.com'
    // Such aliasing will result in duplicated records.
    // Semantically both are equal, but refers different objects
    // in database.
    query = reply->url().query();
    idx = query.indexOf(param);
    idx += param.size();

    // TODO: move it to parser
    rec.append(QSqlField{"name", QVariant::String});
    rec.append(QSqlField{"rank", QVariant::String});
    rec.append(QSqlField{"local_rank", QVariant::String});
    rec.append(QSqlField{"country", QVariant::String});

    m_parser.setDevice(reply);
    while (!m_parser.atEnd())
    {
        m_parser >> rec;
        rec.setValue("name",  query.mid(idx));
        emit send(rec);
    }

    if (!(--m_pending) && m_noMoreInput)
        emit end();

    reply->deleteLater();
}

#endif // HTTP_FETCHER_H

