//
//  SoundManager.h
//  MatchThree
//
//  Created by kpuspesh on 7/15/13.
//
//

#ifndef __MatchThree__SoundManager__
#define __MatchThree__SoundManager__

#include <iostream>
#include "IManager.h"

USING_NS_CC;
class SoundManager : public IManager {
    
private:
    //Instance of the singleton
    static SoundManager* m_mySingleton;
        
protected:
    //Constructor
    SoundManager();
    ~SoundManager();
    
public:
    void initListeners(CCObject *target);
    void registerListener(CCObject *target, const char * event);
    void deregisterListener(const char * event);
    void removeListeners();
    
    void playSound(CCObject *obj);
    
    //Get instance of singleton
    static SoundManager* sharedManager();
};

#endif /* defined(__MatchThree__SoundManager__) */
