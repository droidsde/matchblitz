//
//  common.h
//  MatchThree
//
//  Created by Ankit Jain on 6/8/13.
//
//

#ifndef __MatchThree__common__
#define __MatchThree__common__

#include "cocos2d.h"
#include <iostream>
#include "constants.h"
extern std::string play_filenames[];
extern std::string tile_bg_filename;
extern std::string bg_filename;

extern std::string burst_effect_filename;
extern std::string burst_smoke_effect_filename;

extern std::string play_vstripe_filenames[];
extern std::string play_hstripe_filenames[];
extern std::string play_wrapped_filenames[];
extern std::string play_color_burst_filename;


extern const char* SWIPE_START;
extern const char* SWIPE_DONE;
extern const char* SWIPE_CANCELLED;
extern const char* BALLOON_CONVERTED;
extern const char* STRIPED_CREATED;
extern const char* WRAPPED_CREATED;
extern const char* COLORBURST_CREATED;
extern const char* HALFWRAPPED_CREATED;
extern const char* SPAWN_STARTED;
extern const char* SPAWN_DONE;
extern const char* MOVEMENT_STARTED;
extern const char* MOVEMENT_DONE;
extern const char* BALLOON_BURST_NORMAL;
extern const char* BALLOON_BURST_STRIPED;
extern const char* BALLOON_BURST_WRAPPED;
extern const char* BALLOON_BURST_COLORBURST;

typedef std::pair<const char *, std::string> EffectMapPair;
typedef std::map<const char*, std::string> EffectMap;

USING_NS_CC;
class Globals : public CCObject
{
private:
    
public:
    int _startX;
    int _startY;
    EffectMap sound_effects;
    EffectMap particle_effects;
    
    int getStartX();
    int getStartY();
    void setStartX(int x);
    void setStartY(int y);
    
    std::string getSoundEffect(const char*);
    
    virtual bool init();
    CREATE_FUNC(Globals);
};

extern Globals * _global;

#endif /* defined(__MatchThree__common__) */
