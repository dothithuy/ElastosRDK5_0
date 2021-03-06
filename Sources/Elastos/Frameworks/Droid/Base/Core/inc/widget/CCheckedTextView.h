
#ifndef __CCHECKEDTEXTVIEW_H__
#define __CCHECKEDTEXTVIEW_H__

#include "_CCheckedTextView.h"
#include "widget/CheckedTextView.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CCheckedTextView), public CheckedTextView
{
public:
    IVIEW_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    ITEXTVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Toggle();

    CARAPI IsChecked(
        /* [out] */ Boolean * pChecked);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetCheckMarkDrawable(
        /* [in] */ Int32 resid);

    CARAPI SetCheckMarkDrawableEx(
        /* [in] */ IDrawable * pD);

    CARAPI GetCheckMarkDrawable(
        /* [out] */ IDrawable** drawable);

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
#endif // __CCHECKEDTEXTVIEW_H__
