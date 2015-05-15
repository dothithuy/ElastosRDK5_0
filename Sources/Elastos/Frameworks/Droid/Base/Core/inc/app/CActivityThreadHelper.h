
#ifndef __CACTIVITYTHREADHELPER_H__
#define __CACTIVITYTHREADHELPER_H__

#include "_CActivityThreadHelper.h"

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IIPackageManager;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CActivityThreadHelper)
{
public:
    CARAPI GetCurrentActivityThread(
        /* [out] */ IActivityThread** thread);

    CARAPI GetCurrentPackageName(
        /* [out] */ String* name);

    CARAPI GetCurrentApplication(
        /* [out] */ IApplication** app);

    CARAPI GetPackageManager(
        /* [out] */ IIPackageManager** pkgManager);

    /**
     * Return the Intent that's currently being handled by a
     * BroadcastReceiver on this thread, or null if none.
     * @hide
     */
    CARAPI GetIntentBeingBroadcast(
        /* [out] */ IIntent** intent);

    CARAPI GetSystemMain(
        /* [out] */ IActivityThread** thread);

    CARAPI Main(
        /* [in] */ const ArrayOf<String>& args);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CACTIVITYTHREADHELPER_H__