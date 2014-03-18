//
//  MapInfo.cpp
//  testTileMap
//
//  Created by chenquanjun on 14-3-12.
//
//

#include "MapInfo.h"

MapInfo::MapInfo(void){
    _mapGridSize = Size(0, 0);
    _pointSize = Size(0, 0);
    
    m_nKeyOffset = 10000;
}

MapInfo::~MapInfo(void){
    
}

MapInfo* MapInfo::create(const std::string& filename){
    MapInfo *pMapInfo = new MapInfo();
    if (pMapInfo && pMapInfo->init(filename))
    {
        pMapInfo->autorelease();
        return pMapInfo;
    }
    CC_SAFE_DELETE(pMapInfo);
    return nullptr;
}


bool MapInfo::init(const std::string& filename){
    
    //地图
    auto map = TMXTiledMap::create(filename);
    
    //object图层
    auto objectGrp = map->getObjectGroup("object");
    
    auto layer = map->getLayer("background");
    
    _mapGridSize = layer->getLayerSize();//地图大小
 
    //所有对象
    auto& objects = objectGrp->getObjects();
    
    int capacity = _mapGridSize.width * _mapGridSize.height;
    
    ValueMap& dict = objects.at(0).asValueMap();
    _pointSize = Size(dict["width"].asFloat(), dict["height"].asFloat());//单元大小
    
    int mapType[capacity];//临时储存信息
    
    //tileMap 与 数据结构 转换
    //PS:1、dict取出来是乱序的。2、从左下角开始
    for (auto& obj : objects) {
        ValueMap& dict = obj.asValueMap();
        
        float x = dict["x"].asFloat() / _pointSize.width;
        float y = dict["y"].asFloat() / _pointSize.height;
        
        int objectId = dict["objectid"].asInt();
        

        int mapId = x + y * _mapGridSize.width;
        
//        CCLOG("%f, %f, %i, %i", x, y ,objectId, mapId);
        
        mapType[mapId] = objectId;//id对应类型
        
    }
    
    //数组转化成vector保存
    for (int i = 0; i < capacity; i++) {
        int num = mapType[i];
        
        //此处断言是因为tmx文件里面的object位置偏位导致不存在信息，从而为0
        CCASSERT(num != 0, "objectId not set");
        
        Value pNum = Value(num);
        _mapInfoTypeVec.push_back(pNum); //vector保存
    }
    return true;
}

MapPath* MapInfo::getMapPath(int startId, int endId){
    m_nStartIndex = startId;
    m_nEndIndex = endId;
    
    MapPath* path = this->getPathFromCache();//从缓存寻找
    
    if (path == nullptr) {
        //生成新的path
        path = this->findPath();
    }
    
    
    
    return path;
}

PointArray* MapInfo::getPointPath(int startId, int endId){
    return getMapPath(startId, endId)->getPointArr();
}

ValueVector MapInfo::getMapInfoTypeVec(MapInfoType type){
    ValueVector typeVec;
    int typeNum = (int)type;
    int i = 0;
    for (auto& value: _mapInfoTypeVec) {
        int testNum = value.asInt();
        if (testNum == typeNum) {
            typeVec.push_back(Value(i));
        }
        i++;
    }
    
    return typeVec;
}

int MapInfo::convertPointToId(Point point){
    int mapId = -1;
    //在大地图内, 此处的转换是按照左下角往右扩展，然后再往上扩展形式计算，例如
    // 8 9 ...
    // 4 5 6 7
    // 0 1 2 3
//    if (Rect(0, 0, _mapSize.width, _mapSize.height).containsPoint(point)) {
        int x = point.x / _pointSize.width;
        int y = point.y / _pointSize.height;
        mapId = x + y * _mapGridSize.width;
//    }
    
//    CCLOG("point:%f,%f, id:%i", point.x ,point.y, mapId);
    return mapId;
}

Point MapInfo::convertIdToPoint(int mapId){
    Point point = Point(0, 0);
    
    if (mapId >= 0 && mapId < _mapInfoTypeVec.size()) {
        int y = (mapId / (int)_mapGridSize.width); //y坐标
        int x = (mapId - y * _mapGridSize.width) ; //x坐标

        point = Point(x  * _pointSize.width, y * _pointSize.height);
    }
//    CCLOG("id:%i, point:%f,%f", mapId, point.x ,point.y);
    return point;
}

