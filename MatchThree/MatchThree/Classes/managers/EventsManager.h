//
//  EventsManager.h
//  MatchThree
//
//  Created by kpuspesh on 7/15/13.
//
//

#ifndef __MatchThree__EventsManager__
#define __MatchThree__EventsManager__

#include "IManager.h"
#include <iostream>

USING_NS_CC;
class EventsManager : public IManager {

private:
    //Instance of the singleton
    static EventsManager* m_mySingleton;
    
    CCArray* m_effectQueue;
    CCArray* eventMetadataList;
    
protected:
    //Constructor
    EventsManager();
    ~EventsManager();
    
public:
    // Functions to register listener to any events being thrown around
    void initListeners(CCObject *target);
    void registerListener(CCObject *target, const char * event);
    void deregisterListener(const char * event);
    void removeListeners();
    
    void dispatchEvent(const char* event, CCObject* target);
    void addEventListener(CCObject *target, const char* event, SEL_CallFuncO sel, CCObject* obj);
    
    //Get instance of singleton
    static EventsManager* sharedManager();
};


USING_NS_CC;
class EventsMetadata : public CCObject
{
private:
    
public:
    CCObject *target;
    const char *name;
    std::string filename;
    
    virtual bool init();
    CREATE_FUNC(EventsMetadata);
};


#endif /* defined(__MatchThree__EventsManager__) */
