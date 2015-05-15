
#ifndef  __CMETAKEYKEYLISTENERHELPER_H__
#define  __CMETAKEYKEYLISTENERHELPER_H__

#include "_CMetaKeyKeyListenerHelper.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CMetaKeyKeyListenerHelper)
{
public:
    CARAPI ResetMetaState(
        /* [in] */ ISpannable* text);

    CARAPI GetMetaState(
        /* [in] */ ICharSequence* text,
        /* [out] */ Int32* state);

    CARAPI GetMetaStateEx(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 meta,
        /* [out] */ Int32* state);

    CARAPI GetMetaStateEx2(
        /* [in] */ Int64 state,
        /* [out] */ Int32* ret);

    CARAPI GetMetaStateEx3(
        /* [in] */ Int64 state,
        /* [in] */ Int32 meta,
        /* [out] */ Int32* ret);

    CARAPI AdjustMetaAfterKeypress(
        /* [in] */ ISpannable* content);

    CARAPI IsMetaTracker(
        /* [in] */ ICharSequence* text,
        /* [in] */ IInterface* what,
        /* [out] */ Boolean* state);

    CARAPI IsSelectingMetaTracker(
        /* [in] */ ICharSequence* text,
        /* [in] */ IInterface* what,
        /* [out] */ Boolean* state);

    CARAPI StartSelecting(
        /* [in] */ IView* view,
        /* [in] */ ISpannable* content);

    CARAPI StopSelecting(
        /* [in] */ IView* view,
        /* [in] */ ISpannable* content);

    CARAPI ClearMetaKeyState(
        /* [in] */ IEditable* content,
        /* [in] */ Int32 states);

    CARAPI ResetLockedMeta(
        /* [in] */ Int64 state,
        /* [out] */ Int64* meta);

    CARAPI GetMetaState(
        /* [in] */ Int64 state,
        /* [out] */ Int32* result);

    CARAPI GetMetaStateEx(
        /* [in] */ Int64 state,
        /* [in] */ Int32 meta,
        /* [out] */ Int32* result);

    CARAPI AdjustMetaAfterKeypressEx(
        /* [in] */ Int64 state,
        /* [out] */ Int64* result);

    CARAPI HandleKeyDown(
        /* [in] */ Int64 state,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Int64* result);

    CARAPI HandleKeyUp(
        /* [in] */ Int64 state,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Int64* result);

private:
    //private
    static CARAPI_(Int32) GetActive(
        /* [in] */ ICharSequence* text,
        /* [in] */ IInterface* meta,
        /* [in] */ Int32 on,
        /* [in] */ Int32 lock);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif  //__CMETAKEYKEYLISTENERHELPER_H__