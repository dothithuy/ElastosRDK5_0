
#ifndef __CX509ENCODEDKEYSPEC_H__
#define __CX509ENCODEDKEYSPEC_H__

#include "_CX509EncodedKeySpec.h"
#include "EncodedKeySpec.h"

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CX509EncodedKeySpec), public EncodedKeySpec
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> **encoded);

    CARAPI GetFormat(
        /* [out] */ String *format);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> *encodedKey);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __CX509ENCODEDKEYSPEC_H__
