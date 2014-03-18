//
//  WalkTo.cpp
//  testAStar
//
//  Created by chenquanjun on 14-3-13.
//
//

#include "WalkTo.h"

WalkTo::WalkTo()
:_pointArr(nullptr),
 _nIndex(0)
{
    
}
WalkTo::~WalkTo(){
    CC_SAFE_RELEASE(_pointArr);
}

WalkTo* WalkTo::create(float duration, PointArray* pointArr)
{
    WalkTo *walkTo = new WalkTo();
    walkTo->initWithDuration(duration, pointArr);
    walkTo->autorelease();
    
    return walkTo;
}

bool WalkTo::initWithDuration(float duration, PointArray* pointArr)
{
    if (ActionInterval::initWithDuration(duration))
    {
        CCASSERT(pointArr != nullptr, "should not be null");
        setPointArr(pointArr);
        return true;
    }
    
    return false;
}

WalkTo* WalkTo::clone() const
{
	// no copy constructor
	auto a = new WalkTo();
	a->initWithDuration(_duration, _pointArr->clone());
	a->autorelease();
	return a;
}

WalkTo* WalkTo::reverse() const
{
    auto a = new WalkTo();
	a->initWithDuration(_duration, _pointArr->reverse());
	a->autorelease();
	return nullptr;
}

void WalkTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    if (_target)
    {
        if (_pointArr->count() > 0) {
            Point point = _pointArr->getControlPointAtIndex(_nIndex);
            _target->setPosition(point);
        }
 
    }

}

void WalkTo::update(float time)
{
    if (_target)
    {
        int index = _pointArr->count() * time - 1;
        
        if (index <= _nIndex) {
            //小于是防止负数
            return;
        }
        Point point = _pointArr->getControlPointAtIndex(index);
        _target->setPosition(point);
        _nIndex = index;

        
    }    
}