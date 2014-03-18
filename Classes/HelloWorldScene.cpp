#include "HelloWorldScene.h"
#include "TestSprite.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    _mapLayer = Layer::create();
    this->addChild(_mapLayer);
    

    do {
        auto map = TMXTiledMap::create("map3.tmx");
        _mapLayer->addChild(map);
        
        //object图层
        auto objectGrp = map->getObjectGroup("object");
        
        auto layer = map->getLayer("background");
        
        Size layerSize = layer->getLayerSize();

        
        //所有对象
        auto& objects = objectGrp->getObjects();
   
        //tileMap 与 数据结构 转换
        for (auto& obj : objects) {
            ValueMap& dict = obj.asValueMap();
            
            float x = dict["x"].asFloat() / dict["width"].asFloat();
            float y = dict["y"].asFloat() / dict["height"].asFloat();
         
            int mapId = x + y * layerSize.width;
            
            __String *str = __String::createWithFormat("%i", mapId);
            LabelTTF *label = LabelTTF::create(str->getCString(), "Arial", 10);
            label->setPosition(dict["x"].asFloat() + 15, dict["y"].asFloat() + 18);
            label->setColor(Color3B::BLACK);
//            this->addChild(label, 100);
          
        }
    } while (0);
    
    
    auto pMapInfo = MapInfo::create("map3.tmx");
    _mapInfo = pMapInfo;
    pMapInfo->retain();
    
    _nTarget1 = findRandomTarget();
    _nTarget2 = findRandomTarget();
    _nTarget3 = findRandomTarget();
    _nTarget4 = findRandomTarget();
    
    auto pMathPath1 = pMapInfo->getMapPath(findRandomTarget(), _nTarget1);
    auto pMathPath2 = pMapInfo->getMapPath(findRandomTarget(), _nTarget2);
    auto pMathPath3 = pMapInfo->getMapPath(findRandomTarget(), _nTarget3);
    auto pMathPath4 = pMapInfo->getMapPath(findRandomTarget(), _nTarget4);
    
    
    PointArray *pointArr1 = pMathPath1->getPointArr();
    PointArray *pointArr2 = pMathPath2->getPointArr();
    PointArray *pointArr3 = pMathPath3->getPointArr();
    PointArray *pointArr4 = pMathPath4->getPointArr();
//    Size pointSize = pMapInfo->getPointSize();

    
    do {//测试动画

        //将纹理提前加入精灵帧缓存
        SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
        frameCache->addSpriteFramesWithFile("player1.plist");
        
        Size winSize = Director::getInstance()->getWinSize();
        
        for (int j = 0; j < 4; j++) {
            Vector<SpriteFrame*> animFrames;
            
            for(int i = 0; i < 4; i++)
            {
                __String * str = __String::createWithFormat("player1_%i_%i.png", j, i);
                SpriteFrame *frame = frameCache->getSpriteFrameByName(str->getCString());
                
                animFrames.pushBack(frame);
            }
            
            Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
            
            __String * str = __String::createWithFormat(PLAYER_ANIM_ACTION_STR, j);
            AnimationCache::getInstance()->addAnimation(animation, str->getCString());
            
            //test
            Sprite *animSprite = Sprite::createWithSpriteFrameName("player1_0_0.png");
            auto *anim = Animate::create(animation);
            animSprite->runAction(RepeatForever::create(anim));
            _mapLayer->addChild(animSprite);
            animSprite->setPosition(Point(winSize.width * 0.1 * (j + 1), winSize.height * 0.8f));
        }
 

    } while (0);
    
    EaseWalkTo *easeWalkTo1 = EaseWalkTo::create(3, pointArr1);
    EaseWalkTo *easeWalkTo2 = EaseWalkTo::create(3, pointArr2);
    EaseWalkTo *easeWalkTo3 = EaseWalkTo::create(3, pointArr3);
    EaseWalkTo *easeWalkTo4 = EaseWalkTo::create(3, pointArr4);
    //创建一个精灵用于显示动画
