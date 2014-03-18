//
//  TestSprite.cpp
//  testAStar
//
//  Created by chenquanjun on 14-3-14.
//
//

#include "TestSprite.h"


TestSprite::TestSprite(){
    _lastActionType = PlayerActionType::Invalid;
}

TestSprite::~TestSprite(){
    
}

TestSprite* TestSprite::create(const std::string& fileNameFormat){
    TestSprite *ret = new TestSprite();
    if(ret && ret->init(fileNameFormat))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TestSprite::init(const std::string& fileNameFormat){
    if (Sprite::init())
    {

        //将纹理提前加入精灵帧缓存
        SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
        
        for (int j = 0; j < 4; j++) {
            Vector<SpriteFrame*> animFrames;
            
            for(int i = 0; i < 4; i++)
            {
                __String * str = __String::createWithFormat(fileNameFormat.c_str(), j, i);
                SpriteFrame *frame = frameCache->getSpriteFrameByName(str->getCString());
                
                animFrames.pushBack(frame);
            }
            
            Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
            
            int actionTag = j + 1;
            
            _playerAnimMap.insert(actionTag, animation);//使用map保存

        }
        
        //精灵
        __String * fileStr = __String::createWithFormat(fileNameFormat.c_str(), 0, 0);
        Sprite *animSprite = Sprite::createWithSpriteFrameName(fileStr->getCString());
        
        animSprite->setAnchorPoint(Point(0.5, 0));
        
        this->addChild(animSprite);
        
        _animSprite = animSprite;

        return true;
    }
    else
    {
        return false;
    }
}

void TestSprite::playAction(PlayerActionType actionType){
    if (actionType == _lastActionType) {
        return;//相同动作
    }
    
    //停止旧动作
    if (_lastActionType != PlayerActionType::Invalid) {
        _animSprite->stopActionByTag((int)_lastActionType);
    }
    
    _lastActionType = actionType;
    Animation *animation = (Animation*)_playerAnimMap.at((int)actionType);

    //map中存在key值
    
    if (animation != nullptr) {
        Animate *anim = Animate::create(animation);
        RepeatForever *repeat = RepeatForever::create(anim);
        
        repeat->setTag((int)actionType);
        _animSprite->runAction(repeat);
    }
}

