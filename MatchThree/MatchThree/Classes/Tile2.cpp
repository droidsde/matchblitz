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
    type = Normal;
    tileToSpawn = NULL;
    return true;
}

bool Tile2::initWithX (int posX, int posY)
{
    this->x = posX;
	this->y = posY;
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
    BalloonType tempType = this->type;
    this->sprite = otherTile->sprite;
	this->value = otherTile->value;
    this->type = otherTile->type;
	otherTile->sprite = tempSprite;
	otherTile->value = tempValue;
    otherTile->type = tempType;
    tempSprite->release();
}

CCPoint Tile2::pixPosition()
{
	return ccp(x * kTileSize +kTileSize/2.0f, y * kTileSize +kTileSize/2.0f);
}

bool Tile2::operator==(const Tile2 &other) const {
    return ((other.x == x) && (other.y ==y));
}

CCSprite* Tile2::getBalloonSprite(int value, BalloonType type) {
    CCSprite *sprite = NULL;
    ccTexParams tex_params = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE };
    switch (type) {
        case Normal:
            CCAssert(value <= kKindCount && value > 0, "Invalid color");
            sprite = CCSprite::create(play_filenames[value - 1].c_str());
            sprite->setScale(kPieceWidth/sprite->getContentSize().width);
            sprite->getTexture()->generateMipmap();
            sprite->getTexture()->setTexParameters(&tex_params);
            break;
            
        case StripedVertical:
            CCAssert(value <= kKindCount && value > 0, "Invalid color");
            sprite = CCSprite::create(play_vstripe_filenames[value - 1].c_str());
            sprite->setScale(kPieceWidth/sprite->getContentSize().width);
            sprite->getTexture()->generateMipmap();
            sprite->getTexture()->setTexParameters(&tex_params);
            break;
          
        case StripedHorizontal:
            CCAssert(value <= kKindCount && value > 0, "Invalid color");
            sprite = CCSprite::create(play_hstripe_filenames[value - 1].c_str());
            sprite->setScale(kPieceWidth/sprite->getContentSize().width);
            sprite->getTexture()->generateMipmap();
            sprite->getTexture()->setTexParameters(&tex_params);
            break;

        case WrappedHalfBurst:
        case Wrapped:
            CCAssert(value <= kKindCount && value > 0, "Invalid color");
            sprite = CCSprite::create(play_wrapped_filenames[value - 1].c_str());
            sprite->setScale(kPieceWidth/sprite->getContentSize().width);
            sprite->getTexture()->generateMipmap();
            sprite->getTexture()->setTexParameters(&tex_params);
            break;
    
        case ColorBurst:
            sprite = CCSprite::create(play_color_burst_filename.c_str());
            sprite->setScale(kPieceWidth/sprite->getContentSize().width);
            sprite->getTexture()->generateMipmap();
            sprite->getTexture()->setTexParameters(&tex_params);
            break;
            
        default:
            break;
    }
    return sprite;
}

Tile2::~Tile2() {
    CCLOG("DELETING tile at %d, %d  %s -> %p", x, y, (_debug_isOriginal)? "Old":"New", this);
    //CCAssert(!_debug_isOriginal, "Trying to delete original box tile");
}

void Tile2::release() {
    
    CCLOG("Releasing tile at %d, %d  %s -> %p", x, y, (_debug_isOriginal)? "Old":"New", this);
    
    //gejCCAssert(!(_debug_isOriginal && (this->m_uReference + this->m_uAutoReleaseCount< 2)), "Releasing old tile to 0");
    if (!_debug_isOriginal && (x || y)) {
        CCLOG("Releaseing new tile");
    }

    CCObject::release();
}