//    Sprite *animSprite = Sprite::createWithSpriteFrameName("player1_0_0.png");
    
    //将纹理提前加入精灵帧缓存
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("player1.plist");
    frameCache->addSpriteFramesWithFile("player2.plist");
    frameCache->addSpriteFramesWithFile("player3.plist");
    frameCache->addSpriteFramesWithFile("player4.plist");
    
    TestSprite *testSprite1 = TestSprite::create("player1_%i_%i.png");
    TestSprite *testSprite2 = TestSprite::create("player2_%i_%i.png");
    TestSprite *testSprite3 = TestSprite::create("player3_%i_%i.png");
    TestSprite *testSprite4 = TestSprite::create("player4_%i_%i.png");
    
    testSprite1->setAnchorPoint(Point(0.5, 0));
    testSprite2->setAnchorPoint(Point(0.5, 0));
    testSprite3->setAnchorPoint(Point(0.5, 0));
    testSprite4->setAnchorPoint(Point(0.5, 0));
    
    _mapLayer->addChild(testSprite1, 200);
    _mapLayer->addChild(testSprite2, 200);
    _mapLayer->addChild(testSprite3, 200);
    _mapLayer->addChild(testSprite4, 200);
    
    Sequence *seq1 = Sequence::create(
                                     easeWalkTo1,
                                     CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionDone1,this)),
                                     NULL
                                     );
    
    Sequence *seq2 = Sequence::create(
                                      easeWalkTo2,
                                      CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionDone2,this)),
                                      NULL
                                      );
    
    Sequence *seq3 = Sequence::create(
                                      easeWalkTo3,
                                      CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionDone3,this)),
                                      NULL
                                      );
    
    Sequence *seq4 = Sequence::create(
                                      easeWalkTo4,
                                      CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionDone4,this)),
                                      NULL
                                      );
    
    testSprite1->runAction(seq1);
    testSprite2->runAction(seq2);
    testSprite3->runAction(seq3);
    testSprite4->runAction(seq4);
    
    
    do {//touch
        TestSprite *testSprite = TestSprite::create("player1_%i_%i.png");
        testSprite->setAnchorPoint(Point(0.5, 0));
        
        _mapLayer->addChild(testSprite, 200);
        
        int targetId = this->findRandomTarget();
        
        testSprite->setPosition(_mapInfo->convetIdToPointMid(targetId));
        
        //触摸
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto myListener = EventListenerTouchOneByOne::create();
        
        //如果不加入此句消息依旧会向下传递
        myListener->setSwallowTouches(true);
        
        myListener->onTouchBegan = [=](Touch* touch,Event* event)
        {
            
//            Point touchPoint = touch->getLocation();
//            Point firstTouchPoint = touch->getStartLocation();
//            int mapId = this->_mapInfo->convertPointToId(touch->getLocation());
//            
//            Point position = testSprite->getPosition();
//            int originId = _mapInfo->convertPointToId(position);
//            
//            MapPath* pMapPath = _mapInfo->getMapPath(originId, mapId);
//            
//            //some check
//            if (pMapPath != nullptr)
//            {
//                PointArray *pointArr1 = pMapPath->getPointArr();
//                
//                float duration = 0.2 * pointArr1->count();
//                
//                EaseWalkTo *easeWalkTo1 = EaseWalkTo::create(duration, pointArr1);
//                
//                testSprite->stopAllActions();
//                testSprite->runAction(easeWalkTo1);
//
            _bIsMove = false;
            return true;
//            }
//            return false;
        };
        
        myListener->onTouchMoved = [=](Touch* touch,Event* event)
        {
            //do something
            Point touchPoint = _mapLayer->convertToNodeSpace(touch->getLocation()); ;
            Point firstTouchPoint = _mapLayer->convertToNodeSpace(touch->getStartLocation());
            Point preTouchPoint = _mapLayer->convertToNodeSpace(touch->getPreviousLocation());
            
            if (touchPoint.getDistance(firstTouchPoint) > 5){
                _bIsMove = true;

                
            }else{

            }
            
            if (_bIsMove) {
                Point position  = _mapLayer->getPosition();
                _mapLayer->setPosition(Point(position.x + touchPoint.x - preTouchPoint.x, position.y + touchPoint.y - preTouchPoint.y));
            }
        };
        
        //touch ended
        myListener->onTouchEnded = [=](Touch* touch,Event* event)
        {  
            //do something
            
            Point touchPoint = _mapLayer->convertToNodeSpace(touch->getLocation()); ;
            
            if (!_bIsMove) {
                int mapId = this->_mapInfo->convertPointToId(touchPoint);
                
                Point position = testSprite->getPosition();
                int originId = _mapInfo->convertPointToId(position);
                
                MapPath* pMapPath = _mapInfo->getMapPath(originId, mapId);
                
                //some check
                if (pMapPath != nullptr)
                {
                    PointArray *pointArr1 = pMapPath->getPointArr();
                    
                    float duration = 0.2 * pointArr1->count();
                    
                    EaseWalkTo *easeWalkTo1 = EaseWalkTo::create(duration, pointArr1);
                    
                    testSprite->stopAllActions();
                    testSprite->runAction(easeWalkTo1);
                    
                }
            }else{

            }
            
            _mapLayer->stopAllActions();
//            _mapLayer->runAction(EaseBackOut::create(MoveTo::create(0.5f, Point(0, 0))));

        };
        
        dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
        
    } while (0);
    return true;
}

