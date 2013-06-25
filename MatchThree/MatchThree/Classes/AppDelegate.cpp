//
//  MatchThreeAppDelegate.cpp
//  MatchThree
//
//  Created by kpuspesh on 5/17/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"

USING_NS_CC;
using namespace CocosDenshion;


AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    int designSizeWidth = 1024;
    int designSizeHeight = 768;
    
    _global = Globals::create();
    _global->retain();
    _global->init();
    
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    CCSize screenSize = pEGLView->getFrameSize();
    if(screenSize.width > screenSize.height) {
        designSizeWidth = 1024;
        designSizeHeight = 768;
        _global->setStartX(kStartX);
        _global->setStartY(kStartY);
    } else {
        designSizeWidth = 768;
        designSizeHeight = 1024;
        _global->setStartX(kStartXAlternate);
        _global->setStartY(kStartYAlternate);
    }
    
    CCSize designSize = CCSize(designSizeWidth, designSizeHeight);
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionExactFit);

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = MatchThree::scene();

    // run
    pDirector->runWithScene(pScene);
    pDirector->setContentScaleFactor(screenSize.height/designSize.height);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
