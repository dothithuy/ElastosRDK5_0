
#ifndef __CEXTENSIONHELPER_H__
#define __CEXTENSIONHELPER_H__

#include "_CExtensionHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CExtensionHelper)
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

#endif // __CEXTENSIONHELPER_H__
