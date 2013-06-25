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
extern std::string play_vstripe_filenames[];
extern std::string play_hstripe_filenames[];
extern std::string play_wrapped_filenames[];
extern std::string play_color_burst_filename;

USING_NS_CC;
class Globals : public CCObject
{
private:
    
public:
    int _startX;
    int _startY;
    
    int getStartX();
    int getStartY();
    void setStartX(int x);
    void setStartY(int y);
    
    virtual bool init();
    CREATE_FUNC(Globals);
};

extern Globals * _global;

#endif /* defined(__MatchThree__common__) */
