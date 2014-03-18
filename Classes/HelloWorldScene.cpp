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
    //随机种子
    srand((unsigned)time(NULL));

    //地图layer
    _mapLayer = Layer::create();
    this->addChild(_mapLayer);
    

    //显示地图用
    auto map = TMXTiledMap::create("map3.tmx");
    _mapLayer->addChild(map);
 
    //创建地图信息
    auto pMapInfo = MapInfo::create("map3.tmx");
    _mapInfo = pMapInfo;
    pMapInfo->retain();
    

    //将纹理提前加入精灵帧缓存
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("player1.plist");
    frameCache->addSpriteFramesWithFile("player2.plist");
    frameCache->addSpriteFramesWithFile("player3.plist");
    frameCache->addSpriteFramesWithFile("player4.plist");
    
    do {//批量生成
        for (int i = 0; i < 20; i++) {
            int startId1 = findRandomTarget();
            
            Point startPoint1 = _mapInfo->convetIdToPointMid(startId1);
            
            TestSprite *testSprite1 = TestSprite::create("player1_%i_%i.png");
            
            testSprite1->setPosition(startPoint1);
            
            testSprite1->setAnchorPoint(Point(0.5, 0));

            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
    } while (0);
    
    do {//批量生成
        for (int i = 0; i < 20; i++) {
            int startId1 = findRandomTarget();
            
            Point startPoint1 = _mapInfo->convetIdToPointMid(startId1);
            
            TestSprite *testSprite1 = TestSprite::create("player2_%i_%i.png");
            
            testSprite1->setPosition(startPoint1);
            
            testSprite1->setAnchorPoint(Point(0.5, 0));
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
    } while (0);
    
    do {//批量生成
        for (int i = 0; i < 20; i++) {
            int startId1 = findRandomTarget();
            
            Point startPoint1 = _mapInfo->convetIdToPointMid(startId1);
            
            TestSprite *testSprite1 = TestSprite::create("player3_%i_%i.png");
            
            testSprite1->setPosition(startPoint1);
            
            testSprite1->setAnchorPoint(Point(0.5, 0));
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
    } while (0);
    
    do {//批量生成
        for (int i = 0; i < 20; i++) {
            int startId1 = findRandomTarget();
            
            Point startPoint1 = _mapInfo->convetIdToPointMid(startId1);
            
            TestSprite *testSprite1 = TestSprite::create("player4_%i_%i.png");
            
            testSprite1->setPosition(startPoint1);
            
            testSprite1->setAnchorPoint(Point(0.5, 0));
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
    } while (0);
    
//    int startId1 = findRandomTarget();
//    int startId2 = findRandomTarget();
//    int startId3 = findRandomTarget();
//    int startId4 = findRandomTarget();
//    
//    Point startPoint1 = _mapInfo->convetIdToPointMid(startId1);
//    Point startPoint2 = _mapInfo->convetIdToPointMid(startId2);
//    Point startPoint3 = _mapInfo->convetIdToPointMid(startId3);
//    Point startPoint4 = _mapInfo->convetIdToPointMid(startId4);
//    
//    TestSprite *testSprite1 = TestSprite::create("player1_%i_%i.png");
//    TestSprite *testSprite2 = TestSprite::create("player2_%i_%i.png");
//    TestSprite *testSprite3 = TestSprite::create("player3_%i_%i.png");
//    TestSprite *testSprite4 = TestSprite::create("player4_%i_%i.png");
//    
//    testSprite1->setPosition(startPoint1);
//    testSprite2->setPosition(startPoint2);
//    testSprite3->setPosition(startPoint3);
//    testSprite4->setPosition(startPoint4);
//    
//    testSprite1->setAnchorPoint(Point(0.5, 0));
//    testSprite2->setAnchorPoint(Point(0.5, 0));
//    testSprite3->setAnchorPoint(Point(0.5, 0));
//    testSprite4->setAnchorPoint(Point(0.5, 0));
//    
//    _mapLayer->addChild(testSprite1, 200);
//    _mapLayer->addChild(testSprite2, 200);
//    _mapLayer->addChild(testSprite3, 200);
//    _mapLayer->addChild(testSprite4, 200);
//    
//    this->actionDone(testSprite1);
//    this->actionDone(testSprite2);
//    this->actionDone(testSprite3);
//    this->actionDone(testSprite4);
    
    do {//touch
        //touch 控制精灵
        TestSprite *testSprite = TestSprite::create("player1_%i_%i.png");
        testSprite->setAnchorPoint(Point(0.5, 0));
        testSprite->runAction(RepeatForever::create(Blink::create(0.5, 3)));
        
        _mapLayer->addChild(testSprite, 200);
        
        int targetId = this->findRandomTarget();
        
        Point initPosition = _mapInfo->convetIdToPointMid(targetId);
        
        testSprite->setPosition(initPosition);
        
        //触摸
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto myListener = EventListenerTouchOneByOne::create();
        
        //如果不加入此句消息依旧会向下传递
        myListener->setSwallowTouches(true);
        
        Size mapSize = Size( _mapInfo->getMapGridSize().width * _mapInfo->getPointSize().width, _mapInfo->getMapGridSize().height * _mapInfo->getPointSize().height) ;
        Size winSize = Director::getInstance()->getWinSize();
        
        //touch begin
        myListener->onTouchBegan = [=](Touch* touch,Event* event)
        {
            
            _bIsMove = false;
            return true;
//            return false;
        };
        
        //touch moved
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
                    easeWalkTo1->setTag(99);
                    testSprite->stopActionByTag(99);
                    testSprite->runAction(easeWalkTo1);
                    
                }
            }else{

            }
            
            do {//地图层自动调整
     
                Point mapPoint = _mapLayer->getPosition();
                float x = mapPoint.x;
                float y = mapPoint.y;
                
                Point adjustPoint = Point::ZERO;
                
                if (x > 0) {
                    adjustPoint += Point(-x, 0);
                }else if (x < winSize.width - mapSize.width){
                    adjustPoint += Point(winSize.width - mapSize.width - x, 0);
                }
                
                if (y > 0) {
                    adjustPoint += Point(0, -y);
                }else if (y < winSize.height - mapSize.height){
                    adjustPoint += Point(0, winSize.height - mapSize.height - y);
                }
                _mapLayer->stopAllActions();
                _mapLayer->runAction(EaseBackOut::create(MoveBy::create(0.5f, adjustPoint)));
            } while (0);

        };
        
        dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
        
    } while (0);
    return true;
}

void HelloWorld::actionDone(Node *pSender){
    int newTarget = findRandomTarget();
    
    int oldTarget = _mapInfo->convertPointToId(pSender->getPosition());
    
    MapPath* pMathPath = _mapInfo->getMapPath(oldTarget, newTarget);
    
    PointArray *pointArr = pMathPath->getPointArr();
    
    float duration = 0.2 * pointArr->count();
    
    EaseWalkTo *easeWalkTo1 = EaseWalkTo::create(duration, pointArr);
    
    
    Sequence *seq = Sequence::create(
                                     easeWalkTo1,
                                     CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionDone,this)),
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
