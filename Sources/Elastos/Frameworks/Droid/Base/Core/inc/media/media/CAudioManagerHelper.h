
#ifndef __CAUDIOMANAGERHELPER_H__
#define __CAUDIOMANAGERHELPER_H__

#include "_CAudioManagerHelper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CAudioManagerHelper)
{
public:
    /**
     * Checks valid ringer mode values.
     *
     * @return true if the ringer mode indicated is valid, false otherwise.
     *
     * @see #setRingerMode(int)
     * @hide
     */
    CARAPI IsValidRingerMode(
        /* [in] */ Int32 rightMode,
        /* [out] */ Boolean* result);

    /**
    * @hide
    */
    CARAPI SetHdmiAvailable(
        /* [in] */ Boolean val);

    CARAPI GetHdmiAvailable(
        /* [out] */ Boolean* result);

    /**
    * @hide
    */
    CARAPI GetHdmiExpected(
        /* [out] */ Boolean* result);

    /**
    * @hide
    */
    CARAPI SetHdmiExpected(
        /* [in] */ Boolean val);

    CARAPI GetDefaultStreamVolume(
        /* [out, callee] */ ArrayOf<Int32>** defaultStreamVolume);
};

} // namespace Media
} // namespace Droid
} // namespace Elastos

#endif // __CAUDIOMANAGERHELPER_H__
