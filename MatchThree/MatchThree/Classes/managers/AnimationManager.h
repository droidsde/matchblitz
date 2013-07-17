//
//  AnimationManager.h
//  MatchThree
//
//  Created by kpuspesh on 7/15/13.
//
//

#ifndef __MatchThree__AnimationManager__
#define __MatchThree__AnimationManager__

#include <iostream>
#include "IManager.h"

USING_NS_CC;
class AnimationManager : public IManager {
    
private:
    //Instance of the singleton
    static AnimationManager* m_mySingleton;
        
protected:
    //Constructor
    AnimationManager();
    ~AnimationManager();
    
public:
    CCLayer * parentTarget;
    
    void initListeners(CCObject *target);
    void registerListener(CCObject *target, const char * event);
    void deregisterListener(const char * event);
    void removeListeners();
    
    void playParticleEffect(CCObject *obj);
    
    //Get instance of singleton
    static AnimationManager* sharedManager();
};

#endif /* defined(__MatchThree__AnimationManager__) */
