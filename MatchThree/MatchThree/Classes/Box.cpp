//
//  Box.cpp
//  MatchThree
//
//  Created by kpuspesh on 6/2/13.
//
//

#include "Box.h"

using namespace cocos2d;

bool Box::init()
{
    // do initialization here
    return true;
}

void Box::clearBurstDelay(){
    CCLog("F clearBurstDelay()");
    for (int y=0; y<size.height; y++) {
		for (int x=0; x < size.width; x++) {
            this->objectAtX(x, y)->burstDelay = 0.0f;
        }
    }
};

float Box::getMaxBurstDelay(){
    float maxDelay = 0.0f;
    for (int y=0; y<size.height; y++) {
		for (int x=0; x < size.width; x++) {
            maxDelay = MAX(maxDelay,this->objectAtX(x, y)->burstDelay);
        }
    }
    return maxDelay;
};

bool Box::drawBG(int x, int y){
    CCSprite *sprite = CCSprite::create(tile_bg_filename.c_str());
    sprite->setScale(kTileSize/sprite->getContentSize().width);
    sprite->setPosition(ccp(kStartX + x * kTileSize + kTileSize/2, kStartY + y * kTileSize + kTileSize/2));
    sprite->setOpacity(kTileBGOpacity);
    layer->addChild(sprite,0);
    return true;  // not in use for now
}

bool Box::initWithSize (CCSize aSize, int aFactor)
{
    size = aSize;
    OutBorderTile = Tile2::create();
    if (!OutBorderTile) {
        return false;
    }
    OutBorderTile->retain();
    OutBorderTile->initWithX(-1, -1);
	
    content = CCArray::createWithCapacity(size.height);
    content->retain();
    
    // Initialize the n x m array with
    // tile objects
	for (int y=0; y<size.height; y++) {
		CCArray *rowContent = CCArray::createWithCapacity(size.width);
		for (int x=0; x < size.width; x++) {
			Tile2 *tile = new Tile2();
            tile->init();
            tile->_debug_isOriginal = true;
            tile->initWithX(x, y);
            drawBG(x, y);
            
            //Get the initial balloon
            int value = rand()%kKindCount + 1;
            
            CCSprite *sprite = Tile2::getBalloonSprite(value, Normal);
            sprite->setPosition(tile->pixPosition());
            tile->sprite = sprite;
            
            tile->value = value;
            tile->type = Normal;
            layer->addChild(sprite,1);

			rowContent->addObject(tile);
		}
		content->addObject(rowContent);
	}
	
    // Empty array/set to hold tiles to remove at any point in time
	readyToRemoveTiles = CCSet::create();
    readyToChangeTiles = CCSet::create();
	readyToRemoveTiles->retain();
    readyToChangeTiles->retain();
  
	return true;
}

/**
 * Returns the Tile2 object (tile) at any given co-ordinates
 */
Tile2 * Box::objectAtX(int x, int y)
{
	if (x < 0 || x >= kBoxWidth || y < 0 || y >= kBoxHeight) {
		return OutBorderTile;
	}
    CCAssert(y < content->count(), "invalid y");
    CCArray * tmp = dynamic_cast<CCArray *> (content->objectAtIndex(y));
    CCAssert(x < tmp->count(), "invalid y");
    CCObject * obj = tmp->objectAtIndex(x);
	return dynamic_cast<Tile2 *> (obj);
}

/**
 * Checks the status of the box to figure out if there
 * are some matches already and do we need to clean some 
 * tiles
 */
bool Box::check() {
    bool ret = false;
    CCLog("+F Box:check()");

    this->checkTilesToClear();
    
    this->checkCombinations();
    
    ret = this->runEffectSequence();
    
    clearBurstDelay();
    
    CCLog("-F Box::check()");
    return ret;
}

