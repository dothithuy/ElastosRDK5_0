
#ifndef __CINVALIDITYDATEHELPER_H__
#define __CINVALIDITYDATEHELPER_H__

#include "_CInvalidityDateHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CInvalidityDateHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Type ** ppAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CINVALIDITYDATEHELPER_H__
