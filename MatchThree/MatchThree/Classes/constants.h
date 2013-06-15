//
//  constants.h
//  MatchThree
//
//  Created by kpuspesh on 6/2/13.
//
//

#ifndef MatchThree_constants_h
#define MatchThree_constants_h

#define kTileSize 75.0f
#define kPieceHeight 60.0f
#define kPieceWidth 60.0f
#define kMoveTileTime 0.3f
#define kBoxWidth 9
#define kBoxHeight 9
#define kStartX 200
#define kStartY 60
#define kMaxLevelNo 10
#define kMaxRecordCount 5
#define kKindCount 4
#define kTileFadeInTime 0.3f
#define kTileBGOpacity 180 // between 0 and 255

enum Orientation{
	OrientationHori,
	OrientationVert,
};
typedef enum Orientation Orientation;


#endif
