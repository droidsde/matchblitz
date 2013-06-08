#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"
#include "Box.h"
#include "Tile2.h"

USING_NS_CC;
class MatchThree : public cocos2d::CCLayerColor
{
private:
	cocos2d::CCSpriteBatchNode * _batchNode;
    
    Box * _box;
    Tile2 * _selectedTile;
    Tile2 * _firstOne;
    
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void onEnterTransitionDidFinish();
    
    void changeWithTileA(Tile2 *a, Tile2 * b, SEL_CallFuncND sel);
    void backCheck(CCNode * sender, Tile2 * data);
    void check(CCNode * sender, Tile2 * data);
    void ccTouchesBegan(CCSet* touches, CCEvent* event);
    void afterTurn(CCSprite * node);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MatchThree);
};

#endif // __HELLOWORLD_SCENE_H__
