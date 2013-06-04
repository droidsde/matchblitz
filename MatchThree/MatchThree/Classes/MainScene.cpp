#include "MainScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MatchThree::scene()
{
    CCScene *scene = CCScene::create();
    MatchThree *layer = MatchThree::create();
    scene->addChild(layer);
    return scene;
}

bool MatchThree::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSize size = CCDirector::sharedDirector()->getWinSize();
      
    CCSprite *bg = CCSprite::create("ingame_menu.png");
    bg->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(bg, 0);

    _box = Box::create();
    _box->retain();
    _box->initWithSize(CCSizeMake(kBoxWidth,kBoxHeight), 6);
    _box->layer = this;
    _box->lock = true;
    
    this->setTouchEnabled(true);
    
    return true;
}

void MatchThree::onEnterTransitionDidFinish ()
{
    _box->check();
}

void MatchThree::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    if (_box->lock) {
       return;
    }
    
    CCTouch* touch = (CCTouch *) touches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    int x = (location.x -kStartX) / kTileSize;
    int y = (location.y -kStartY) / kTileSize;
    
    
    if (_selectedTile && _selectedTile->x == x && _selectedTile->y == y) {
        return;
    }
    
    Tile2 *tile = _box->objectAtX(x, y);
    if (_selectedTile && _selectedTile->nearTile(tile)) {
        _box->lock = true;
        this->changeWithTileA(_selectedTile, tile, callfuncND_selector(MatchThree::check));
        _selectedTile = NULL;
    } else {
        _selectedTile = tile;
        this->afterTurn(tile->sprite);
    }
}


void MatchThree::changeWithTileA(Tile2 * a, Tile2 * b, SEL_CallFuncND sel)
{
    CCAction *actionA = CCSequence::createWithTwoActions(
                            CCMoveTo::create(kMoveTileTime, b->pixPosition()),
                            CCCallFuncND::create(this, sel, a));
    CCAction *actionB = CCSequence::createWithTwoActions(
                            CCMoveTo::create(kMoveTileTime, a->pixPosition()),
                            CCCallFuncND::create(this, sel, b));
                                                         
    a->sprite->runAction(actionA);
    b->sprite->runAction(actionB);
    a->trade(b);
}
                                            
void MatchThree::backCheck(CCNode * sender, Tile2 * data)
{
    if(NULL == _firstOne){
        _firstOne = data;
        return;
    }
    _firstOne = NULL;
    _box->lock = false;
}

void MatchThree::check(CCNode * sender, Tile2 * data)
{
    if(NULL == _firstOne){
        _firstOne = data;
        return;
    }
    bool result = _box->check();
    if (result)
    {
        _box->lock = false;
    }
    else
    {
        this->changeWithTileA(data, _firstOne, callfuncND_selector(MatchThree::backCheck));
        this->runAction(CCSequence::create(
                            CCDelayTime::create(kMoveTileTime + 0.03f),
                            CCCallFuncN::create(_box, callfuncN_selector(Box::unlock))));
    }
    
    _firstOne = NULL;
}


void MatchThree::afterTurn(CCSprite * node)
{
   if (_selectedTile && node == _selectedTile->sprite) {
        CCSprite *sprite = (CCSprite *)node;
        CCFiniteTimeAction *someAction = CCSequence::create(
                            CCScaleBy::create(kMoveTileTime, 0.5f),
                            CCScaleBy::create(kMoveTileTime, 2.0f),
                            CCCallFuncN::create(this, callfuncN_selector(MatchThree::afterTurn)), NULL);
        
        sprite->runAction(someAction);
    }
}


void MatchThree::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
