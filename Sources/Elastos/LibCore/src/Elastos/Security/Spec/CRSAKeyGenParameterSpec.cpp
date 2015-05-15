
#include "CRSAKeyGenParameterSpec.h"
#include "cmdef.h"
#include "CBigInteger.h"

using Elastos::Math::CBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

static const AutoPtr<IBigInteger> CRSAKeyGenParameterSpec::F0 = CRSAKeyGenParameterSpec::InitStatic(3LL);

static const AutoPtr<IBigInteger> CRSAKeyGenParameterSpec::F4 = CRSAKeyGenParameterSpec::InitStatic(65537LL);

const AutoPtr<IBigInteger> CRSAKeyGenParameterSpec::InitStatic(
    /* [in] */ Int64 bi)
{
    AutoPtr<IBigInteger> tmp;
    CBigInteger::ValueOf(bi, (IBigInteger**)&tmp);
    return tmp;
}

ECode CRSAKeyGenParameterSpec::GetKeysize(
    /* [out] */ Int32 *keySize)
{
    VALIDATE_NOT_NULL(keySize)
    *keySize = mKeysize;
    return NOERROR;
}

ECode CRSAKeyGenParameterSpec::GetPublicExponent(
    /* [out] */ IBigInteger **pubExponent)
{
    VALIDATE_NOT_NULL(pubExponent)
    *pubExponent = mPublicExponent;
    INTERFACE_ADDREF(*pubExponent)
    return NOERROR;
}

ECode CRSAKeyGenParameterSpec::constructor(
    /* [in] */ Int32 keysize,
    /* [in] */ IBigInteger *publicExponent)
{
    mKeysize = keysize;
    mPublicExponent = publicExponent;
    return NOERROR;
}

}
}
}
