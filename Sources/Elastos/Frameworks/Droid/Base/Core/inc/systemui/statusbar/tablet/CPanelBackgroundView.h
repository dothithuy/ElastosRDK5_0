
#ifndef __CPANELBACKGROUNDVIEW_H__
#define __CPANELBACKGROUNDVIEW_H__

#include "_CPanelBackgroundView.h"
#include "view/ViewMacro.h"
#include "systemui/statusbar/tablet/PanelBackgroundView.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Tablet {

CarClass(CPanelBackgroundView), public PanelBackgroundView
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);
};

}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif // __CPANELBACKGROUNDVIEW_H__
