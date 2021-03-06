
#include "cmdef.h"
#include "CNormalizerHelper.h"
#include "CNativeNormalizerHelper.h"

using Libcore::ICU::INativeNormalizerHelper;
using Libcore::ICU::CNativeNormalizerHelper;

namespace Elastos {
namespace Text {

ECode CNormalizerHelper::IsNormalized(
    /* [in] */ ICharSequence* src,
    /* [in] */ NormalizerForm form,
    /* [out] */ Boolean* isNormalized)
{
    VALIDATE_NOT_NULL(src)
    VALIDATE_NOT_NULL(isNormalized)

    AutoPtr<INativeNormalizerHelper> normhelper;
    FAIL_RETURN(CNativeNormalizerHelper::AcquireSingleton((INativeNormalizerHelper **)&normhelper));
    return normhelper->IsNormalized(src,form,isNormalized);
}

ECode CNormalizerHelper::Normalize(
    /* [in] */ ICharSequence* src,
    /* [in] */ NormalizerForm form,
    /* [out] */ String* normalizeString)
{
    VALIDATE_NOT_NULL(src)
    VALIDATE_NOT_NULL(normalizeString)

    AutoPtr<INativeNormalizerHelper> normhelper;
    FAIL_RETURN(CNativeNormalizerHelper::AcquireSingleton((INativeNormalizerHelper **)&normhelper));
    return normhelper->Normalize(src,form,normalizeString);
}

} // namespace Text
} // namespace Elastos

