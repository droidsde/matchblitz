//
//  GameManager.cpp
//  MatchThree
//
//  Created by kpuspesh on 7/15/13.
//
//

#include "EventsManager.h"

EventsManager* EventsManager::m_mySingleton = NULL;

EventsManager::EventsManager()
{
    this->eventMetadataList = CCArray::create();
    this->eventMetadataList->retain();
}

EventsManager::~EventsManager()
{
    this->eventMetadataList->removeAllObjects();
    this->eventMetadataList->release();
}

EventsManager* EventsManager::sharedManager()
{
    //If the singleton has no instance yet, create one
    if(NULL == m_mySingleton)
    {
        //Create an instance to the singleton
        m_mySingleton = new EventsManager();
    }
    return m_mySingleton;
}

bool EventsMetadata::init() {
    // no-op
    this->name = "";
    this->filename = "";
    this->target = NULL;
    return true;
}

void EventsManager::initListeners(CCObject *target) {
   // no-op
}
void EventsManager::removeListeners() {
    
}
void EventsManager::registerListener(CCObject *target, const char * event){
//    EventsManager::sharedManager()->addEventListener(target, event, callfuncO_selector(SoundManager::playSound), NULL);
}
void EventsManager::deregisterListener(const char * event){
    
}

void EventsManager::dispatchEvent(const char* event, CCObject* obj) {
    EventsMetadata *sm = EventsMetadata::create();
    sm->name = event;
    sm->target = obj;
    this->eventMetadataList->addObject(sm);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(event, sm);
}

void EventsManager::addEventListener(CCObject *target, const char* event, SEL_CallFuncO sel, CCObject* obj) {
    EventsMetadata *sm = EventsMetadata::create();
    sm->name = event;
    sm->target = obj;
    this->eventMetadataList->addObject(sm);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(target, sel, event, NULL);
}