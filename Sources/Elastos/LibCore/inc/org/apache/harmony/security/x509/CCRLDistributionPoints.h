
#ifndef __CCRLDISTRIBUTIONPOINTS_H__
#define __CCRLDISTRIBUTIONPOINTS_H__

#include "_CCRLDistributionPoints.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CCRLDistributionPoints)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI DumpValueEx(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CCRLDISTRIBUTIONPOINTS_H__
