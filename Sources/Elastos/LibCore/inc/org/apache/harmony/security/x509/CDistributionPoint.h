
#ifndef __CDISTRIBUTIONPOINT_H__
#define __CDISTRIBUTIONPOINT_H__

#include "_CDistributionPoint.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CDistributionPoint)
{
public:
    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X509::IDistributionPointName * pDistributionPoint,
        /* [in] */ Org::Apache::Harmony::Security::X509::IReasonFlags * pReasons,
        /* [in] */ Org::Apache::Harmony::Security::X509::IGeneralNames * pCRLIssuer);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CDISTRIBUTIONPOINT_H__
