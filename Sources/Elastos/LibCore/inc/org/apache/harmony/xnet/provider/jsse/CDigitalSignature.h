
#ifndef __CDIGITALSIGNATURE_H__
#define __CDIGITALSIGNATURE_H__

#include "_CDigitalSignature.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CDigitalSignature)
{
public:
    CARAPI Init(
        /* [in] */ Elastos::Security::IPrivateKey * pKey);

    CARAPI InitEx(
        /* [in] */ Elastos::Security::Cert::ICertificate * pCert);

    CARAPI Update(
        /* [in] */ ArrayOf<Byte> * pData);

    CARAPI SetMD5(
        /* [in] */ ArrayOf<Byte> * pData);

    CARAPI SetSHA(
        /* [in] */ ArrayOf<Byte> * pData);

    CARAPI Sign(
        /* [out, callee] */ ArrayOf<Byte> ** ppSign);

    CARAPI VerifySignature(
        /* [in] */ ArrayOf<Byte> * pData,
        /* [out] */ Boolean * pResult);

    CARAPI constructor(
        /* [in] */ const String& authType);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CDIGITALSIGNATURE_H__