void HelloWorld::actionDone1(Node *pSender){

    
    int newTarget = findRandomTarget();

    MapPath* pMathPath1 = _mapInfo->getMapPath(_nTarget1, newTarget);
    
//    Point prePoint = _mapInfo->convertIdToPoint(_nTarget1);
//    Point newPoint = _mapInfo->convertIdToPoint(newTarget);
    
//    CCLOG("target1:%i, %i, pre:%.1f, %.1f, new:%.1f, %.1f", _nTarget1, newTarget, prePoint.x + 16,prePoint.y + 16,newPoint.x + 16 ,newPoint.y + 16);
    
    _nTarget1 = newTarget;
    
    PointArray *pointArr1 = pMathPath1->getPointArr();
    
    float duration = 0.2 * pointArr1->count();
    
    EaseWalkTo *easeWalkTo1 = EaseWalkTo::create(duration, pointArr1);

    
    Sequence *seq = Sequence::create(
                                     easeWalkTo1,
                                     CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionDone1,this)),
                                     NULL
                                     );
    
    pSender->runAction(seq);
}

void HelloWorld::actionDone2(Node *pSender){
    int newTarget = findRandomTarget();
    
    MapPath* pMathPath = _mapInfo->getMapPath(_nTarget2, newTarget);
    
//    CCLOG("target2:%i, %i", _nTarget2, newTarget);
    
    _nTarget2 = newTarget;
    
    PointArray *pointArr = pMathPath->getPointArr();
    
    float duration = 0.2 * pointArr->count();
    
    EaseWalkTo *easeWalkTo = EaseWalkTo::create(duration, pointArr);
    
    
    Sequence *seq = Sequence::create(
                                     easeWalkTo,
                                     CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionDone2,this)),
                                     NULL
                                     );
    
    pSender->runAction(seq);
}

void HelloWorld::actionDone3(Node *pSender){
    int newTarget = findRandomTarget();
    
    MapPath* pMathPath = _mapInfo->getMapPath(_nTarget3, newTarget);
    
//    CCLOG("target3:%i, %i", _nTarget3, newTarget);
    
    _nTarget3 = newTarget;
    
    PointArray *pointArr = pMathPath->getPointArr();
    
    float duration = 0.2 * pointArr->count();
    
    EaseWalkTo *easeWalkTo = EaseWalkTo::create(duration, pointArr);
    
    
    Sequence *seq = Sequence::create(
                                     easeWalkTo,
                                     CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionDone3,this)),
                                     NULL
                                     );
    
    pSender->runAction(seq);
}

void HelloWorld::actionDone4(Node *pSender){
    int newTarget = findRandomTarget();
    
    MapPath* pMathPath = _mapInfo->getMapPath(_nTarget4, newTarget);
    
//    CCLOG("target4:%i, %i", _nTarget2, newTarget);
    
    _nTarget4 = newTarget;
    
    PointArray *pointArr = pMathPath->getPointArr();
    
    float duration = 0.2 * pointArr->count();
    
    EaseWalkTo *easeWalkTo = EaseWalkTo::create(duration, pointArr);
    
    
    Sequence *seq = Sequence::create(
                                     easeWalkTo,
                                     CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionDone4,this)),
                                     NULL
                                     );
    
    pSender->runAction(seq);
}

int HelloWorld::findRandomTarget(){
    auto roadVec = _mapInfo->getMapInfoTypeVec(MapInfoType::Road);
    int count = roadVec.size();
    
    
    float r = CCRANDOM_0_1();
    
    if (r == 1) // to prevent from accessing data-arr[data->num], out of range.
    {
        r = 0;
    }
    
    
    int randomTarget = r * count;
    
    auto value = roadVec.at(randomTarget);
    
    int newTarget = value.asInt();
    
    return newTarget;
}

void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
