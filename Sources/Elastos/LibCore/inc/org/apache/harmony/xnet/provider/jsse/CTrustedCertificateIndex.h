
#ifndef __CTRUSTEDCERTIFICATEINDEX_H__
#define __CTRUSTEDCERTIFICATEINDEX_H__

#include "_CTrustedCertificateIndex.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CTrustedCertificateIndex)
{
public:
    CARAPI CheckServerTrustedEx(
        /* [in] */ ArrayOf<Elastos::Security::Cert::IX509Certificate *> * pChain,
        /* [in] */ const String& authType,
        /* [in] */ const String& host,
        /* [out] */ Elastos::Utility::IList ** ppCertificates);

    CARAPI HandleTrustStorageUpdate();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Elastos::Utility::ISet * pAnchors);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CTRUSTEDCERTIFICATEINDEX_H__
