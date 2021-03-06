
#ifndef __COPENSSLDSAKEYPAIRGENERATOR_H__
#define __COPENSSLDSAKEYPAIRGENERATOR_H__

#include "_COpenSSLDSAKeyPairGenerator.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(COpenSSLDSAKeyPairGenerator)
{
public:
    CARAPI GenerateKeyPair(
        /* [out] */ Elastos::Security::IKeyPair ** ppKeyPair);

    CARAPI Initialize(
        /* [in] */ Int32 keysize,
        /* [in] */ Elastos::Security::ISecureRandom * pRandom);

    CARAPI InitializeEx(
        /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec * pParams,
        /* [in] */ Elastos::Security::ISecureRandom * pRandom);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __COPENSSLDSAKEYPAIRGENERATOR_H__
