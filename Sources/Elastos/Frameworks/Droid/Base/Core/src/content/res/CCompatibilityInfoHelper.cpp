
#include "ext/frameworkext.h"
#include "content/res/CCompatibilityInfoHelper.h"
#include "content/res/CCompatibilityInfo.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

ECode CCompatibilityInfoHelper::ComputeCompatibleScaling(
    /* [in] */ IDisplayMetrics* dm,
    /* [in] */ IDisplayMetrics* outDm,
    /* [out] */ Float* scale)
{
    VALIDATE_NOT_NULL(scale);
    *scale = CCompatibilityInfo::ComputeCompatibleScaling(dm, outDm);
    return NOERROR;
}

ECode CCompatibilityInfoHelper::GetDefault(
    /* [out] */ ICompatibilityInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = CCompatibilityInfo::DEFAULT_COMPATIBILITY_INFO;
    INTERFACE_ADDREF(*info);
    return NOERROR;
}

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

