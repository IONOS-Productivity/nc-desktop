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

/**
 * This method is called whenever a button was pressed in the application.
 * A query for a POST message will be created to report this event. The
 * created query will be stored in a message queue.
 */
void GAnalytics::sendEvent(const QString &page, const QString &element)
{
    d->enqueQueryWithCurrentTime(element, page);
}

void GAnalytics::sendEventImmediatley(const QString &page, const QString &element)
{
    d->enqueQueryWithCurrentTime(element, page);
    d->postMessage();
}
