#ifndef __CANALOGCLOCK_H__
#define __CANALOGCLOCK_H__

#include "_CAnalogClock.h"
#include "AnalogClock.h"
#include "view/ViewMacro.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CAnalogClock) , public AnalogClock
{
public:
    IVIEW_METHODS_DECL();

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

};
}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
