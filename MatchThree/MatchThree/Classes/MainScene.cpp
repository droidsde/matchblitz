#include "MainScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MatchThree::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MatchThree *layer = MatchThree::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MatchThree::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
   /* CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(MatchThree::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
*/
    /////////////////////////////
    // 3. add your codes below...
    
    // ask director the window size
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    _batchNode = CCSpriteBatchNode::create("Sprites.pvr.ccz");
    this->addChild(_batchNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");
    
    _flower = CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    _flower->setPosition( ccp(winSize.width * 0.1, winSize.height * 0.5) );
    _batchNode->addChild(_flower, 1);

    /*CCParticleSystemQuad* m_emitter = new CCParticleSystemQuad();
    m_emitter = CCParticleFlower::create();
    m_emitter->setPosition(10, 10);
    // Gravity Mode
    this->addChild(m_emitter, 0);
    
    CCParticleSystemQuad* m_emitter2 = new CCParticleSystemQuad();
    m_emitter2 = CCParticleSmoke::create();
    m_emitter2->setPosition(100, 100);
    this->addChild(m_emitter2, 10);
    
    CCParticleSystemQuad* m_emitter3 = new CCParticleSystemQuad();
    m_emitter3 = CCParticleFire::create();
    this->addChild(m_emitter3, 20);*/
    
    this->scheduleUpdate();
    
    return true;
}

void MatchThree::update(<#float dt#>)
{
    // do something
    return;
}

void MatchThree::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
