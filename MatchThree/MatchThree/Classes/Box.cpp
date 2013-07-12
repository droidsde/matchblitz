//
//  Box.cpp
//  MatchThree
//
//  Created by kpuspesh on 6/2/13.
//
//

#include "Box.h"

using namespace cocos2d;


Box::~Box(){
    CCLOGERROR("Deleting BOX");
}

bool Box::init()
{
    // do initialization here
    return true;
}

bool Box::initWithSize (CCSize aSize, int aFactor)
{
    size = aSize;
    _swappedTileA = NULL;
    _swappedTileB = NULL;
    
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
    unstableTiles = CCArray::create();
	readyToRemoveTiles->retain();
    readyToChangeTiles->retain();
    unstableTiles->retain();

	return true;
}

bool Box::drawBG(int x, int y){
    CCSprite *sprite = CCSprite::create(tile_bg_filename.c_str());
    sprite->setScale(kTileSize/sprite->getContentSize().width);
    sprite->setPosition(ccp(x * kTileSize + kTileSize/2, y * kTileSize + kTileSize/2));
    sprite->setOpacity(kTileBGOpacity);
    layer->addChild(sprite,0);
    return true;  // not in use for now
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

    // Checks tiles to clear and puts them in array
    // in addition to the new tile which need to be
    // spawned so that we can run the animations before
    // repair-ing the whole box
    ret = this->checkTilesToClear();

    // Goes ahead with running all the burst/spawn
    // animations and callbacks to repair logic
    ret = this->runEffectSequence();

    clearBurstDelay();

    CCLog("-F Box::check()");
    return ret;
}

bool Box::checkTilesToClear() {
    int callingOrder = 0;
    
    if(this->checkForSpecialSwaps() == false) {
        
        this->checkWith(OrientationHori, ++callingOrder);
        this->checkWith(OrientationVert, ++callingOrder);
    }
    
    return false;
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
            tile->sprite->stopAllActions();
            CCFiniteTimeAction *action = CCSequence::create(
                                                            CCDelayTime::create(tile->burstDelay),
                                                            CCScaleTo::create(0.3f, 0.0f),
                                                            CCCallFuncN::create(this, callfuncN_selector(Box::removeSprite)),
                                                            NULL
                                                            );
            tile->sprite->runAction(action);
            
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
        
        //CCLOG("Replacing tile at %d,%d | new_tile->%p org_tile->%p", x, y, new_tile, tile);
        //CCLOG("new_tile ref count %d ", new_tile->m_uReference);
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
        new_sprite->setPosition(ccp(x * kTileSize + kTileSize/2, y * kTileSize + kTileSize/2));
        //new_sprite->setOpacity(0);
        tile->value = value;
        tile->type = type;
        tile->sprite = new_sprite;
        layer->addChild(new_sprite);
        tile->sprite->runAction(CCSequence::create(
                                                   CCDelayTime::create(delay),
                                                   CCFadeIn::create(0.3f),
                                                   NULL));
        
        if(tile->type == WrappedHalfBurst) {
            unstableTiles->addObject(tile);
            
            // Also start the glowing animation on it
            CCSequence *unstableStateAction = CCSequence::create(
                                                                 CCScaleBy::create(0.3f, 0.8f),
                                                                 CCScaleBy::create(0.3f, 1.25f),
                                                                 NULL);
            tile->sprite->runAction(CCRepeatForever::create(unstableStateAction));
        }
        
        new_tile->release();
    }
    
    // temp hack to empty the array of tiles which got removed
    CCLOG("Releasing readyToRemoveTiles");
    //readyToRemoveTiles->removeAllObjects();
    readyToRemoveTiles->release();
    readyToRemoveTiles = CCSet::create();
	readyToRemoveTiles->retain();
    
    CCLOG("Releasing readyToChangeTiles");
    //readyToChangeTiles->removeAllObjects();
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
    if(this->check() == false)
    {
        // All moves are done and box has come to
        // temporary stable state, now lets check
        // for any half burst balloon (if any)
        if(this->checkForWrappedHalfBurst()) {
            if(this->check() == false) {
                this->unlock();
            }
        } else {
            this->unlock();
        }
    }
    CCLOG("-F Box::afterAllMoveDone()");
}

bool Box::isLocked()
{
    return this->_lock;
}

void Box::lock()
{
    this->_lock = true;
}

void Box::unlock()
{
    this->_lock = false;
}

void Box::registerSwappedTiles(Tile2 * first, Tile2 * second)
{
    if(first == NULL || second == NULL) return;
    
    _swappedTileA = first;
    _swappedTileB = second;
}

void Box::deregisterSwappedTiles()
{
    _swappedTileA = NULL;
    _swappedTileB = NULL;
}

/****************************************************
 * PRIVATE Helper functions                         *
 ****************************************************/


