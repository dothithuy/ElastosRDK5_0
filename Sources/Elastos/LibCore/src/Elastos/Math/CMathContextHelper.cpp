
#include "cmdef.h"
#include "CMathContextHelper.h"
#include "CMathContext.h"

namespace Elastos {
namespace Math {

ECode CMathContextHelper::GetDECIMAL32(
    /* [out] */ IMathContext** context)
{
    VALIDATE_NOT_NULL(context);
    *context = CMathContext::DECIMAL32;
    INTERFACE_ADDREF(*context);
    return NOERROR;
}

ECode CMathContextHelper::GetDECIMAL64(
    /* [out] */ IMathContext** context)
{
    VALIDATE_NOT_NULL(context);
    *context = CMathContext::DECIMAL64;
    INTERFACE_ADDREF(*context);
    return NOERROR;
}

ECode CMathContextHelper::GetDECIMAL128(
    /* [out] */ IMathContext** context)
{
    VALIDATE_NOT_NULL(context);
    *context = CMathContext::DECIMAL128;
    INTERFACE_ADDREF(*context);
    return NOERROR;
}

ECode CMathContextHelper::GetUNLIMITED(
    /* [out] */ IMathContext** context)
{
    VALIDATE_NOT_NULL(context);
    *context = CMathContext::UNLIMITED;
    INTERFACE_ADDREF(*context);
    return NOERROR;
}

} // namespace Math
} // namespace Elastos