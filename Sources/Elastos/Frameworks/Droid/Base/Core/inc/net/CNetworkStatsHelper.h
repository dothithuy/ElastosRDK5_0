
#ifndef __CNETWORKSTATSHELPER_H__
#define __CNETWORKSTATSHELPER_H__

#include "_CNetworkStatsHelper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkStatsHelper)
{
public:

    CARAPI SetToString(
        /* [in] */ Int32 set,
        /* [out] */ String* result);
    
    CARAPI TagToString(
        /* [in] */ Int32 tag,
        /* [out] */ String* result);

    CARAPI Subtract(
        /* [in] */ INetworkStats* left,
        /* [in] */ INetworkStats* right,
        /* [in] */ INonMonotonicObserver* observer,
        /* [in] */ IInterface* cookie,
        /* [out] */ INetworkStats** result);
};

}
}
}

#endif // __CNETWORKSTATSHISTORYHELPER_H__