/**
 * Checks the box for potential matches
 * either horizontally or vertically
 */
void Box::checkWith(Orientation orient, int order)
{
    CCLOG("+F Box::checkWith()");
	int iMax = (orient == OrientationHori) ? size.width: size.height;
	int jMax = (orient == OrientationHori) ? size.height: size.width;
	for (int i=0; i<iMax; i++) {
		int count = 0;
		int vv = -1;
		CCArray *matches = CCArray::createWithCapacity(jMax);

		for (int j=0; j<jMax; j++) {
			Tile2 *tile = this->objectAtX(((orient == OrientationVert)? i:j), ((orient == OrientationVert)? j :i));

            if(tile->value == 0) {
                readyToRemoveTiles->addObject(tile);
            }
            else if(tile->value == vv){
                count++;
                matches->addObject(tile);
			} else {
                // current streak has been broken
                this->doCombinations(count, matches, orient, order);

				count = 1;
                matches->removeAllObjects();
                matches->addObject(tile);
				vv = tile->value;
			}
        }
        this->doCombinations(count, matches, orient, order);
        matches->removeAllObjects();
        matches->release();
    }
    CCLOG("+F Box::checkWith()");
}

void Box::doCombinations(int count, CCArray * matches, Orientation orient, int order) {
    if(count >= 3) {
        // Striped one
        int spawnX = -1;
        int spawnY = -1;
        int value = -1;
        Tile2 * first = NULL;
        Tile2 * new_tile = NULL;

        first = (Tile2 *) matches->objectAtIndex(0);
        spawnX = first->x;
        spawnY = first->y;
        value = first->value;

        if (count == 4) {
            BalloonType matchType = (orient == OrientationHori)? StripedHorizontal : StripedVertical;

            // Lets add that to the spawn array too
            new_tile = Tile2::create();
            new_tile->x = spawnX;
            new_tile->y = spawnY;
            new_tile->value = value;
            new_tile->_debug_isOriginal = false;
            new_tile->type = matchType;

        } else if (count >= 5) {
            BalloonType matchType = ColorBurst;

            spawnX = (orient == OrientationHori)? spawnX+2 : spawnX;
            spawnY = (orient == OrientationVert)? spawnY+2 : spawnY;

            // Lets add that to the spawn array too
            new_tile = Tile2::create();
            new_tile->x = spawnX;
            new_tile->y = spawnY;
            new_tile->value = value;
            new_tile->_debug_isOriginal = false;
            new_tile->type = matchType;
        }

        for(int i=0; i<count; i++) {
            Tile2 *obj = (Tile2 *)matches->objectAtIndex(i);

            // if this is the second order of match3 check
            // also, verify if wrapped combination (T/L-shape)
            // is possible and take action a/c
            if(order > 1) {
                this->checkForWrappedCombination(&obj, &new_tile, spawnX, spawnY, value);
            }
            burstTile(obj, 0.0f);
        }

        // Make sure that we update the `tileToSpawn` pointer
        // associated with tiles so that next loop can use that
        // to figure out any bigger matches
        if(new_tile != NULL) {
            for(int i=0; i<count; i++) {
                Tile2 *obj = (Tile2 *)matches->objectAtIndex(i);
                obj->tileToSpawn = new_tile;
            }
        }

        if(new_tile) readyToChangeTiles->addObject(new_tile);
    }
}

