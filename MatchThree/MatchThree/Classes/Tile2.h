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

enum BalloonType{
    None,
	Normal,
    StripedVertical,
    StripedHorizontal,
    Wrapped,
    WrappedHalfBurst,
    ColorBurst
};
typedef enum BalloonType BalloonType;

USING_NS_CC;
class Tile2 : public CCObject
{
private:
    
public:
    int x, y;
	int value;
    BalloonType type;
    float burstDelay;
    bool _debug_isOriginal;
    
	CCSprite *sprite;
    BalloonType matchType;
    
    
    bool initWithX(int posX, int posY);
    bool nearTile(Tile2 * othertile);
    void trade(Tile2 * otherTile);
    CCPoint pixPosition();
    static CCSprite* getBalloonSprite(int value, BalloonType type);
    bool operator==(const Tile2 &other) const;

    virtual bool init();
    virtual void release(void);
    CREATE_FUNC(Tile2);
    ~Tile2();
};

USING_NS_CC;
class TileMatch : public CCObject
{
private:
    
public:
    Tile2 * startTile;
    int length;
    Orientation direction;
    
    bool intersects(TileMatch *obj);
    
    virtual bool init();
    CREATE_FUNC(TileMatch);
    ~TileMatch();
};

#endif
