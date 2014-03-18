#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "MapInfo.h"
#include "WalkTo.h"
#include "EaseWalkTo.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void actionDone1(Node *pSender);
    
    void actionDone2(Node *pSender);
    
    void actionDone3(Node *pSender);
    
    void actionDone4(Node *pSender);
    
    int findRandomTarget();
    
    int _nTarget1, _nTarget2, _nTarget3, _nTarget4;
    
//    int _nTargetTouch;
    MapInfo *_mapInfo;
    
    Layer *_mapLayer;
    
    bool _bIsMove;
};

#endif // __HELLOWORLD_SCENE_H__
