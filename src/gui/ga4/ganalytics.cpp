#include "ganalytics.h"
#include "ganalytics_worker.h"

#include <QDataStream>
#include <QDebug>
#include <QLocale>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QQueue>
#include <QSettings>
#include <QTimer>
#include <QUrlQuery>
#include <QUuid>
#include <QLocale>

#include <QJSonObject>
#include <QJSonDocument>
#include <QJsonArray>

GAnalytics::GAnalytics(QObject *parent) : QObject(parent)
{
    d = new GAnalyticsWorker(this);
}

/**
 * Destructor of class GAnalytics.
 */
GAnalytics::~GAnalytics()
{
    delete d;
}

GAnalytics *GAnalytics::instance()
{
    static GAnalytics* instance = nullptr;
    if(instance == nullptr)
    {
        instance = new GAnalytics();
    }

    return instance;
}

void GAnalytics::setLogLevel(GAnalytics::LogLevel logLevel)
{
    d->m_logLevel = logLevel;
}

GAnalytics::LogLevel GAnalytics::logLevel() const
{
    return d->m_logLevel;
}

// SETTER and GETTER
void GAnalytics::setViewportSize(const QString &viewportSize)
{
    d->m_viewportSize = viewportSize;
}

QString GAnalytics::viewportSize() const
{
    return d->m_viewportSize;
}

void GAnalytics::setLanguage(const QString &language)
{
    d->m_language = language;
}

QString GAnalytics::language() const
{
    return d->m_language;
}

void GAnalytics::setAnonymizeIPs(bool anonymize)
{
    d->m_anonymizeIPs = anonymize;
}

bool GAnalytics::anonymizeIPs()
{
    return d->m_anonymizeIPs;
}

void GAnalytics::setSendInterval(int milliseconds)
{
    d->m_timer.setInterval(milliseconds);
}

int GAnalytics::sendInterval() const
{
    return (d->m_timer.interval());
}

bool GAnalytics::isEnabled()
{
    return d->m_isEnabled;
}

void GAnalytics::setMeasurementId(const QString &measurementId)
{
    d->m_measurementId = measurementId;
}

void GAnalytics::setApiSecret(const QString & apiSecret)
{
	d->m_apiSecret = apiSecret;
}

void GAnalytics::setClientID(const QString &clientID)
{
    d->m_clientID = clientID;
}

void GAnalytics::enable(bool state)
{
    d->enable(state);
}

void GAnalytics::enableValidation(bool state)
{
    d->m_validation = state;
}

int GAnalytics::version()
{
    return d->m_version;
}

void GAnalytics::setVersion(int version)
{
    d->m_version = version;
}

void GAnalytics::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager)
{
    if (d->networkManager != networkAccessManager)
    {
        // Delete the old network manager if it was our child
        if (d->networkManager && d->networkManager->parent() == this)
        {
            d->networkManager->deleteLater();
        }

        d->networkManager = networkAccessManager;
    }
}

QNetworkAccessManager *GAnalytics::networkAccessManager() const
{
    return d->networkManager;
}

void GAnalytics::clicked(const TrackPage trackPage, const TrackElement trackButton){
    sendEvent("clicked", _trackPageString[trackPage], _trackElementString[trackButton]);
}

void GAnalytics::opened(const TrackPage trackPage){
    sendEvent("screen_view", _trackPageString[trackPage]);
}

/**
 * This method is called whenever a button was pressed in the application.
 * A query for a POST message will be created to report this event. The
 * created query will be stored in a message queue.
 */
void GAnalytics::sendEvent(const QString &eventName, const QString &page, const QString &element)
{
	QJsonObject root;
	root["client_id"] = d->m_clientID;

    //https://developers.google.com/analytics/devguides/collection/protocol/ga4/reference?client_type=gtag#payload_consent
	QJsonObject consent;
    consent["ad_user_data"] = "DENIED";
    consent["ad_personalization"] = "DENIED";
	root["consent"] = consent;

	QJsonObject event;
    event["name"] = eventName;

	QJsonObject eventParams;
	if (!page.isEmpty()) {
	    eventParams["screen_name"] = page;
    }
    if (!element.isEmpty()) {
	    eventParams["trackelement"] = element;
    }

    // In order for user activity to display in standard reports like Realtime, engagement_time_msec and session_id must be supplied as part of the params for an event.
    // https://developers.google.com/analytics/devguides/collection/protocol/ga4/sending-events?client_type=gtag#optional_parameters_for_reports
    eventParams["engagement_time_msec"] = 100;
    eventParams["session_id"] = d->m_clientID;

    // Language 
    QLocale systemLocale = QLocale::system();
    eventParams["ul"]= systemLocale.name();     

    // screen resolution    
    eventParams["sr"]= d->m_screenResolution;    

    // login

    // screen_view

    // operation system
#ifdef Q_OS_WIN
    eventParams["os"] = "Windows";
#endif
#ifdef Q_OS_LINUX
    eventParams["os"] = "Linux";
#endif
#ifdef Q_OS_MAC
    eventParams["os"] = "MAC";
#endif
	event["params"] = eventParams;

	QJsonArray eventArray;
	eventArray.append(event);
	root["events"] = eventArray;

    d->enqueQueryWithCurrentTime(root);
}

/**
 * Qut stream to persist class GAnalytics.
 */
QDataStream &operator<<(QDataStream &outStream, const GAnalytics &analytics)
{
    outStream << analytics.d->persistMessageQueue();

    return outStream;
}

/**
 * In stream to read GAnalytics from file.
 */
QDataStream &operator>>(QDataStream &inStream, GAnalytics &analytics)
{
    QList<QString> dataList;
    inStream >> dataList;
    analytics.d->readMessagesFromFile(dataList);

    return inStream;
}