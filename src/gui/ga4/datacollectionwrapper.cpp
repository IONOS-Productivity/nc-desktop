#include "datacollectionwrapper.h"

DataCollectionWrapper::DataCollectionWrapper(QObject *parent) : QObject(parent) {
}

DataCollectionWrapper::~DataCollectionWrapper() {
}

void DataCollectionWrapper::clicked(const TrackingPage trackingPage, const TrackingElement trackingButton){
    trackEvent( _trackingPageString[trackingPage], _trackingElementString[trackingButton]);
}

void DataCollectionWrapper::opened(const TrackingPage trackingPage){
    trackEvent(_trackingEventString[TrackingEvent::Open], _trackingPageString[trackingPage]);
}

void DataCollectionWrapper::login(){
    trackEvent(QString(), _trackingEventString[TrackingEvent::Login]);
}

void DataCollectionWrapper::trackEvent(QString page, QString element) {
    GAnalytics::getInstance().sendEvent(page, element);
}