bool Box::runEffectSequence() {
    
    if (readyToRemoveTiles->count() == 0 && readyToChangeTiles->count() == 0) {
        CCLog("-F Box:check() no tiles to change/remove");
        return false;       // nothing to do, no matches in current state
    }
    
    // Go and remove the tiles which are marked for removal (due to match3+)
    // also runs small animation on the tile sprite
    CCSetIterator itr = readyToRemoveTiles->begin();
    for (; itr != readyToRemoveTiles->end(); itr++) {
        Tile2 *tile = (Tile2 *) *itr;
        tile->value = 0;
        if (tile->sprite) {
            CCLOG("Scaling tile %d,%d with delay %f", tile->x, tile->y, tile->burstDelay);
            CCFiniteTimeAction *action = CCSequence::create(
                                                            CCDelayTime::create(tile->burstDelay),
                                                            CCScaleTo::create(0.3f, 0.0f),
                                                            CCCallFuncN::create(this, callfuncN_selector(Box::removeSprite)),
                                                            NULL
                                                            );
            tile->sprite->runAction(action);
            tile->matchType = Normal;//TODO
            
            // Add the balloon burst effect
            CCParticleSystemQuad *burst = CCParticleSystemQuad::create(burst_effect_filename.c_str());
            burst->setPosition(tile->pixPosition());
            burst->setAutoRemoveOnFinish(true);
            layer->addChild(burst);
        }
    }
    
    //Change all tiles (e.g. normal to striped balloon);
    itr = readyToChangeTiles->begin();
    for (; itr != readyToChangeTiles->end(); itr++) {
        Tile2 *new_tile = (Tile2 *) *itr;
        int x = new_tile->x;
        int y = new_tile->y;
        int value = new_tile->value;
        BalloonType type= new_tile->type;
        Tile2 *tile = this->objectAtX(x, y);
        
        CCLOG("Replacing tile at %d,%d | new_tile->%p org_tile->%p", x, y, new_tile, tile);
        CCLOG("new_tile ref count %d ", new_tile->m_uReference);
        float delay = tile->burstDelay;
        if (readyToRemoveTiles->containsObject(tile)){
            readyToRemoveTiles->removeObject(tile);
        }
        if (tile->sprite) {
            CCFiniteTimeAction *action = CCSequence::create(
                                                            CCDelayTime::create(delay),
                                                            CCFadeOut::create(0.3f),
                                                            CCCallFuncN::create(this, callfuncN_selector(Box::removeSprite)),
                                                            NULL
                                                            );
            tile->sprite->runAction(action);
        }
        CCSprite *new_sprite = Tile2::getBalloonSprite(value, type);
        new_sprite->setPosition(ccp(kStartX + x * kTileSize + kTileSize/2, kStartY + y * kTileSize + kTileSize/2));
        //new_sprite->setOpacity(0);
        tile->value = value;
        tile->type = type;
        tile->sprite = new_sprite;
        layer->addChild(new_sprite);
        tile->sprite->runAction(CCSequence::create(
                                                   CCDelayTime::create(delay),
                                                   CCFadeIn::create(0.3f),
                                                   NULL));
        
    }
    
    // temp hack to empty the array of tiles which got removed
    CCLOG("Releasing readyToRemoveTiles");
    //readyToRemoveTiles->removeAllObjects();
    readyToRemoveTiles->release();
    readyToRemoveTiles = CCSet::create();
	readyToRemoveTiles->retain();
    
    CCLOG("Releasing readyToChangeTiles");
    readyToChangeTiles->removeAllObjects();
    readyToChangeTiles->release();
    readyToChangeTiles = CCSet::create();
	readyToChangeTiles->retain();
    
    // REPAIR the box after matched tiles were deleted
    CCLOG("Setting repair callback");
    layer->runAction( CCSequence::create(
                                         CCDelayTime::create(kRepairDelayTime + this->getMaxBurstDelay()),
                                         CCCallFuncN::create(this, callfuncN_selector(Box::repairCallback)), NULL));
    
    return true;
}

void Box::repairCallback(){
    CCLOG("+F Box::repairCallback");
    int maxCount = this->repair();
    layer->runAction( CCSequence::create(
                                         CCDelayTime::create(kMoveTileTime * maxCount + 0.03f),
                                         CCCallFuncN::create(this, callfuncN_selector(Box::afterAllMoveDone)), NULL));
    CCLOG("-F Box::repairCallback");
}


