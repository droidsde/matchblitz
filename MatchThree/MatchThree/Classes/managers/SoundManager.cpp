//
//  SoundManager.cpp
//  MatchThree
//
//  Created by kpuspesh on 7/15/13.
//
//

#include "SoundManager.h"
#include "EventsManager.h"
#include "SimpleAudioEngine.h"

SoundManager* SoundManager::m_mySingleton = NULL;

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{

}

SoundManager* SoundManager::sharedManager()
{
    //If the singleton has no instance yet, create one
    if(NULL == m_mySingleton)
    {
        //Create an instance to the singleton
        m_mySingleton = new SoundManager();
    }
    return m_mySingleton;
}

void SoundManager::initListeners(CCObject *target) {
    std::map<const char*, std::string>::iterator itr;
    for(itr = _global->sound_effects.begin(); itr != _global->sound_effects.end(); ++itr) {
        EventsManager::sharedManager()->addEventListener(target, (*itr).first, callfuncO_selector(SoundManager::playSound), NULL);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect((*itr).second.c_str());
    }
}

void SoundManager::registerListener(CCObject *target, const char *event) {
    EventsManager::sharedManager()->addEventListener(target, event, callfuncO_selector(SoundManager::playSound), NULL);
}

void SoundManager::deregisterListener(const char *event) {
    EventsManager::sharedManager()->deregisterListener(event);
}

void SoundManager::removeListeners() {

}

void SoundManager::playSound(CCObject *obj) {
    EventsMetadata *sm = (EventsMetadata *) obj;
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(_global->getSoundEffect(sm->name).c_str());
}
