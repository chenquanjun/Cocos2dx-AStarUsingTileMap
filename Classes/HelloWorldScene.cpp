#include "HelloWorldScene.h"

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

    //地图layer
    _mapLayer = Layer::create();
    this->addChild(_mapLayer);
    

    auto tmxFileName = "map3.tmx";
    
    //显示地图用
    auto map = TMXTiledMap::create(tmxFileName);
    _mapLayer->addChild(map);
 
    //创建地图信息
    auto pMapInfo = MapInfo::create(tmxFileName);
    _mapInfo = pMapInfo;
    pMapInfo->retain();
    

    //将纹理提前加入精灵帧缓存
    auto *frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("player1.plist");
    frameCache->addSpriteFramesWithFile("player2.plist");
    frameCache->addSpriteFramesWithFile("player3.plist");
    frameCache->addSpriteFramesWithFile("player4.plist");
    
    do {//批量生成
        for (int i = 0; i < 20; i++) {
        
            auto *testSprite1 = this->createTestSpriteWithFormat("player1_%i_%i.png");
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
        
        for (int i = 0; i < 20; i++) {
            
            auto *testSprite1 = this->createTestSpriteWithFormat("player2_%i_%i.png");
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
        
        for (int i = 0; i < 20; i++) {
            
            auto *testSprite1 = this->createTestSpriteWithFormat("player3_%i_%i.png");
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
        
        for (int i = 0; i < 20; i++) {
            
            auto *testSprite1 = this->createTestSpriteWithFormat("player4_%i_%i.png");
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
    } while (0);

    
    do {//touch
        //touch 控制精灵
        auto *testSprite = this->createTestSpriteWithFormat("player1_%i_%i.png");

        testSprite->runAction(RepeatForever::create(Blink::create(0.5, 3)));
        
        _mapLayer->addChild(testSprite, 200);
        
        //触摸
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto myListener = EventListenerTouchOneByOne::create();
        
        //如果不加入此句消息依旧会向下传递
        myListener->setSwallowTouches(true);

        //touch begin
        myListener->onTouchBegan = [=](Touch* touch,Event* event)
        {
            
            _bIsMove = false;
            return true;
        };
        
        //touch moved
        myListener->onTouchMoved = [=](Touch* touch,Event* event)
        {
            auto touchPoint = _mapLayer->convertToNodeSpace(touch->getLocation()); ;
            auto firstTouchPoint = _mapLayer->convertToNodeSpace(touch->getStartLocation());
    
            if (touchPoint.getDistance(firstTouchPoint) > 5){
                _bIsMove = true;

            }else{

            }
            
            if (_bIsMove) {
                auto preTouchPoint = _mapLayer->convertToNodeSpace(touch->getPreviousLocation());
                auto position  = _mapLayer->getPosition();
                _mapLayer->setPosition(Point(position.x + touchPoint.x - preTouchPoint.x, position.y + touchPoint.y - preTouchPoint.y));
            }
        };
        
        //touch ended
        myListener->onTouchEnded = [=](Touch* touch,Event* event)
        {  
            
            auto touchPoint = _mapLayer->convertToNodeSpace(touch->getLocation()); ;
            
            if (!_bIsMove) {
                //根据点击坐标转换成mapId
                auto mapId = this->_mapInfo->convertPointToId(touchPoint);
                
                auto position = testSprite->getPosition();
                auto originId = _mapInfo->convertPointToId(position);
                
                //生成路径
                auto* pMapPath = _mapInfo->getMapPath(originId, mapId);

                if (pMapPath != nullptr)
                {
                    auto *pointArr1 = pMapPath->getPointArr();
                    
                    auto duration = 0.2 * pointArr1->count();
                    
                    auto *easeWalkTo1 = EaseWalkTo::create(duration, pointArr1);
                    easeWalkTo1->setTag(99);
                    testSprite->stopActionByTag(99);
                    testSprite->runAction(easeWalkTo1);
                    
                }
            }else{

            }
            
            //触摸结束后自动调整地图防止越界
            this->adjustMapLayer();

        };
        
        dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
        
    } while (0);
    return true;
}

TestSprite* HelloWorld::createTestSpriteWithFormat(std::string fileName){
    auto startPoint = _mapInfo->getRandomPointMidByType(MapInfoType::Road);
    
    auto *testSprite = TestSprite::create(fileName.c_str());
    
    testSprite->setPosition(startPoint);
    
    testSprite->setAnchorPoint(Point(0.5, 0));
    
    return testSprite;
}

void HelloWorld::adjustMapLayer(){
    
    do {//地图层自动调整
        
        Size mapSize = Size( _mapInfo->getMapGridSize().width * _mapInfo->getPointSize().width, _mapInfo->getMapGridSize().height * _mapInfo->getPointSize().height) ;
        Size winSize = Director::getInstance()->getWinSize();
        
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
}

void HelloWorld::actionDone(Node *pSender){
    int newTarget = _mapInfo->getRandomMapIdByType(MapInfoType::Road);
    
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

