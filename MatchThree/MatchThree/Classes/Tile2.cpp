//
//  Tile.cpp
//  MatchThree
//
//  Created by kpuspesh on 6/2/13.
//
//

#import "Tile2.h"

bool Tile2::init()
{
    value = 99;
    return true;
}

bool Tile2::initWithX (int posX, int posY)
{
    this->x = posX;
	this->y = posY;
    this->retain();
    return true;
}

bool Tile2::nearTile (Tile2 *othertile)
{
	return
	(x == othertile->x && abs(y - othertile->y)==1)
	||
	(y == othertile->y && abs(x - othertile->x)==1);
}

void Tile2::trade (Tile2 * otherTile)
{
    this->sprite->retain();
    CCSprite *tempSprite = this->sprite;
	int tempValue = this->value;
    this->sprite = otherTile->sprite;
	this->value = otherTile->value;
	otherTile->sprite = tempSprite;
	otherTile->value = tempValue;
    tempSprite->release();
}

CCPoint Tile2::pixPosition()
{
	return ccp(kStartX + x * kTileSize +kTileSize/2.0f,kStartY + y * kTileSize +kTileSize/2.0f);
}

bool Tile2::operator==(const Tile2 &other) const {
    return ((other.x == x) && (other.y ==y));
}
