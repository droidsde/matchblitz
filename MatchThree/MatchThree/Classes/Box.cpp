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
    OutBorderTile = Tile2::create();
    OutBorderTile->retain();
    OutBorderTile->initWithX(-1, -1);
	
    content = CCArray::createWithCapacity(size.height);
    
	for (int y=0; y<size.height; y++) {
		CCArray *rowContent = CCArray::createWithCapacity(size.width);
		for (int x=0; x < size.width; x++) {
			Tile2 *tile = Tile2::create();
            tile->retain();
            tile->initWithX(x, y);
			rowContent->addObject(tile);
		}
        rowContent->retain();
		content->addObject(rowContent);
	}
    content->retain();
	
	readyToRemoveTiles = CCSet::create();
	readyToRemoveTiles->retain();
  
	return this;
}

Tile2 * Box::objectAtX(int x, int y)
{
	if (x < 0 || x >= kBoxWidth || y < 0 || y >= kBoxHeight) {
		return OutBorderTile;
	}
    CCArray * tmp = (CCArray *) content->objectAtIndex(y); // revisit @PUSPESH
	return (Tile2*) tmp->objectAtIndex(x);
}


bool Box::check() {
    this->checkWith(OrientationHori);
    this->checkWith(OrientationVert);
    
    if (readyToRemoveTiles->count() == 0) {
        return false;
    }
    int sizeOfSet = readyToRemoveTiles->count();
    CCSetIterator itr = readyToRemoveTiles->begin();
    for (; itr != readyToRemoveTiles->end(); itr++) {
        Tile2 *tile = (Tile2 *) *itr;
        tile->value = 0;
        if (tile->sprite) {
            CCAction *action = CCSequence::create(
                                                            CCScaleBy::create(kMoveTileTime, 0.5f),
                                                            CCScaleBy::create(kMoveTileTime, 2.0f),
                                                            CCCallFuncN::create(tile->sprite, callfuncN_selector(Box::removeSprite)));
            tile->sprite->runAction(action);
        }
    }
    
    sizeOfSet = readyToRemoveTiles->count();
    readyToRemoveTiles->removeAllObjects();
    readyToRemoveTiles->release();
    readyToRemoveTiles = CCSet::create();
	readyToRemoveTiles->retain();
    sizeOfSet = readyToRemoveTiles->count();
    int maxCount = this->repair();
    
    layer->runAction( CCSequence::create(
                                         CCDelayTime::create(kMoveTileTime * maxCount + 0.03f),
                                         CCCallFuncN::create(this, callfuncN_selector(Box::afterAllMoveDone))));
    
    return true;
}

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
               
void Box::afterAllMoveDone(CCNode * sender)
{
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
    
   for (int y=0; y<size.height; y++) {
       Tile2 *tile = this->objectAtX(columnIndex, y);
       if(tile->value == 0) {
           extension++;
       } else if (extension == 0) {
           
       } else {
           Tile2 *destTile = this->objectAtX(columnIndex, y-extension);
           
           CCSequence *action = CCSequence::create(CCMoveBy::create(kMoveTileTime*extension, ccp(0,-kTileSize*extension)));
           tile->sprite->runAction(action);
           
           destTile->value = tile->value;
           destTile->sprite = tile->sprite;
       }
    }
          
      for (int i=0; i<extension; i++) {
          int value = (arc4random()%kKindCount+1);
          Tile2 *destTile = this->objectAtX(columnIndex, kBoxHeight-extension+i);
          CCString *name = CCString::createWithFormat("block_%d.png", value);
          CCSprite *sprite = CCSprite::create(name->getCString());
          sprite->setPosition(ccp(kStartX + columnIndex * kTileSize + kTileSize/2, kStartY + (kBoxHeight + i) * kTileSize + kTileSize/2));
          CCAction *action = CCMoveBy::create(kMoveTileTime*extension, ccp(0,-kTileSize*extension));
          
          layer->addChild(sprite);
          sprite->runAction(action);

          destTile->value = value;
          destTile->sprite = sprite;
      }
     
    return extension;
  }
