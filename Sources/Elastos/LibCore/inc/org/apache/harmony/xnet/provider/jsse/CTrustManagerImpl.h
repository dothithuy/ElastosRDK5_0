
#ifndef __CTRUSTMANAGERIMPL_H__
#define __CTRUSTMANAGERIMPL_H__

#include "_CTrustManagerImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CTrustManagerImpl)
{
public:
    CARAPI CheckClientTrusted(
        /* [in] */ ArrayOf<Elastos::Security::Cert::IX509Certificate *> * pChain,
        /* [in] */ const String& authType);

    CARAPI CheckServerTrusted(
        /* [in] */ ArrayOf<Elastos::Security::Cert::IX509Certificate *> * pChain,
        /* [in] */ const String& authType);

    CARAPI GetAcceptedIssuers(
        /* [out, callee] */ ArrayOf<Elastos::Security::Cert::IX509Certificate *> ** ppIssuers);

    CARAPI CheckServerTrustedEx(
        /* [in] */ ArrayOf<Elastos::Security::Cert::IX509Certificate *> * pChain,
        /* [in] */ const String& authType,
        /* [in] */ const String& host,
        /* [out] */ Elastos::Utility::IList ** ppCertificates);

    CARAPI HandleTrustStorageUpdate();

    CARAPI constructor(
        /* [in] */ Elastos::Security::IKeyStore * pKeyStore);

    CARAPI constructor(
        /* [in] */ Elastos::Security::IKeyStore * pKeyStore,
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::ICertPinManager * pManager);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CTRUSTMANAGERIMPL_H__
