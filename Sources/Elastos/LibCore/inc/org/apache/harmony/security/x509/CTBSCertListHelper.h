
#ifndef __CTBSCERTLISTHELPER_H__
#define __CTBSCERTLISTHELPER_H__

#include "_CTBSCertListHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CTBSCertListHelper)
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

#endif // __CTBSCERTLISTHELPER_H__
