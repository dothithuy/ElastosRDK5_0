
#ifndef __CICONMENUVIEW_H__
#define __CICONMENUVIEW_H__

#include "_CIconMenuView.h"
#include "view/menu/IconMenuView.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CIconMenuView), public IconMenuView
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI GetLayout(
        /* [out, callee] */ ArrayOf<Int32>** layout);

    CARAPI GetLayoutNumRows(
        /* [out] */ Int32* num);

    CARAPI Initialize(
        /* [in] */ IMenuBuilder* menu);

    CARAPI GetWindowAnimations(
        /* [out] */ Int32* animations);

    CARAPI InvokeItem(
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* state);

    CARAPI Run();
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  // __CICONMENUVIEW_H__