Point MapInfo::convetIdToPointMid(int mapId){
    Point point = convertIdToPoint(mapId);
    return Point(point.x + _pointSize.width * 0.5f, point.y + _pointSize.height * 0.5f);
}


MapPath* MapInfo::findPath(){
    std::vector<PointNode*> vecClose;
    std::vector<PointNode*> vecOpen;
    
    if (m_nStartIndex == -1 || m_nEndIndex == -1)
    {
        return nullptr;
    }
    
    //如果目标在障碍列表里面返回空
    auto &pointTypeStart = _mapInfoTypeVec.at(m_nStartIndex); //vector保存的地图信息
    if (pointTypeStart.asInt() == (int)MapInfoType::Block)    // 障碍
    {
        return nullptr;
    }
    
    auto &pointTypeEnd = _mapInfoTypeVec.at(m_nEndIndex); //vector保存的地图信息
    if (pointTypeEnd.asInt() == (int)MapInfoType::Block)    // 障碍
    {
        return nullptr;
    }
    
    // 先添加开始点
    PointNode* pNode = new PointNode;
    pNode->nIndex = m_nStartIndex;
    vecClose.push_back(pNode);
    
    int nStep = 0;
    while(true)
    {
        //注意网格数目较大的话需要修改此值，防止寻路路径失败
        if (nStep++ >= MAPINFO_MAX_STEP_NUM)
        {
            break;
        }
        PointNode* pNextNode = vecClose[vecClose.size() - 1];
        if (!pNextNode)
        {
            break;//没有
        }
        if (pNextNode->nIndex == m_nEndIndex)
        {
            break;//目标
        }
        
        //改成8的话是8个方向，此处只使用4个方向
        for (int i = 0; i < MAPINFO_ASTAR_DIRECTION_NUM; i++)
        {
            int nIndex = GetIndexByDir(pNextNode->nIndex, i);
            if (-1 == nIndex)
            {
                continue;
            }
            
            auto &pointType = _mapInfoTypeVec.at(nIndex); //vector保存的地图信息
            if (pointType.asInt() == (int)MapInfoType::Block)    // 障碍
            {
                continue;
            }
            if (InTable(nIndex, vecClose) != nullptr)      // 在close表里
            {
                continue;
            }
            
            PointNode* pNode = InTable(nIndex, vecOpen);     // 在open表里
            if (pNode)
            {
                int nNewG = pNextNode->nG + GetGByIndex(pNextNode->nIndex, pNode->nIndex);
                if (pNode->nG > nNewG)
                {
                    pNode->nG = nNewG;
                    pNode->pParent = pNextNode;
                }
                continue;
            }
            
            // 新搜索到的格子
            pNode = new PointNode;
            pNode->nIndex = nIndex;
            pNode->nG = pNextNode->nG + GetGByIndex(pNextNode->nIndex, pNode->nIndex);
            pNode->pParent = pNextNode;
            vecOpen.push_back(pNode);
        }
        
        int nMinF = 0xFFFFFF;
        pNextNode = nullptr;
        int nNextNodeIndex = 0;
        for (int i = 0; i < (int)vecOpen.size(); i++)
        {
            PointNode* pNode = vecOpen[i];
            if (!pNode)
            {
                continue;
            }
            int nH = GetHByIndex(pNode->nIndex);
            int nF = nH + pNode->nG;
            if (nF < nMinF)
            {
                nMinF = nF;
                pNextNode = pNode;
                nNextNodeIndex = i;
            }
        }
        if (nNextNodeIndex >= 0 && nNextNodeIndex < (int)vecOpen.size())
        {
            vecClose.push_back(pNextNode);
            vecOpen.erase(vecOpen.begin() + nNextNodeIndex);
        }
    }
    
    // 寻路结束，找最优路径
    pNode = vecClose[vecClose.size() - 1];
    
    PointArray *pathArr = PointArray::create(0);//创建传入的数字没有使用
    
//    float pointWidth = _pointSize.width;
//    float pointHeight = _pointSize.height;
//    float offsetX = pointWidth * 0.5f;
//    float offsetY = pointHeight * 0.5f;
    
    //此处生成的是逆向的的array
    while (pNode)
    {
//        int y = pNode->nIndex / (int)_mapGridSize.width; //y坐标
//        int x = pNode->nIndex - y * _mapGridSize.width; //x坐标
        
        int mapId = pNode->nIndex;
        Point point = this->convetIdToPointMid(mapId);//map id 转换成 网格中点坐标
        
//        Point point = Point(x * pointWidth + offsetX, y * pointHeight + offsetY);//方格中点
        pathArr->addControlPoint(point);
        
        pNode = pNode->pParent;//此处变成逆向
    }

    
    //逆向路径
    MapPath* pathRevert = MapPath::create(m_nEndIndex, m_nStartIndex , pathArr);
    MapPath* path = pathRevert->getRevertPath(); //上面变成逆向
    
    int keyRevert = m_nEndIndex * m_nKeyOffset + m_nStartIndex;
    int key = m_nStartIndex * m_nKeyOffset + m_nEndIndex;

    //使用开始点x10000+结束点作为key值缓存字典
    _mapPathCacheMap.insert(key, path);
    _mapPathCacheMap.insert(keyRevert, pathRevert);//反向缓存
//    CCLOG("cache path:%i, %i", m_nStartIndex, m_nEndIndex);
    
    //init
    
    //此处需要释放
    do {
        for (std::vector<PointNode *>::iterator it = vecClose.begin(); it != vecClose.end(); it ++)
            if (NULL != *it)
            {
                delete *it;
                *it = NULL;
            }
        vecClose.clear();
    } while (0);
    
    do {
        for (std::vector<PointNode *>::iterator it = vecOpen.begin(); it != vecOpen.end(); it ++)
            if (NULL != *it)
            {
                delete *it;
                *it = NULL;
            }
        vecOpen.clear();
    } while (0);
//    vecClose.clear();
//    vecOpen.clear();
    m_nStartIndex = -1;
    m_nEndIndex = -1;
    
    return path->getCopy();
}

