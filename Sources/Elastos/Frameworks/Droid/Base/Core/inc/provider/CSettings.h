#ifndef __CSETTINGS_H__
#define __CSETTINGS_H__

#include "_CSettings.h"

namespace Elastos {
namespace Droid {
namespace Provider {

/**
 * The Settings provider contains global system-level device preferences.
 */
CarClass(CSettings)
{
public:
    /**
     * Returns the device ID that we should use when connecting to the mobile gtalk server.
     * This is a string like "android-0x1242", where the hex string is the Android ID obtained
     * from the GoogleLoginService.
     *
     * @param androidId The Android ID for this device.
     * @return The device ID that should be used when connecting to the mobile gtalk server.
     * @hide
     */
    CARAPI GetGTalkDeviceId(
        /* [in] */ Int64 androidId,
        /* [out] */ String* deviceId);
};

} //namespace Provider
} //namespace Droid
} //namespace Elastos

#endif //__CSETTINGS_H__
