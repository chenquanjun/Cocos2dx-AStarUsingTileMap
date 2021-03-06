//
//  MapInfo.h
//  testTileMap
//
//  Created by chenquanjun on 14-3-12.
//
//

#ifndef __testTileMap__MapInfo__
#define __testTileMap__MapInfo__

#include "cocos2d.h"
#include "MapPath.h"
#include "MapGeneral.h"
USING_NS_CC;

class MapInfo : public Object{
public:
    //使用tmx文件创建
    static MapInfo* create(const std::string& filename);
    
    //获得地图路径
    MapPath* getMapPath(int startId, int endId);
    
    //获得点坐标形式的路径
    PointArray* getPointPath(int startId, int endId);
    
    //获得相应类型的信息对应表
    ValueVector getMapInfoTypeVec(MapInfoType type);
    
    //根据类型获得随机mapId
    int getRandomMapIdByType(MapInfoType type);
    
    //根据类型获得随机地图网格的点
    Point getRandomPointByType(MapInfoType type);
    
    //根据类型获得随机地图网格的中点
    Point getRandomPointMidByType(MapInfoType type);
    
    //把点转换成mapId
    int convertPointToId(Point point);
    
    //把mapid转换成点坐标(网格的左下角)
    Point convertIdToPoint(int mapId);
    
    //把mapid转换成点坐标(网格中心，适合精灵使用)
    Point convertIdToPointMid(int mapId);
    
    //地图大小(PS:此处的大小是网格大小)
    CC_SYNTHESIZE_READONLY(Size, _mapGridSize, MapGridSize);
    //点大小
    CC_SYNTHESIZE_READONLY(Size, _pointSize, PointSize);

private:
    //A星点结构
    struct PointNode
    {
        PointNode()  {nIndex = 0; nG = 0; pParent = NULL;}
        int nIndex;
        int nG;
        PointNode* pParent;
    };
    
private:
    MapInfo(void);
    
    virtual ~MapInfo(void);
    
    virtual bool init(const std::string& filename);
    
    int GetIndexByDir(int nIndex, int nDir);
    int GetGByIndex(int nStartIndex, int nEndIndex);
    int GetHByIndex(int nIndex);
    PointNode *InTable(int nIndex, std::vector<PointNode*> &vecTbl);
    
    //A星寻路
    MapPath *findPath();
    //从缓存获得路径
    MapPath *getPathFromCache();
 
private:
    int m_nStartIndex;
    int m_nEndIndex;
    
    ValueVector _mapInfoTypeVec; //传入地图后转化成vector保存
    
    ValueMapIntKey _mapInfoTypeMap;//根据类型保存在字典里面
    
    Map<int, MapPath*> _mapPathCacheMap; //已经生成的路径缓存
    
    int m_nKeyOffset;//保存key值的偏移
    
};

#endif /* defined(__testTileMap__MapInfo__) */
