
#ifndef __CCHRONOMETER_H__
#define __CCHRONOMETER_H__

#include "_CChronometer.h"
#include "widget/Chronometer.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CChronometer), public Chronometer
{
public:
    IVIEW_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    ITEXTVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetBase(
        /* [in] */ Int64 base);

    CARAPI GetBase(
        /* [out] */ Int64 * pBase);

    CARAPI SetFormat(
        /* [in] */ const String& format);

    CARAPI GetFormat(
        /* [out] */ String * pFormat);

    CARAPI SetOnChronometerTickListener(
        /* [in] */ IOnChronometerTickListener * pListener);

    CARAPI GetOnChronometerTickListener(
        /* [out] */ IOnChronometerTickListener ** ppListener);

    CARAPI Start();

    CARAPI Stop();

    CARAPI SetStarted(
        /* [in] */ Boolean started);

    CARAPI OnPreDraw(
        /* [out] */ Boolean * pResult);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

}// namespace Elastos
}// namespace Droid
}// namespace Widget
#endif // __CCHRONOMETER_H__
