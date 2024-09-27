#include "datacollectionwrapper.h"

DataCollectionWrapper::DataCollectionWrapper(QObject *parent) : QObject(parent) {
}

DataCollectionWrapper::~DataCollectionWrapper() {
}

void DataCollectionWrapper::clicked(const TrackingPage trackingPage, const TrackingElement trackingButton){
    trackEvent(_trackingEventString[TrackingEvent::Click], _trackingPageString[trackingPage], _trackingElementString[trackingButton]);
}

void DataCollectionWrapper::opened(const TrackingPage trackingPage){
    trackEvent(_trackingEventString[TrackingEvent::Open], _trackingPageString[trackingPage]);
}

void DataCollectionWrapper::login(){
    trackEvent(_trackingEventString[TrackingEvent::Login]);
}

void DataCollectionWrapper::trackEvent(QString event, QString page, QString element) {
    GAnalytics::getInstance().sendEvent(event, page, element);
}