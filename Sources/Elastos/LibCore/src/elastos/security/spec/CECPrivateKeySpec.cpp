
#include "CECPrivateKeySpec.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {
namespace Spec {


ECode CECPrivateKeySpec::GetParams(
    /* [out] */ IECParameterSpec **params)
{
    VALIDATE_NOT_NULL(params)
    *params = mParams;
    INTERFACE_ADDREF(*params)
    return NOERROR;
}

ECode CECPrivateKeySpec::GetS(
    /* [out] */ IBigInteger **s)
{
    VALIDATE_NOT_NULL(s)
    *s = mS;
    INTERFACE_ADDREF(*s)
    return NOERROR;
}

ECode CECPrivateKeySpec::constructor(
    /* [in] */ IBigInteger *s,
    /* [in] */ IECParameterSpec *params)
{
    mS = s;
    mParams = params;
    // throw NullPointerException if s or params is null
    if (mS == NULL || mParams == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    return NOERROR;
}

}
}
}

