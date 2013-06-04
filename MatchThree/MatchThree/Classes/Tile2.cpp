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
    x = posX;
	y = posY;
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
    CCSprite *tempSprite = sprite;
	int tempValue = value;
    sprite = otherTile->sprite;
	value = otherTile->value;
	otherTile->sprite = tempSprite;
	otherTile->value = tempValue;
	free(tempSprite);
}

CCPoint Tile2::pixPosition()
{
	return ccp(kStartX + x * kTileSize +kTileSize/2.0f,kStartY + y * kTileSize +kTileSize/2.0f);
}
