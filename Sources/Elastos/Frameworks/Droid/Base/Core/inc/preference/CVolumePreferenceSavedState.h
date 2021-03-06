
#ifndef  __CVOLUMEPREFERENCESAVEDSTATE_H__
#define  __CVOLUMEPREFERENCESAVEDSTATE_H__

#include "_CVolumePreferenceSavedState.h"
#include "PreferenceBaseSavedState.h"
#include "VolumePreference.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CVolumePreferenceSavedState), public PreferenceBaseSavedState
{
public:
    CVolumePreferenceSavedState();

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
    CARAPI_(AutoPtr<VolumePreference::VolumeStore>) GetVolumeStore();

private:
    AutoPtr<VolumePreference::VolumeStore> mVolumeStore;

    friend class VolumePreference;
};

} // Preference
} // Droid
} // Elastos

#endif  // __CVOLUMEPREFERENCESAVEDSTATE_H__
