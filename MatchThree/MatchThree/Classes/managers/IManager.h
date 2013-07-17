//
//  IManager.h
//  MatchThree
//
//  Created by kpuspesh on 7/16/13.
//
//

#ifndef MatchThree_IManager_h
#define MatchThree_IManager_h

#include "common.h"
#include "cocos2d.h"

USING_NS_CC;
class IManager {
private:
    
protected:
    
    
public:
    virtual void initListeners(CCObject *target) = 0;
    virtual void removeListeners() = 0;
    virtual void registerListener(CCObject *target, const char * event) = 0;
    virtual void deregisterListener(const char * event) = 0;
};

#endif
