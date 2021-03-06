
#ifndef __CDSAPUBLICKEYSPEC_H__
#define __CDSAPUBLICKEYSPEC_H__

#include "_CDSAPublicKeySpec.h"

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CDSAPublicKeySpec)
{
public:
    CARAPI GetG(
        /* [out] */ IBigInteger **g);

    CARAPI GetP(
        /* [out] */ IBigInteger **p);

    CARAPI GetQ(
        /* [out] */ IBigInteger **q);

    CARAPI GetY(
        /* [out] */ IBigInteger **y);

    CARAPI constructor(
        /* [in] */ IBigInteger *y,
        /* [in] */ IBigInteger *p,
        /* [in] */ IBigInteger *q,
        /* [in] */ IBigInteger *g);

private:
    // Private key
    AutoPtr<IBigInteger> mY;
    // Prime
    AutoPtr<IBigInteger> mP;
    // Sub-prime
    AutoPtr<IBigInteger> mQ;
    // Base
    AutoPtr<IBigInteger> mG;
};

}
}
}

#endif // __CDSAPUBLICKEYSPEC_H__
