//
//  Tile.h
//  MatchThree
//
//  Created by kpuspesh on 6/2/13.
//
//

#ifndef MatchThree_Tile_h
#define MatchThree_Tile_h

#import "cocos2d.h"
#import "common.h"

USING_NS_CC;
class Tile2 : public CCObject
{
private:
    
public:
    int x, y;
	int value;
	CCSprite *sprite;
    
    virtual bool init();
    bool initWithX(int posX, int posY);
    
    bool nearTile(Tile2 * othertile);
    void trade(Tile2 * otherTile);
    CCPoint pixPosition();

    CREATE_FUNC(Tile2);
};

#endif
