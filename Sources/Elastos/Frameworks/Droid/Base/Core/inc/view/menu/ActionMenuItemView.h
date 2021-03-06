#ifndef  __ACTIONMENUITEMVIEW_H__
#define  __ACTIONMENUITEMVIEW_H__

#include "widget/TextView.h"

using Elastos::Droid::View::IViewOnLongClickListener;
using Elastos::Droid::View::Menu::IActionMenuChildView;
using Elastos::Droid::Widget::TextView;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

class ActionMenuItemView : public TextView
{
public:
    ActionMenuItemView();

    ActionMenuItemView(
        /* [in] */ IContext* context);

    ActionMenuItemView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    ActionMenuItemView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    //@Override
    CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    //@Override
    CARAPI SetPadding(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    CARAPI GetItemData(
        /* [out] */ IMenuItemImpl** itemData);

    CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    CARAPI OnClick(
        /* [in] */ IView* v);

    CARAPI SetItemInvoker(
        /* [in] */ IMenuBuilderItemInvoker* invoker);

    CARAPI PrefersCondensedTitle(
        /* [out] */ Boolean* supported);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetExpandedFormat(
        /* [in] */ Boolean expandedFormat);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI_(Boolean) HasText();

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    //@Override
    CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    //@Override
    CARAPI OnPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    //@Override
    CARAPI_(Boolean) DispatchHoverEvent(
        /* [in] */ IMotionEvent* event);

    CARAPI ShowsIcon(
        /* [out] */ Boolean* shows);

    CARAPI NeedsDividerBefore(
        /* [out] */ Boolean* need);

    CARAPI NeedsDividerAfter(
        /* [out] */ Boolean* need);

    //@Override
    CARAPI OnLongClick(
        /* [in] */ IView* v,
        /* [out] */ Boolean* result);

protected:
    CARAPI Init(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    //@Override
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

private:
    CARAPI_(void) UpdateTextButtonVisibility();

private:
    static String TAG;

    AutoPtr<IMenuItemImpl> mItemData;
    AutoPtr<ICharSequence> mTitle;
    AutoPtr<IDrawable> mIcon;
    AutoPtr<IMenuBuilderItemInvoker> mItemInvoker;

    Boolean mAllowTextWithIcon;
    Boolean mExpandedFormat;
    Int32 mMinWidth;
    Int32 mSavedPaddingLeft;

    static const Int32 MAX_ICON_SIZE; // dp
    Int32 mMaxIconSize;
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ACTIONMENUITEMVIEW_H__
