//
//  TileMatch.h
//  MatchThree
//
//  Created by kpuspesh on 6/24/13.
//
//

#ifndef MatchThree_TileMatch_h
#define MatchThree_TileMatch_h

#include "cocos2d.h"
#include "common.h"

USING_NS_CC;
class TileMatch : public CCObject
{
private:
    
public:
    int length;
    int startTileX;
    int startTileY;
    Orientation direction;
    
    bool intersects(TileMatch *obj);
    
    virtual bool init();
    CREATE_FUNC(TileMatch);
};

#endif
