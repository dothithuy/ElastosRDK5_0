
#ifndef __CWEIGHTEDLINEARLAYOUT_H_
#define __CWEIGHTEDLINEARLAYOUT_H_

#include "_CWeightedLinearLayout.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/LinearLayoutMacro.h"
#include "widget/internal/WeightedLinearLayout.h"


namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CWeightedLinearLayout), public WeightedLinearLayout
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ILINEARLAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
};

} // namespace Internal
} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CWEIGHTEDLINEARLAYOUT_H_
