//
//  MapPath.h
//  testTileMap
//
//  Created by chenquanjun on 14-3-13.
//
//

#ifndef __testTileMap__MapPath__
#define __testTileMap__MapPath__

#include "cocos2d.h"
#include "MapGeneral.h"
USING_NS_CC;

//地图路径
class MapPath : public Object{
    
public:
    
    //开始id，结束id，路径点
    static MapPath* create(int startId, int endId, PointArray* pointArr);
    
    MapPath* getRevertPath(void);//相反路径
    
    MapPath* getCopy();//复制备份
    
private:
    MapPath(void);
    
    virtual ~MapPath(void);
    
    virtual bool init(int startId, int endId, PointArray* pointArr);
    
    
    CC_SYNTHESIZE_READONLY(int, _startId, StartId);
    CC_SYNTHESIZE_READONLY(int, _endId, EndId);
    CC_SYNTHESIZE_RETAIN(PointArray*, _pointArr, PointArr)
};

#endif /* defined(__testTileMap__MapPath__) */
