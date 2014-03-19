Cocos2dx-TileMapPlusAStar
=========================

cocos2dx 3.0 beta2 tileMap And A Star

read tileMap info by "tmx" file and create map data. User A-star to create path which used by sprite to walk to a certain position

demo shows 

1、Sprite find path automatically. 

2、touch an available place and sprite walk to target position.

3、move the map and auto fix position

传入地图创建MapInfo

tmx文件读取完相关信息后即释放

tmx文件暂时是写死读取的格式，

layer用background字段，object用object字段，object里面的精灵用objectid标记类型，类型的枚举见MapGeneral.h

通过传入开始和结束ID获得路径

1、第一次开始寻路算法，生成路径后将路径与相反路径保存下来，以后直接调用；

2、暂时只设计了MapInfoType::Block是不可走