bool Box::checkForWrappedCombination(Tile2 **obj, Tile2 **new_tile, int spawnX, int spawnY, int value) {
    bool wrapped = false;

    if(readyToRemoveTiles->containsObject(*obj)) {
        spawnX = (*obj)->x;
        spawnY = (*obj)->y;
        
        // check the match type and take decision
        // to spawn wrapped one instead, if possible
        if((*obj)->tileToSpawn) {
            if((*obj)->tileToSpawn->type == ColorBurst) {
                (*new_tile)->release();
                (*new_tile) = NULL;
            } else {
                if((*new_tile) != NULL) (*new_tile)->release();
                if(readyToChangeTiles->containsObject((*obj)->tileToSpawn)) {
                    readyToChangeTiles->removeObject((*obj)->tileToSpawn);
                }
                (*obj)->tileToSpawn->release();
                (*obj)->tileToSpawn = NULL;
                // need to create wrapped one
                (*new_tile) = Tile2::create();
                (*new_tile)->x = spawnX;
                (*new_tile)->y = spawnY;
                (*new_tile)->value = value;
                (*new_tile)->_debug_isOriginal = false;
                (*new_tile)->type = Wrapped;

                wrapped = true;
            }
        } else {
            if((*new_tile) != NULL) (*new_tile)->release();
            // need to create wrapped one
            (*new_tile) = Tile2::create();
            (*new_tile)->x = spawnX;
            (*new_tile)->y = spawnY;
            (*new_tile)->value = value;
            (*new_tile)->_debug_isOriginal = false;
            (*new_tile)->type = Wrapped;

            wrapped = true;
        }
    }
    return wrapped;
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
        bool flag = false;
        Tile2 *tile = this->objectAtX(columnIndex, y);
        if(unstableTiles->containsObject(tile)) flag = true;
        
        if(tile->value == 0) {
            extension++;
        } else if (extension == 0) {
            
        } else {
            Tile2 *destTile = this->objectAtX(columnIndex, y+extension);
            
            CCFiniteTimeAction *action = this->createPlayPieceMovement(extension);
            tile->sprite->runAction(action);
            
            destTile->value = tile->value;
            destTile->sprite = tile->sprite;
            destTile->tileToSpawn = NULL;
            destTile->type = tile->type;
            if(flag) {
                unstableTiles->removeObject(tile);
                unstableTiles->addObject(destTile);
            }
        }
    }
    
    // Creating those extra tiles by randomly generating
    // tile types and running the animation for same
    for (int i = extension - 1; i >= 0 ; --i) {
        int value = rand()%kKindCount + 1;
        Tile2 *destTile = this->objectAtX(columnIndex, i);
        
        CCSprite *sprite = Tile2::getBalloonSprite(value, Normal);
        sprite->setPosition(ccp(columnIndex * kTileSize + kTileSize/2, kTileSize/2));
        sprite->setOpacity(0);
        CCFiniteTimeAction *action = this->createPlayPieceAction(i, extension);
        
        layer->addChild(sprite);
        sprite->runAction(action);
        
        destTile->value = value;
        destTile->type = Normal;
        destTile->sprite = sprite;
        destTile->tileToSpawn = NULL;
    }
    
    CCLOG("-F Box::repairSingleColumn(%d)");
    return extension;
}


void Box::burstTile(Tile2 *tile, float burstDelay) {
    CCLOG("+F Box::burstTile tile %d,%d with delay %f", tile->x, tile->y, burstDelay);
    if (readyToRemoveTiles->containsObject(tile)) {
        return;
    }
    
    tile->burstDelay = burstDelay;
    readyToRemoveTiles->addObject(tile);
    
    // Do the chaining effect of burst action here
    // For various special combos and power ups, behavior
    // is controlled by the conditions below
    
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
    } else if (tile->type == Wrapped || tile->type == WrappedHalfBurst) {
        CCLOG("Wrapped burst %d,%d\n", tile->x, tile->y);
        if(tile->x - 1 >= 0) {
            if(tile->y - 1 >= 0) {
                burstTile(this->objectAtX(tile->x-1, tile->y-1), 0.0f);
            }
            burstTile(this->objectAtX(tile->x-1, tile->y), 0.0f);
            if(tile->y + 1 < kBoxHeight) {
                burstTile(this->objectAtX(tile->x-1, tile->y+1), 0.0f);
            }
        }
        if(tile->x + 1 < kBoxWidth) {
            if(tile->y - 1 >= 0) {
                burstTile(this->objectAtX(tile->x+1, tile->y-1), 0.0f);
            }
            burstTile(this->objectAtX(tile->x+1, tile->y), 0.0f);
            if(tile->y + 1 < kBoxHeight) {
                burstTile(this->objectAtX(tile->x+1, tile->y+1), 0.0f);
            }
        }
        if(tile->y + 1 < kBoxHeight) {
            burstTile(this->objectAtX(tile->x, tile->y+1), 0.0f);
        }
        if(tile->y - 1 >= 0) {
            burstTile(this->objectAtX(tile->x, tile->y-1 ), 0.0f);
        }
        
        if(tile->type == Wrapped) {
            // Also, append the half burst one at the original location
            Tile2 * new_tile = Tile2::create();
            new_tile->x = tile->x;
            new_tile->y = tile->y;
            new_tile->value = tile->value;
            new_tile->_debug_isOriginal = false;
            new_tile->type = WrappedHalfBurst;
            readyToChangeTiles->addObject(new_tile);
        }
    }
    CCLOG("-F Box::burstTile tile %d,%d with delay %f", tile->x, tile->y, burstDelay);
}

bool Box::checkForWrappedHalfBurst() {
    if(unstableTiles->count() == 0) return false;
    
    for (int i=0; i<unstableTiles->count(); i++) {
        Tile2 *curTile = (Tile2 *) unstableTiles->objectAtIndex(i);
        burstTile(curTile, kHalfBurstTileDelayTime);
    }
    unstableTiles->removeAllObjects();
    return true;
}

bool Box::checkForSpecialSwaps()
{
    if(_swappedTileA == NULL || _swappedTileB == NULL) return false;
    
    if(_swappedTileB->type == StripedVertical || _swappedTileB->type == StripedHorizontal) {
        Tile2 *tmp = _swappedTileB;
        _swappedTileB = _swappedTileA;
        _swappedTileA = tmp;
    }
    
    if(_swappedTileA->type == StripedHorizontal || _swappedTileA->type == StripedVertical) {
        
        if(_swappedTileB->type == StripedHorizontal || _swappedTileB->type == StripedVertical) {
            // Striped ones are being swapped by the user
            // one row and column needs to go, decide which ones.
            burstTile(_swappedTileA, 0.0f);
            _swappedTileB->type = Tile2::getOpposite(_swappedTileA->type);
            burstTile(_swappedTileB, 0.0f);
            return true;
        }
        
        if(_swappedTileB->type == Wrapped) {
            // Striped is being swapped with a wrapped one
            // 3 rows and columns need to go, decide which ones.
            
            // run the zoom animation
            // Also start the glowing animation on it
            vanishTile(this->objectAtX(_swappedTileB->x-1, _swappedTileB->y-1));
            vanishTile(this->objectAtX(_swappedTileB->x-1, _swappedTileB->y));
            vanishTile(this->objectAtX(_swappedTileB->x-1, _swappedTileB->y+1));
            vanishTile(this->objectAtX(_swappedTileB->x, _swappedTileB->y-1));
            //vanishTile(this->objectAtX(_swappedTileB->x, _swappedTileB->y));
            vanishTile(this->objectAtX(_swappedTileB->x, _swappedTileB->y+1));
            vanishTile(this->objectAtX(_swappedTileB->x+1, _swappedTileB->y-1));
            vanishTile(this->objectAtX(_swappedTileB->x+1, _swappedTileB->y));
            vanishTile(this->objectAtX(_swappedTileB->x+1, _swappedTileB->y+1));

            CCSequence *unstableStateAction = CCSequence::create(
                                                                 CCScaleBy::create(0.5f, 1.25f),
                                                                 NULL);
            _swappedTileA->sprite->runAction(unstableStateAction);
            
            // burstTile(this->objectAtX(_swappedTileB->x, _swappedTileB->y), 0.0f);
            return true;
        }
        
        if(_swappedTileB->type == ColorBurst) {
            // Striped one is being swapped with color bomb
            // need to convert tiles to striped and execute the burst one by one
            // also, pause everything else and handle this first
            
            return true;
        }
        
    }
    
    if (_swappedTileA->type == Wrapped && _swappedTileB->type == Wrapped) {
        
    }
    
    if (_swappedTileA->type == ColorBurst && _swappedTileB->type == Wrapped) {
        
    }
    
    if (_swappedTileB->type == ColorBurst && _swappedTileA->type == Wrapped) {
        
    }
    
    return false;
}

void Box::playBurst(CCNode* sender, void* data) {
    Tile2 *tile = static_cast<Tile2 *>(data);
    CCLOG("Scaling tile %d,%d with delay %f", tile->x, tile->y);
    CCFiniteTimeAction *action = CCSequence::create(
                                                    CCScaleTo::create(0.3f, 0.0f),
                                                    CCCallFuncN::create(this, callfuncN_selector(Box::removeSprite)),
                                                    NULL
                                                    );
    tile->sprite->runAction(action);


    // Add the balloon burst effect
    CCParticleSystemQuad *burst = CCParticleSystemQuad::create(burst_effect_filename.c_str());
    burst->setPosition(tile->pixPosition());
    burst->setAutoRemoveOnFinish(true);
    layer->addChild(burst);
}

CCFiniteTimeAction* Box::createPlayPieceSwiggle(int moves){
    if ( 0 == moves ) {
        return CCDelayTime::create(0.0);
    } else if ( moves <= 4) {
        int angle = (rand()%10) + moves * 5;
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

void Box::clearBurstDelay(){
    CCLog("F clearBurstDelay()");
    for (int y=0; y<size.height; y++) {
		for (int x=0; x < size.width; x++) {
            this->objectAtX(x, y)->burstDelay = 0.0f;
        }
    }
}

float Box::getMaxBurstDelay(){
    float maxDelay = 0.0f;
    for (int y=0; y<size.height; y++) {
		for (int x=0; x < size.width; x++) {
            Tile2 *tmp = this->objectAtX(x, y);
            maxDelay = MAX(maxDelay,tmp->burstDelay);
        }
    }
    return maxDelay;
}

void Box::vanishTile(Tile2 * tile) {
    if (tile->sprite) {
        tile->sprite->stopAllActions();
        CCFiniteTimeAction *action = CCSequence::create(
                                                        CCScaleTo::create(0.3f, 0.0f),
                                                        CCCallFuncN::create(this, callfuncN_selector(Box::removeSprite)),
                                                        NULL
                                                        );
        tile->sprite->runAction(action);
    }
}
