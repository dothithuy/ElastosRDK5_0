
#ifndef __AUTOLOCK_H__
#define __AUTOLOCK_H__

#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#else
#include "Elastos.Core.h"
#endif

namespace Elastos {
namespace Core {

class Autolock
{
public:
    Autolock(
        /* [in] */ IInterface* obj)
    {
        assert(obj != NULL);
        mSyncObj = ISynchronize::Probe(obj);
        assert(mSyncObj != NULL);
        assert(SUCCEEDED(mSyncObj->Lock()));
    }

    ~Autolock()
    {
        assert(mSyncObj != NULL);
        assert(SUCCEEDED(mSyncObj->Unlock()));
    }

private:
    AutoPtr<ISynchronize> mSyncObj;
};

} // namespace Core
} // namespace Elastos

#endif //__AUTOLOCK_H__