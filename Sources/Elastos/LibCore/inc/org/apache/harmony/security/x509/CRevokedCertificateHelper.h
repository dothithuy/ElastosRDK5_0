
#ifndef __CREVOKEDCERTIFICATEHELPER_H__
#define __CREVOKEDCERTIFICATEHELPER_H__

#include "_CRevokedCertificateHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CRevokedCertificateHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence ** ppAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CREVOKEDCERTIFICATEHELPER_H__
