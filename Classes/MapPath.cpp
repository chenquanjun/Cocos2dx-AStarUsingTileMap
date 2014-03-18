//
//  MapPath.cpp
//  testTileMap
//
//  Created by chenquanjun on 14-3-13.
//
//

#include "MapPath.h"

#pragma -mark- map path

MapPath::MapPath(void)
:_pointArr(nullptr),
_startId(-1),
_endId(-1)
{
    
}

MapPath::~MapPath(void){
    CC_SAFE_RELEASE(_pointArr);
}

MapPath* MapPath::create(int startId, int endId, PointArray *pointArr){
    MapPath *pMapPath = new MapPath();
    if (pMapPath && pMapPath->init(startId, endId, pointArr))
    {
        pMapPath->autorelease();
        return pMapPath;
    }
    CC_SAFE_DELETE(pMapPath);
    return nullptr;
}

MapPath*  MapPath::getRevertPath(void){
    
    PointArray *newPointArr = _pointArr->reverse();
    
    return create(_endId, _startId, newPointArr);
}

MapPath* MapPath::getCopy(){
    return create(_startId, _endId, _pointArr->clone());
}

bool MapPath::init(int startId, int endId, PointArray *pointArr){
    CCASSERT(startId >= 0, "id value");
    CCASSERT(endId >= 0, "id value");
    CCASSERT(pointArr->count() > 0, "count error");
    
    _startId = startId;
    _endId = endId;
    setPointArr(pointArr);
    
    return true;
}
