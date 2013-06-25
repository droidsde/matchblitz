//
//  constants.h
//  MatchThree
//
//  Created by kpuspesh on 6/2/13.
//
//

#ifndef MatchThree_constants_h
#define MatchThree_constants_h

#define kTileSize 78.0f
#define kPieceHeight 68.0f
#define kPieceWidth 68.0f
#define kMoveTileTime 0.3f
#define kBoxWidth 9
#define kBoxHeight 9
#define kStartX 310
#define kStartY 25
#define kStartXAlternate 25
#define kStartYAlternate 25
#define kMaxLevelNo 10
#define kMaxRecordCount 5
#define kKindCount 6
#define kTileFadeInTime 0.3f
#define kTileBGOpacity 210 // between 0 and 255
#define kRepairDelayTime 0.3f
#define kBurstPropogationTime 0.08f // Time it takes for burst to travel;
#define kHalfBurstTileDelayTime 0.1f

enum Orientation{
	OrientationHori,
	OrientationVert,
};
typedef enum Orientation Orientation;


#endif