void Box::burstTile(Tile2 *tile, float burstDelay) {
    CCLOG("+F Box::burstTile tile %d,%d with delay %f", tile->x, tile->y, burstDelay);
    if (readyToRemoveTiles->containsObject(tile)) {
        return;
    }
    tile->burstDelay = burstDelay;
    readyToRemoveTiles->addObject(tile);
    if (tile->type == StripedHorizontal) {
        CCLOG("Horizontal burst %d,%d\n", tile->x, tile->y);
        for(int i = 0; i < size.width; ++i) {
            burstTile(this->objectAtX(i, tile->y), burstDelay + abs(tile->x - i)*kBurstPropogationTime  );
        }
        
    } else if (tile->type == StripedVertical) {
        CCLOG("Vertical burst %d,%d\n", tile->x, tile->y);
        for(int i = 0; i < size.height; ++i) {
            burstTile(this->objectAtX(tile->x, i), burstDelay + abs(tile->y - i)*kBurstPropogationTime);
        }
    }
    CCLOG("-F Box::burstTile tile %d,%d with delay %f", tile->x, tile->y, burstDelay);
}

bool Box::checkTilesToClear() {
    
    this->checkWith(OrientationHori);
    this->checkWith(OrientationVert);
    
    return false;
}

void Box::checkCombinations() {
    
}

/**
 * Checks the box for potential matches
 * either horizontally or vertically
 */
void Box::checkWith(Orientation orient)
{
    CCLOG("+F Box::checkWith()");
	int iMax = (orient == OrientationHori) ? size.width: size.height;
	int jMax = (orient == OrientationHori) ? size.height: size.width;
	for (int i=0; i<iMax; i++) {
		int count = 0;
		int vv = -1;
		CCArray *matches = CCArray::createWithCapacity(jMax);
        
		for (int j=0; j<jMax; j++) {
			Tile2 *tile = this->objectAtX(((orient == OrientationHori)? i:j), ((orient == OrientationHori)? j :i));
            
            if (tile->value == 0) {
                
                readyToRemoveTiles->addObject(tile);
                
            } else if(tile->value == vv){
                count++;
                matches->addObject(tile);
			} else {
                // current streak has been broken
                if(count == 3) {
                    // Just a normal match3
                    for(int i=0; i<3; i++) {
                        Tile2 *obj = (Tile2 *)matches->objectAtIndex(i);
                        obj->matchType = Normal;
                        burstTile(obj, 0.0f);
                    }
                } else if (count == 4) {
                    // Striped one
                    int spawnX = -1;
                    int spawnY = -1;
                    int value = -1;
                    BalloonType matchType = (orient == OrientationHori)? StripedHorizontal : StripedVertical;
                    for(int i=0; i<4; i++) {
                        Tile2 *obj = (Tile2 *)matches->objectAtIndex(i);
                        obj->matchType = matchType;
                        burstTile(obj, 0.0f);
                        spawnX = obj->x;
                        spawnY = obj->y;
                        value = obj->value;
                    }
                    // Lets add that to the spawn array too
                    Tile2 *new_tile = Tile2::create();
                    new_tile->x = spawnX;
                    new_tile->y = spawnY;
                    new_tile->value = value;
                    new_tile->_debug_isOriginal = false;
                    new_tile->type = matchType;
                    readyToChangeTiles->addObject(new_tile);
                } else if (count > 4) {
                    for(int i=0; i<count; i++) {
                        Tile2 *obj = (Tile2 *)matches->objectAtIndex(i);
//                        obj->matchType = matchType;
                        burstTile(obj, 0.0f);
                    }
                }
                
				count = 1;
                matches->removeAllObjects();
                matches->addObject(tile);
				vv = tile->value;
			}
        }
        matches->release();
    }
    CCLOG("+F Box::checkWith()");
}


void Box::removeSprite(CCNode * sender)
{
    CCLOG("+F Box::removeSprite()");
    CCAssert(layer->getChildren()->containsObject(sender), "Child doesn't exist");
    layer->removeChild( sender, true);
    CCLOG("-F Box::removeSprite()");
}

/**
 * callback which gets invoked when all moves are done 
 * in one check() call
 */
