#ifndef __EVENTHOLE_H_
#define __EVENTHOLE_H_

#include "view/View.h"

using Elastos::Droid::View::IInternalInsetsInfo;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

class EventHole : public Elastos::Droid::View::View
{
public:
    EventHole();

    EventHole(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    EventHole(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnComputeInternalInsets(
        /* [in] */ IInternalInsetsInfo* inoutInfo);

    //@Override
    virtual void OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    virtual CARAPI OnAttachedToWindow();

    virtual CARAPI OnDetachedFromWindow();

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

private:
    static const String TAG;// = "StatusBar.EventHole";

    Boolean mWindowVis;
    Int32 mLoc[2];
};

}// namespace Policy
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__EVENTHOLE_H_
