//
//  AnimationManager.cpp
//  MatchThree
//
//  Created by kpuspesh on 7/15/13.
//
//

#include "AnimationManager.h"
#include "EventsManager.h"
#include "SimpleAudioEngine.h"
#include "Tile2.h"

AnimationManager* AnimationManager::m_mySingleton = NULL;

AnimationManager::AnimationManager()
{

}

AnimationManager::~AnimationManager()
{

}

AnimationManager* AnimationManager::sharedManager()
{
    //If the singleton has no instance yet, create one
    if(NULL == m_mySingleton)
    {
        //Create an instance to the singleton
        m_mySingleton = new AnimationManager();
    }
    return m_mySingleton;
}

void AnimationManager::initListeners(CCObject *target) {
    this->parentTarget = (CCLayer*) target;
    std::map<const char*, std::string>::iterator itr;
    for(itr = _global->particle_effects.begin(); itr != _global->particle_effects.end(); ++itr) {
        AnimationManager::registerListener(target, (*itr).first);
    }
}

void AnimationManager::registerListener(CCObject *target, const char *event) {
    EventsManager::sharedManager()->addEventListener(target, event, callfuncO_selector(AnimationManager::playParticleEffect), NULL);
}

void AnimationManager::deregisterListener(const char *event) {
    EventsManager::sharedManager()->deregisterListener(event);
}

void AnimationManager::removeListeners() {

}

void AnimationManager::playParticleEffect(CCObject *obj) {
    EventsMetadata *sm = (EventsMetadata *) obj;
    Tile2 *tmp = (Tile2*) sm->target;
    CCParticleSystemQuad *burst = CCParticleSystemQuad::create(_global->particle_effects[sm->name].c_str());
    burst->setPosition(tmp->pixPosition());
    burst->setAutoRemoveOnFinish(true);

    // add to the game layer
    CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1000)->getChildByTag(1002)->addChild(burst);
}

