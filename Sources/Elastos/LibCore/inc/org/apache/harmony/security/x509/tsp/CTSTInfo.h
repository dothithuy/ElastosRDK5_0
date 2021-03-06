
#ifndef __CTSTINFO_H__
#define __CTSTINFO_H__

#include "_CTSTInfo.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {
namespace Tsp {

CarClass(CTSTInfo)
{
public:
    CARAPI ToString(
        /* [out] */ String * pStr);

    CARAPI GetAccuracy(
        /* [out, callee] */ ArrayOf<Int32> ** ppAccuracy);

    CARAPI GetExtensions(
        /* [out] */ Org::Apache::Harmony::Security::X509::IExtensions ** ppExtensions);

    CARAPI GetGenTime(
        /* [out] */ Elastos::Utility::IDate ** ppDate);

    CARAPI GetMessageImprint(
        /* [out] */ Org::Apache::Harmony::Security::X509::Tsp::IMessageImprint ** ppMessageImprint);

    CARAPI GetNonce(
        /* [out] */ Elastos::Math::IBigInteger ** ppNonce);

    CARAPI GetOrdering(
        /* [out] */ Boolean * pOrdering);

    CARAPI GetPolicy(
        /* [out] */ String * pPolicy);

    CARAPI GetSerialNumber(
        /* [out] */ Elastos::Math::IBigInteger ** ppSerialNumber);

    CARAPI GetTsa(
        /* [out] */ Org::Apache::Harmony::Security::X509::IGeneralName ** ppTsa);

    CARAPI GetVersion(
        /* [out] */ Int32 * pVer);

    CARAPI constructor(
        /* [in] */ Int32 ver,
        /* [in] */ const String& policy,
        /* [in] */ Org::Apache::Harmony::Security::X509::Tsp::IMessageImprint * pMessageImprint,
        /* [in] */ Elastos::Math::IBigInteger * pSerialNumber,
        /* [in] */ Elastos::Utility::IDate * pGenTime,
        /* [in] */ ArrayOf<Int32> * pAccuracy,
        /* [in] */ Boolean ordering,
        /* [in] */ Elastos::Math::IBigInteger * pNonce,
        /* [in] */ Org::Apache::Harmony::Security::X509::IGeneralName * pTsa,
        /* [in] */ Org::Apache::Harmony::Security::X509::IExtensions * pExtensions);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CTSTINFO_H__
