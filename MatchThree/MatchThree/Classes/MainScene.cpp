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
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0); 
      
    CCSprite *bg = CCSprite::create("ingame_menu.png");
    bg->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(bg, 0);

    _box = Box::create();
    _box->initWithSize(CCSizeMake(kBoxWidth,kBoxHeight), 6);
    _box->layer = this;
    _box->lock = true;
    
    //this->isTouchEnabled = true;
    
    return true;
}

void MatchThree::onEnterTransitionDidFinish ()
{
    _box->check();
}

void MatchThree::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    if (_box->lock) {
       // return;
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
        this->changeWithTileA(_selectedTile, tile); // @selector(check:data:)
        _selectedTile = NULL;
    } else {
        _selectedTile = tile;
   //     this->afterTurn(tile->sprite);
    }
}


void MatchThree::changeWithTileA(Tile2 * a, Tile2 * b)
{
   /* CCAction *actionA = CCSequence::createWithTwoActions(CCMoveTo::initWithDuration(kMoveTileTime, b->pixPosition()), CCCallFuncND::actionWithTarget($this->check(a));
    CCAction *actionB = CCSequence::actions(CCMoveTo::initWithDuration(kMoveTileTime, a->pixPosition()), CCCallFuncND::actionWithTarget($this->check(b), NULL);
                                                                                    
    CCAction *actionB = [CCSequence actions:
                         [CCMoveTo actionWithDuration:kMoveTileTime position:[a pixPosition]],
                         [CCCallFuncND actionWithTarget:self selector:sel data: b],
                         nil
                         ];
 
                                            
    a->sprite->runAction(actionA);
    b->sprite->runAction(actionB);
    a->trade(b);*/
}
                                            
void MatchThree::backCheck(Tile2 * sender, Tile2 * data)
{
    if(NULL == _firstOne){
        _firstOne = data;
        return;
    }
    _firstOne = NULL;
    _box->lock = false;
}

void MatchThree::check(Tile2 * sender, Tile2 * data)
{
    if(NULL == _firstOne){
        _firstOne = data;
        return;
    }
    bool result = _box->check();
    if (result) {
        _box->lock = false;
    } else {
        this->changeWithTileA(data, _firstOne);
        
     //this->runAction(CCSequence::createWithTwoActions(CCDelayTime::actionWithDuration(kMoveTileTime + 0.03f),
       //                                                CCCallFunc::actionWithTarget(_box->backCheck(sender, <#Tile2 *data#>));
    }
    
    _firstOne = NULL;
}


void MatchThree::afterTurn(CCSprite * node)
{
   /* if (_selectedTile && node == _selectedTile->sprite) {
        CCSprite *sprite = (CCSprite *)node;
        CCSequence *someAction = CCSequence::createWithVariableList(CCScaleBy::actionWithDuration(kMoveTileTime, 0.5f),
                                                                    CCScaleBy::actionWithDuration(kMoveTileTime, 2.0f),
                                                                    CCCallFuncN::actionWithTarget:self selector:@selector(afterTurn:)],
                                  nil];
        
        sprite->runAction(someAction);
    } */
}


void MatchThree::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
