#ifndef DATACOLLECTIONWRAPPER_H
#define DATACOLLECTIONWRAPPER_H

#include "ganalytics.h"

class DataCollectionWrapper : public QObject {

    Q_OBJECT
    Q_ENUMS(TrackingPage)
    Q_ENUMS(TrackingEvent)
    Q_ENUMS(TrackingElement)


public:

    enum TrackingEvent
    {
        Click,
        Open,
        Login
    };

    enum TrackingPage{
        GeneralSettings,
        AccountSettings,
    };

    enum TrackingElement{
        PrivacyPolicy,
        OpenSourceComponents,
        LegalNotice,
        MoreInformation,
        ServerNotifications,
        AutoStart,
        ToogleSendData,
        AutoCheckforUpdate
    };

    DataCollectionWrapper(QObject *parent = 0);;
    ~DataCollectionWrapper();

public slots:
	void login();
	void clicked(const TrackingPage trackingPage, const TrackingElement trackingButton);
	void opened(const TrackingPage trackingPage);
private:
    void trackEvent(QString event, QString page = QString(), QString element = QString());

    std::map<TrackingPage, QString> _trackingPageString = { 
        { GeneralSettings, "GeneralSetting" }, 
        { AccountSettings, "AccountSettings" },
    }; 

    std::map<TrackingElement, QString> _trackingElementString = { 
        { PrivacyPolicy, "PrivacyPolicy" },
        { OpenSourceComponents, "OpenSourceComponents" },
        { LegalNotice, "LegalNotice" },
        { MoreInformation, "MoreInformation" },
        { ServerNotifications, "ServerNotifications" },
        { AutoStart, "AutoStart" },
        { ToogleSendData, "ToogleSendData" },
        { AutoCheckforUpdate, "AutoCheckforUpdate" },
    }; 

    std::map<TrackingEvent, QString> _trackingEventString = { 
        { Click, "click" }, 
        { Open, "screen_view" }, 
        { Login, "login" },
    }; 
};

#endif // DATACOLLECTIONWRAPPER_H