MapPath* MapInfo::getPathFromCache(){
    auto path = _mapPathCacheMap.at(m_nStartIndex * m_nKeyOffset + m_nEndIndex);
   
    if (path == nullptr) {
        return nullptr;
    }else{
        return (MapPath*)path->getCopy();
    }
    
}

int MapInfo::GetIndexByDir(int nIndex, int nDir)
{
    if (nIndex < 0 || nIndex >= (int)_mapGridSize.width * _mapGridSize.height)
    {
        return -1;
    }
    
    int nRow = nIndex / _mapGridSize.width;
    int nCol = nIndex % (int)_mapGridSize.width;
    
    switch(nDir)
    {
        case 0:     // 上
            nRow += 1;
            break;
        case 1:     // 右
            nCol += 1;
            break;
        case 2:     // 下
            nRow -= 1;
            break;
        case 3:     // 左
            nCol -= 1;
            break;
        case 4:     // 右上
            nRow += 1;
            nCol +=1;
            break;
        case 5:     // 右下
            nRow -= 1;
            nCol += 1;
            break;

        case 6:     // 左下
            nRow -= 1;
            nCol -= 1;
            break;

        case 7:     // 左上
            nRow += 1;
            nCol -= 1;
            break;
        default:
            break;
    }
    if (nRow < 0 || nRow >= _mapGridSize.height
        || nCol < 0 || nCol >= _mapGridSize.width)
    {  
        return -1;  
    }  
    return nRow * _mapGridSize.width + nCol;
}

int MapInfo::GetGByIndex(int nStartIndex, int nEndIndex){
    int nStartRow = nStartIndex / _mapGridSize.width;
    int nStartCol = nStartIndex %  (int)_mapGridSize.width;
    
    int nEndRow = nEndIndex /  _mapGridSize.width;
    int nEndCol = nEndIndex % (int)_mapGridSize.width;
    
    if (nStartRow == nEndRow || nStartCol == nEndCol)
    {
        return 10;
    }
    return 14;
}

int MapInfo::GetHByIndex(int nIndex)
{
    int nRow = nIndex / _mapGridSize.width;
    int nCol = nIndex % (int)_mapGridSize.width;
    
    int nEndRow = m_nEndIndex / _mapGridSize.width;
    int nEndCol = m_nEndIndex % (int)_mapGridSize.width;
    
    return (abs(nEndRow - nRow) + abs(nEndCol - nCol))*10;
}

MapInfo::PointNode *MapInfo::InTable(int nIndex, std::vector<PointNode*> &vecTbl)
{
    for (int i = 0; i < (int)vecTbl.size(); i++)
    {
        if (nIndex == vecTbl[i]->nIndex)
        {
            return vecTbl[i];
        }
    }
    return nullptr;
}

