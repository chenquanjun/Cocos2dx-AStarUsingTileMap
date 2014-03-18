//
//  WalkTo.h
//  testAStar
//
//  Created by chenquanjun on 14-3-13.
//
//

#ifndef __testAStar__WalkTo__
#define __testAStar__WalkTo__

#include "cocos2d.h"
#include "CCActionInterval.h"
#include "MapGeneral.h"
USING_NS_CC;

class WalkTo : public ActionInterval
{
public:
 
    static WalkTo* create(float duration, PointArray* pointArr);
    
    //
    // Overrides
    //
    virtual WalkTo* clone() const override;
	virtual WalkTo* reverse(void) const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
protected:
    WalkTo();
    virtual ~WalkTo();
    /** initializes the action with duration and color */
    bool initWithDuration(float duration, PointArray* pointArr);
    
    CC_SYNTHESIZE_RETAIN(PointArray*, _pointArr, PointArr);
 
    int _nIndex;
private:
    CC_DISALLOW_COPY_AND_ASSIGN(WalkTo);
};

#endif /* defined(__testAStar__WalkTo__) */
