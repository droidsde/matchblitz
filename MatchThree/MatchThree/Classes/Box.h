//
//  Box.h
//  MatchThree
//
//  Created by kpuspesh on 6/2/13.
//
//

#ifndef MatchThree_Box_h
#define MatchThree_Box_h

#include "cocos2d.h"
#include "constants.h"
#include "Tile2.h"

USING_NS_CC;
class Box : public CCObject {
private:
    int repair();
    int repairSingleColumn(int columnIndex);
    
public:
    bool lock;
    CCSize size;
    CCLayer *layer;
    
    Tile2 *OutBorderTile;
    Tile2 * first;
    Tile2 * second;
    CCArray *content;
    CCSet *readyToRemoveTiles;
    
    bool initWithSize(CCSize size,int factor);
    Tile2 * objectAtX (int posX, int posY);
    
    bool check();
    void checkWith(Orientation orient);
    void unlock();
    void removeSprite (CCNode * sender);
    void afterAllMoveDone(CCNode * sender);
    bool haveMore();
    
    virtual bool init();
    CREATE_FUNC(Box);
};
#endif
