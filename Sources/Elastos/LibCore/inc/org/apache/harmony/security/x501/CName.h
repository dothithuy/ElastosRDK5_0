
#ifndef __CNAME_H__
#define __CNAME_H__

#include "_CName.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X501 {

CarClass(CName)
{
public:
    CARAPI GetX500Principal(
        /* [out] */ Elastosx::Security::Auth::X500::IX500Principal ** ppX500Principal);

    CARAPI GetName(
        /* [in] */ const String& format,
        /* [out] */ String * pName);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> * pEncoding);

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ Elastos::Utility::IList * pRdn);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CNAME_H__
