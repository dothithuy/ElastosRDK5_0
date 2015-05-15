
#include "ext/frameworkext.h"
#include "content/res/CAssetManagerHelper.h"
#include "content/res/CAssetManager.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

ECode CAssetManagerHelper::GetSystem(
    /* [out] */ IAssetManager** am)
{
    VALIDATE_NOT_NULL(am);

    AutoPtr<CAssetManager> amObj = CAssetManager::GetSystem();
    *am = (IAssetManager*)amObj.Get();
    INTERFACE_ADDREF(*am);
    return NOERROR;
}

ECode CAssetManagerHelper::GetGlobalAssetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = CAssetManager::GetGlobalAssetCount();
    return NOERROR;
}

ECode CAssetManagerHelper::GetAssetAllocations(
    /* [out] */ String* allocations)
{
    VALIDATE_NOT_NULL(allocations);

    *allocations = CAssetManager::GetAssetAllocations();
    return NOERROR;
}

ECode CAssetManagerHelper::GetGlobalAssetManagerCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = CAssetManager::GetGlobalAssetManagerCount();
    return NOERROR;
}

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos