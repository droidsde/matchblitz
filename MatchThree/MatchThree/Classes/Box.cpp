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

bool Box::initWithSize (CCSize aSize, int aFactor)
{
    size = aSize;
    OutBorderTile = new Tile2();
    OutBorderTile->retain();
    OutBorderTile->initWithX(-1, -1);
	
    content = CCArray::createWithCapacity(size.height);
    
    // Initialize the n x m array with
    // tile objects
	for (int y=0; y<size.height; y++) {
		CCArray *rowContent = CCArray::createWithCapacity(size.width);
		for (int x=0; x < size.width; x++) {
			Tile2 *tile = new Tile2();
            tile->initWithX(x, y);
            tile->autorelease();
			rowContent->addObject(tile);
		}
        rowContent->retain();
		content->addObject(rowContent);
	}
    content->retain();
	
    // Empty array/set to hold tiles to remove at any point in time
	readyToRemoveTiles = CCSet::create();
	readyToRemoveTiles->retain();
  
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
    CCArray * tmp = (CCArray *) content->objectAtIndex(y);
	return (Tile2 *) tmp->objectAtIndex(x);
}

/**
 * Checks the status of the box to figure out if there
 * are some matches already and do we need to clean some 
 * tiles
 */
bool Box::check() {
    this->checkWith(OrientationHori);
    this->checkWith(OrientationVert);
    
    if (readyToRemoveTiles->count() == 0) {
        return false;       // nothing to do, no matches in current state
    }
    
    // Go and remove the tiles which are marked for removal (due to match3+)
    // also runs small animation on the tile sprite
    CCSetIterator itr = readyToRemoveTiles->begin();
    for (; itr != readyToRemoveTiles->end(); itr++) {
        Tile2 *tile = (Tile2 *) *itr;
        tile->value = 0;
        if (tile->sprite) {
            CCFiniteTimeAction *action = CCSequence::create(
                                                    CCScaleTo::create(0.3f, 0.0f),
                                                    CCCallFuncN::create(tile, callfuncN_selector(Box::removeSprite)),
                                                    NULL
                                                );
            tile->sprite->runAction(action);
        }
    }
    
    // temp hack to empty the array of tiles which got removed
    readyToRemoveTiles->removeAllObjects();
    //readyToRemoveTiles->release();
    readyToRemoveTiles = CCSet::create();
	readyToRemoveTiles->retain();
    
    // REPAIR the box after matched tiles were deleted
    int maxCount = this->repair();
    
    layer->runAction( CCSequence::create(
                                         CCDelayTime::create(kMoveTileTime * maxCount + 0.03f),
                                         CCCallFuncN::create(this, callfuncN_selector(Box::afterAllMoveDone)), NULL));
    
    return true;
}

/**
 * Checks the box for potential matches 
 * either horizontally or vertically
 */
void Box::checkWith(Orientation orient)
{
	int iMax = (orient == OrientationHori) ? size.width: size.height;
	int jMax = (orient == OrientationHori) ? size.height: size.width;
	for (int i=0; i<iMax; i++) {
		int count = 0;
		int vv = -1;
		first = NULL;
		second = NULL;
		for (int j=0; j<jMax; j++) {
			Tile2 *tile = this->objectAtX(((orient == OrientationHori)? i:j), ((orient == OrientationHori)? j :i));
			if(tile->value == vv)
            {
				count++;
				if (count > 3) {
					readyToRemoveTiles->addObject(tile);
				}
                else
                {
					if (count == 3) {
						readyToRemoveTiles->addObject(first);
						readyToRemoveTiles->addObject(second);
						readyToRemoveTiles->addObject(tile);
						first = NULL;
						second = NULL;
					}
                    else if (count == 2) {
						second = tile;
					}else {
						//
					}
                }
			} else {
				count = 1;
				first = tile;
				second = NULL;
				vv = tile->value;
			}
        }
    }
}


void Box::removeSprite(CCNode * sender)
{
   layer->removeChild( sender, true);
}

/**
 * callback which gets invoked when all moves are done 
 * in one check() call
 */
void Box::afterAllMoveDone(CCNode * sender)
{
    // Check if due to the new tiles, there are more matches
    // if yes, repeat the process otherwise unlock the box
   if(this->check())
   {
     
   }
   else
   {
       this->unlock();
   }
   
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
   int maxCount = 0;
   for (int x=0; x<size.width; x++) {
       int count = this->repairSingleColumn(x);
       if (count > maxCount) {
           maxCount = count;
       }
   }
   return maxCount;
}

int Box::repairSingleColumn(int columnIndex)
{
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
          int value = (arc4random()%kKindCount+1);
          //Tile2 *destTile = this->objectAtX(columnIndex, size.height-extension+i);
          Tile2 *destTile = this->objectAtX(columnIndex, i);

          //destTile->retain();
          CCSprite *sprite = CCSprite::create(play_filenames[value-1].c_str());
          sprite->setScale(kPieceWidth/sprite->getContentSize().width);
          sprite->retain();
          // sprite->setPosition(ccp(kStartX + columnIndex * kTileSize + kTileSize/2, kStartY + (kBoxHeight + i) * kTileSize + kTileSize/2));
          sprite->setPosition(ccp(kStartX + columnIndex * kTileSize + kTileSize/2, kStartY + kTileSize/2));
          sprite->setOpacity(0);
          CCFiniteTimeAction *action = this->createPlayPieceAction(i, extension);
          
          layer->addChild(sprite);
          sprite->runAction(action);

          destTile->value = value;
          destTile->sprite = sprite;
      }

     
    return extension;
  }
