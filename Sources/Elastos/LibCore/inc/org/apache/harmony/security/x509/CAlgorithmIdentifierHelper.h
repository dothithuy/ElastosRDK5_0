
#ifndef __CALGORITHMIDENTIFIERHELPER_H__
#define __CALGORITHMIDENTIFIERHELPER_H__

#include "_CAlgorithmIdentifierHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CAlgorithmIdentifierHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence ** ppAsn1);

    CARAPI SetASN1(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence * pAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CALGORITHMIDENTIFIERHELPER_H__
