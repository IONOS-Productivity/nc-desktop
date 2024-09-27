#pragma once

#include <QObject>
#include <QVariantMap>

class QNetworkAccessManager;
class GAnalyticsWorker;

// https://developers.google.com/analytics/devguides/collection/protocol/ga4/verify-implementation?client_type=gtag
class GAnalytics : public QObject
{
    Q_OBJECT
    Q_ENUMS(LogLevel)



public:
    ~GAnalytics();

    static GAnalytics& getInstance()
    {
        static GAnalytics instance; 
        return instance;
    }

public:
    enum LogLevel
    {
        Debug,
        Info,
        Error
    };

        enum TrackEvent
    {
        Click,
        Launch,
        CloseDialog,
        Open
    };

    enum TrackPage{
        GeneralSetting,
        AccountSetting,
    };

    enum TrackElement{
        Save,
        Cancel,
        Add,
        Remove
    };

    void setMeasurementId(const QString& measurementId);
	void setApiSecret(const QString& apiSecret);
    void setClientID(const QString& clientID);

    int version();
    void setVersion(int version);

    void setLogLevel(LogLevel logLevel);
    LogLevel logLevel() const;

    // Getter and Setters
    void setViewportSize(const QString &viewportSize);
    QString viewportSize() const;

    void setLanguage(const QString &language);
    QString language() const;

    void setAnonymizeIPs(bool anonymize);
    bool anonymizeIPs();

    void setSendInterval(int milliseconds);
    int sendInterval() const;

    void enable(bool state = true);
    bool isEnabled();
    
    void enableValidation(bool state);

    /// Get or set the network access manager. If none is set, the class creates its own on the first request
    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);
    QNetworkAccessManager *networkAccessManager() const;

public slots:
    void sendEvent(const QString &eventName, const QString &page = QString(), const QString &element = QString());
	void clicked(const TrackPage trackPage, const TrackElement trackButton);
	void opened(const TrackPage trackPage);

private:
    explicit GAnalytics(QObject *parent = 0);
    Q_DISABLE_COPY_MOVE(GAnalytics);

    std::map<TrackPage, QString> _trackPageString = { 
        { GeneralSetting, "GeneralSetting" }, 
        { AccountSetting, "AccountSetting" },
    }; 
    std::map<TrackElement, QString> _trackElementString = { 
        { Save, "Save" }, 
        { Cancel, "Cancel" },
    }; 
    std::map<TrackEvent, QString> _trackEventString = { 
        { Click, "Click" }, 
        { Launch, "Launch" },
    }; 

private:
    GAnalyticsWorker *d;

    friend QDataStream &operator<<(QDataStream &outStream, const GAnalytics &analytics);
    friend QDataStream &operator>>(QDataStream &inStream, GAnalytics &analytics);
};

QDataStream &operator<<(QDataStream &outStream, const GAnalytics &analytics);
QDataStream &operator>>(QDataStream &inStream, GAnalytics &analytics);

