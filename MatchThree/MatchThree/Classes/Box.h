//
//  Box.h
//  MatchThree
//
//  Created by kpuspesh on 6/2/13.
//
//
// Note 0,0 index on Box os bottom left
//
//  ^
//  |
//  4
//  3
//  2
//  1
//  0 1 2 3 4 ->
#ifndef MatchThree_Box_h
#define MatchThree_Box_h

#include "cocos2d.h"
#include "common.h"
#include "Tile2.h"

USING_NS_CC;
class Box : public CCObject {
private:
    int repair();
    int repairSingleColumn(int columnIndex);
    void checkWith(Orientation orient);
    CCFiniteTimeAction* createPlayPieceAction(int index, int total);
    CCFiniteTimeAction* createPlayPieceSwiggle(int moves);
    CCFiniteTimeAction* createPlayPieceMovement(int moves);
    void burstTile(Tile2 *tile, float burstDelay);
    
public:
    bool lock;
    CCSize size;
    CCLayer* layer;
    
    Tile2 *OutBorderTile;
    Tile2 * first;
    Tile2 * second;
    CCArray *content;
    CCArray *delayTimeContent;
    CCSet *readyToRemoveTiles;
    CCSet *readyToChangeTiles;
    bool drawBG(int x, int y);
    
    bool initWithSize(CCSize size,int factor);
    Tile2 * objectAtX (int posX, int posY);
    
    bool check();
    void unlock();
    void removeSprite (CCNode * sender);
    void afterAllMoveDone(CCNode * sender);
    void repairCallback();
    bool haveMore();
    void initLayer(CCLayer * l);
    void clearBurstDelay();
    float getMaxBurstDelay();
    
    virtual bool init();
    ~Box();
    CREATE_FUNC(Box);
};
#endif
