//
//  EaseWalkTo.h
//  testAStar
//
//  Created by chenquanjun on 14-3-14.
//
//

#ifndef __testAStar__EaseWalkTo__
#define __testAStar__EaseWalkTo__

#include "cocos2d.h"
#include "CCActionInterval.h"
#include "MapGeneral.h"
USING_NS_CC;

class EaseWalkTo : public ActionInterval
{
public:
    
    static EaseWalkTo* create(float duration, PointArray* pointArr);
    
    //
    // Overrides
    //
    virtual EaseWalkTo* clone() const override;
	virtual EaseWalkTo* reverse(void) const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
protected:
    EaseWalkTo();
    virtual ~EaseWalkTo();
    /** initializes the action with duration and color */
    bool initWithDuration(float duration, PointArray* pointArr);
    
    void playActionByPos(Point start, Point end, Node *target);
    
    CC_SYNTHESIZE_RETAIN(PointArray*, _pointArr, PointArr);
    
    int _nIndex;
    
//    PlayerActionType _actionType;
private:
    CC_DISALLOW_COPY_AND_ASSIGN(EaseWalkTo);
};

#endif /* defined(__testAStar__EaseWalkTo__) */
