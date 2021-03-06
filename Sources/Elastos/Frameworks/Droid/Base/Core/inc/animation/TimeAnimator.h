
#ifndef  __TIMEANIMATOR_H__
#define  __TIMEANIMATOR_H__

#include "animation/ValueAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

class TimeAnimator : public ValueAnimator
{
public:
    TimeAnimator();

    virtual CARAPI Start();

    virtual CARAPI_(Boolean) AnimationFrame(
        /* [in] */ Int64 currentTime);

    /**
     * Sets a listener that is sent update events throughout the life of
     * an animation.
     *
     * @param listener the listener to be set.
     */
    virtual CARAPI SetTimeListener(
        /* [in] */ ITimeListener* listener);

    //@Override
    virtual CARAPI AnimateValue(
        /* [in] */ Float fraction);

    //@Override
    virtual CARAPI InitAnimation();

    virtual CARAPI Clone(
        /* [out] */ IAnimator** object);

private:
    AutoPtr<ITimeListener> mListener;
    Int64 mPreviousTime;
};


}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
#endif  //__TIMEANIMATOR_H__
