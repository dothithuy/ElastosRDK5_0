#ifndef __CPOPUPWINDOW_H_
#define __CPOPUPWINDOW_H_

#include "_CPopupWindow.h"
#include "ext/frameworkext.h"
#include "widget/PopupWindowMacro.h"

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::View::IViewOnTouchListener;
using Elastos::Droid::View::IView;

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CPopupWindow), public PopupWindow
{
public:

    IPOPUPWINDOW_METHODS_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle,
        /* [in] */ Int32 defStyleRes);

    CARAPI constructor(
        /* [in] */ IView* contentView);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IView* contentView,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IView* contentView,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean focusable);

//    CARAPI_(PInterface) Probe(
//        /* [in] */ REIID riid);

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CPOPUPWINDOW_H_