void Box::afterAllMoveDone(CCNode * sender)
{
    CCLOG("+F Box::afterAllMoveDone()");
    // Check if due to the new tiles, there are more matches
    // if yes, repeat the process otherwise unlock the box
   if(this->check())
   {
     
   }
   else
   {
       this->unlock();
   }
   CCLOG("-F Box::afterAllMoveDone()");
}

void Box::unlock()
{
   this->lock = false;
}

CCFiniteTimeAction* Box::createPlayPieceSwiggle(int moves){
    if ( 0 == moves ) {
        return CCDelayTime::create(0.0);
    } else if ( moves <= 4) {
        int angle = 10 + moves * 5;
        if (rand() % 2 == 0) {
            angle *= -1;
        }
        return CCSequence::create(CCEaseOut::create(CCRotateBy::create( (kMoveTileTime*moves)/4.0, angle),1),
                           CCEaseInOut::create(CCRotateBy::create( (kMoveTileTime*moves)/2.0, angle * -2),1),
                           CCEaseIn::create(CCRotateTo::create(kMoveTileTime*moves/4.0,0),1),
                           NULL);
    } else {
        int first = rand() % (moves -1) + 1;
        int second = moves - first;
        return CCEaseInOut::create(CCSequence::create(
                                                      this->createPlayPieceSwiggle(first),
                                                      this->createPlayPieceSwiggle(second),
                                                      NULL),1);
    }
}

CCFiniteTimeAction* Box::createPlayPieceMovement(int moves) {
    return  CCSpawn::createWithTwoActions(
                                          CCEaseInOut::create(CCMoveBy::create(kMoveTileTime * (moves), ccp(0, kTileSize *( moves))),2),
                                          this->createPlayPieceSwiggle(moves));
}

CCFiniteTimeAction* Box::createPlayPieceAction(int index, int total) {
    return CCSequence::create(
                              CCDelayTime::create(kMoveTileTime * (total - (index))),
                              CCFadeIn::create(kTileFadeInTime),
                              this->createPlayPieceMovement(index),
                              NULL);
}


/**
 * Repair the columns one by one to fill in missing
 * tiles which got deleted due to some match
 */
int Box::repair()
{
   CCLOG("+F Box::repair()");
   int maxCount = 0;
   for (int x=0; x<size.width; x++) {
       int count = this->repairSingleColumn(x);
       if (count > maxCount) {
           maxCount = count;
       }
   }
   CCLOG("-F Box::repair()");
   return maxCount;
}

int Box::repairSingleColumn(int columnIndex)
{
   CCLOG("+F Box::repairSingleColumn(%d)", columnIndex);
   int extension = 0;
    
    // If there were deleted tiles then running the drop
    // animation for the column so that new tiles can be
    // added on t
   for (int y = size.height - 1; y >=0 ; --y) {
       Tile2 *tile = this->objectAtX(columnIndex, y);
       if(tile->value == 0) {
           extension++;
       } else if (extension == 0) {
           
       } else {
           Tile2 *destTile = this->objectAtX(columnIndex, y+extension);
           
           CCFiniteTimeAction *action = this->createPlayPieceMovement(extension);
           tile->sprite->runAction(action);
           
           destTile->value = tile->value;
           destTile->sprite = tile->sprite;
       }
    }

    // Creating those extra tiles by randomly generating
    // tile types and running the animation for same
      for (int i = extension - 1; i >= 0 ; --i) {
          int value = rand()%kKindCount + 1;
          //Tile2 *destTile = this->objectAtX(columnIndex, size.height-extension+i);
          Tile2 *destTile = this->objectAtX(columnIndex, i);

          //destTile->retain();
          CCSprite *sprite = Tile2::getBalloonSprite(value, Normal);
          //sprite->retain();
          sprite->setPosition(ccp(kStartX + columnIndex * kTileSize + kTileSize/2, kStartY + kTileSize/2));
          sprite->setOpacity(0);
          CCFiniteTimeAction *action = this->createPlayPieceAction(i, extension);
          
          layer->addChild(sprite);
          sprite->runAction(action);

          destTile->value = value;
          destTile->type = Normal;
          destTile->sprite = sprite;
      }

    CCLOG("-F Box::repairSingleColumn(%d)");
    return extension;
}

Box::~Box(){
     CCLOGERROR("Deleting BOX");
}
