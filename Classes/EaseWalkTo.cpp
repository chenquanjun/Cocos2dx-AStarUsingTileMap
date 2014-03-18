//
//  EaseWalkTo.cpp
//  testAStar
//
//  Created by chenquanjun on 14-3-14.
//
//

#include "EaseWalkTo.h"
#include "TestSprite.h"

EaseWalkTo::EaseWalkTo()
:_pointArr(nullptr),
_nIndex(0)
{
    
}
EaseWalkTo::~EaseWalkTo(){
    CC_SAFE_RELEASE(_pointArr);
}

EaseWalkTo* EaseWalkTo::create(float duration, PointArray* pointArr)
{
    EaseWalkTo *walkTo = new EaseWalkTo();
    walkTo->initWithDuration(duration, pointArr);
    walkTo->autorelease();
    
    return walkTo;
}

bool EaseWalkTo::initWithDuration(float duration, PointArray* pointArr)
{
    if (ActionInterval::initWithDuration(duration))
    {
        CCASSERT(pointArr != nullptr, "should not be null");
        setPointArr(pointArr);
        return true;
    }
    
    return false;
}

EaseWalkTo* EaseWalkTo::clone() const
{
	// no copy constructor
	auto a = new EaseWalkTo();
	a->initWithDuration(_duration, _pointArr->clone());
	a->autorelease();
	return a;
}

EaseWalkTo* EaseWalkTo::reverse() const
{
    auto a = new EaseWalkTo();
	a->initWithDuration(_duration, _pointArr->reverse());
	a->autorelease();
	return nullptr;
}

void EaseWalkTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    if (_target)
    {
        if (_pointArr->count() > 0) {
            Point point = _pointArr->getControlPointAtIndex(0);
            _target->setPosition(point);
        }
        
    }
    
}

void EaseWalkTo::update(float time)
{
    if (_target)
    {
        //从0 到 1
        int count = _pointArr->count();

        float fIndex = count * time;
        int index = (int)fIndex;

        if (index < count) {
            Point pointBegin = _pointArr->getControlPointAtIndex(index);
            Point pointEnd = _pointArr->getControlPointAtIndex(index + 1);
            float offset = fIndex - index;

            float x = pointBegin.x + (pointEnd.x - pointBegin.x) * offset;
            float y = pointBegin.y + (pointEnd.y - pointBegin.y) * offset;
            _target->setPosition(Point(x, y));
            _nIndex = index;
            
            this->playActionByPos(pointBegin, pointEnd, _target);
        }else{
            //最后一点
            Point point = _pointArr->getControlPointAtIndex(index - 1);
            _target->setPosition(point);

            TestSprite *isTestSprite = dynamic_cast<TestSprite*>(_target);
            
            if (isTestSprite != nullptr) {
                isTestSprite->playAction(PlayerActionType::Invalid);
            }
        }
    }    
}

void EaseWalkTo::playActionByPos(Point start, Point end, Node *target){
    float offsetX = end.x - start.x;
    float offsetY = end.y - start.y;
    PlayerActionType actionType = PlayerActionType::Invalid;
    
    bool isAction = true;
    
    if (offsetY > 5) {
        actionType = PlayerActionType::Up;
    }else if (offsetY < -5){
        actionType = PlayerActionType::Down;
    }else if (offsetX > 5){
        actionType = PlayerActionType::Right;
    }else if (offsetX < -5){
        actionType = PlayerActionType::Left;
    }else{
        actionType = PlayerActionType::Invalid;
        isAction =false;
    }
    
    TestSprite *isTestSprite = dynamic_cast<TestSprite*>(_target);
    
    if (isTestSprite != nullptr) {
        isTestSprite->playAction(actionType);
    }

}