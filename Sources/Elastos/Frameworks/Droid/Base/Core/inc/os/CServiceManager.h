
#ifndef __CSERVICEMANAGER_H__
#define __CSERVICEMANAGER_H__

#include "ext/frameworkext.h"
#include "_CServiceManager.h"
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>

using Elastos::Utility::HashMap;
using Elastos::Utility::IObjectStringMap;
using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CServiceManager)
{
public:
    CServiceManager();

    ~CServiceManager();

    CARAPI AddService(
        /* [in] */ const String& name,
        /* [in] */ IInterface* service);

    CARAPI AddServiceEx(
        /* [in] */ const String& name,
        /* [in] */ IInterface* service,
        /* [in] */ Boolean allowIsolated);

    CARAPI GetService(
        /* [in] */ const String& name,
        /* [out] */ IInterface** service);

    /**
     * This is only intended to be called when the process is first being brought
     * up and bound by the activity manager. There is only one thread in the process
     * at that time, so no locking is done.
     *
     * @param cache the cache of service references
     * @hide
     */
    CARAPI InitServiceCache(
        /* [in] */ IObjectStringMap* services);

private:
    HashMap<String, AutoPtr<IInterface> > mServiceCache;
    Mutex mServiceCacheLock;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __CSERVICEMANAGER_H__
