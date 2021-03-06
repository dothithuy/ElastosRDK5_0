
#ifndef  __CPHONEWINDOWSAVEDSTATE_H__
#define  __CPHONEWINDOWSAVEDSTATE_H__

#include "_CPhoneWindowSavedState.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

CarClass(CPhoneWindowSavedState)
{
public:
    CPhoneWindowSavedState();

    CARAPI constructor();

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

public:
    Int32 mFeatureId;
    Boolean mIsOpen;
    Boolean mIsInExpandedMode;
    AutoPtr<IBundle> mMenuState;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namepsace Droid
} // namespace Elastos

#endif  // __CPHONEWINDOWSAVEDSTATE_H__
