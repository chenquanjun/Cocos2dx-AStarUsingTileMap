//
//  MapGeneral.h
//  testAStar
//
//  Created by chenquanjun on 14-3-14.
//
//

#ifndef testAStar_MapGeneral_h
#define testAStar_MapGeneral_h

//地图信息类
//传入地图创建MapInfo
//tmx文件读取完相关信息后即释放
//tmx文件暂时是写死读取的格式，
//layer用background字段，object用object字段，object里面的精灵用objectid标记类型，类型的枚举见MapGeneral.h

//通过传入开始和结束ID获得路径
//1、第一次开始寻路算法，生成路径后将路径与相反路径保存下来，以后直接调用；
//2、暂时只设计了MapInfoType::Block是不可走

//强类型枚举
//地图信息枚举，此处的数字是根据tmx文件的object层的精灵的objectid决定
enum class MapInfoType{
    Invalid   =  0, //无效
    Block     = -1, //石头等障碍物
    Road      =  1, //路径
    Seat      =  8, //座位
    Start     = 10, //开始坐标
    Thing     = 11, //物体
};

//动作类型枚举 此处的枚举与plist文件的顺序相同
enum class PlayerActionType{
    Invalid    = 0, //无效
    Down       = 1, //下
    Left       = 2, //左
    Right      = 3, //右
    Up         = 4, //上
    Max        = 5, //
};

//A星寻路的方向数目
#define MAPINFO_ASTAR_DIRECTION_NUM 4
//A星寻路的最大寻路步数
#define MAPINFO_MAX_STEP_NUM 10000
//A星寻路的map最大缓存路径数目
#define MAPINFO_MAX_STORE_PATH_SIZE 10000

#endif
