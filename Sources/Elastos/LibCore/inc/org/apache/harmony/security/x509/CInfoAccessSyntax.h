
#ifndef __CINFOACCESSSYNTAX_H__
#define __CINFOACCESSSYNTAX_H__

#include "_CInfoAccessSyntax.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CInfoAccessSyntax)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI DumpValueEx(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI ToString(
        /* [out] */ String * pStr);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CINFOACCESSSYNTAX_H__
