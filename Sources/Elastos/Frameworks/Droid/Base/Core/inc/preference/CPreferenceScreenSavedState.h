
#ifndef  __CPREFERENCESCREENSAVEDSTATE_H__
#define  __CPREFERENCESCREENSAVEDSTATE_H__

#include "_CPreferenceScreenSavedState.h"
#include "PreferenceBaseSavedState.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Preference {

class PreferenceScreen;

CarClass(CPreferenceScreenSavedState), public PreferenceBaseSavedState
{
public:
    CPreferenceScreenSavedState();

    CARAPI IsDialogShowing(
        /* [out] */ Boolean* isDialogShowing);

    CARAPI SetDialogShowing(
        /* [in] */ Boolean isDialogShowing);

    CARAPI SetDialogBundle(
        /* [in] */ IBundle* b);

    CARAPI GetDialogBundle(
        /* [out] */ IBundle** b);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelable* source);

private:
    Boolean mIsDialogShowing;
    AutoPtr<IBundle> mDialogBundle;
};

} // Preference
} // Droid
} // Elastos

#endif  // __CPREFERENCESCREENSAVEDSTATE_H__
