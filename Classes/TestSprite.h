//
//  TestSprite.h
//  testAStar
//
//  Created by chenquanjun on 14-3-14.
//
//

#ifndef __testAStar__TestSprite__
#define __testAStar__TestSprite__

#include "cocos2d.h"
#include "MapGeneral.h"
USING_NS_CC;

class TestSprite : public Sprite{
private:
    TestSprite();
    ~TestSprite();
    
public:
    static TestSprite* create(const std::string& fileNameFormat);
    
    void playAction(PlayerActionType actionType);
    
private:
    bool init(const std::string& fileNameFormat);
    
    
private:
    Map<int, Animation*> _playerAnimMap;
    
    Sprite* _animSprite;
    
    PlayerActionType _lastActionType;
};

#endif /* defined(__testAStar__TestSprite__) */
