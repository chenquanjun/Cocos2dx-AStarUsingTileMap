//
//  MapGeneral.h
//  testAStar
//
//  Created by chenquanjun on 14-3-14.
//
//

#ifndef testAStar_MapGeneral_h
#define testAStar_MapGeneral_h

//强类型枚举
enum class MapInfoType{
    Invalid   =  0, //无效
    Block     = -1, //石头等障碍物
    Road      =  1, //路径
    Seat      =  8, //座位
    Start     = 10, //开始坐标
    Thing     = 11, //物体
};

enum class PlayerActionType{
    Invalid    = 0, //无效
    Down       = 1, //下
    Left       = 2, //左
    Right      = 3, //右
    Up         = 4, //上
    Max        = 5, //
};

#define PLAYER_ANIM_ACTION_STR "anim_action_%i"

#endif
