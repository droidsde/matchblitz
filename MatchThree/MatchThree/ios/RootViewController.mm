//
//  MatchThreeAppController.h
//  MatchThree
//
//  Created by kpuspesh on 5/17/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#import "RootViewController.h"
#import "cocos2d.h"
#import "common.h"

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
 
*/
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
    //UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAll;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}


- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    int designSizeWidth = 1024;
    int designSizeHeight = 768;
    
    cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
    cocos2d::CCSize s = director->getOpenGLView()->getFrameSize();
    director->getOpenGLView()->setFrameSize(s.height, s.width);
    s = director->getOpenGLView()->getFrameSize();
    
    if(s.width > s.height) {
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
    cocos2d::CCSize designSize = cocos2d::CCSize(designSizeWidth,designSizeHeight);
    cocos2d::CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionExactFit);
    //director->setContentScaleFactor(s.height/designSize.height);
    
    cocos2d::CCNode * node = director->getRunningScene()->getChildByTag(1000);
    cocos2d::CCSprite * bg = (cocos2d::CCSprite*) node->getChildByTag(1001);
    bg->setPosition(ccp(designSize.width/2, designSize.height/2));
    bg->setScaleX(designSize.width/bg->getContentSize().width);
    bg->setScaleY(designSize.height/bg->getContentSize().height);
    
    cocos2d::CCLayer * gameLayer = (cocos2d::CCLayer*) node->getChildByTag(1002);
    gameLayer->setPosition(ccp(_global->getStartX(), _global->getStartY()));
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
