
#ifndef __CCONFIGURATIONHELPER_H__
#define __CCONFIGURATIONHELPER_H__

#include "_CConfigurationHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CarClass(CConfigurationHelper)
{
public:
    CARAPI ResetScreenLayout(
        /* [in] */ Int32 curLayout,
        /* [out] */ Int32* layout);

    CARAPI ReduceScreenLayout(
        /* [in] */ Int32 curLayout,
        /* [in] */ Int32 longSizeDp,
        /* [in] */ Int32 shortSizeDp,
        /* [out] */ Int32* layout);

    /**
     * Determine if a new resource needs to be loaded from the bit set of
     * configuration changes returned by {@link #updateFrom(Configuration)}.
     *
     * @param configChanges The mask of changes configurations as returned by
     * {@link #updateFrom(Configuration)}.
     * @param interestingChanges The configuration changes that the resource
     * can handled, as given in {@link android.util.TypedValue#changingConfigurations}.
     *
     * @return Return true if the resource needs to be loaded, else false.
     */
    CARAPI NeedNewResources(
        /* [in] */ Int32 configChanges,
        /* [in] */ Int32 interestingChanges,
        /* [out] */ Boolean * value);

    CARAPI GetEmpty(
        /* [out] */ IConfiguration** empty);
};

}
}
}
}

#endif // __CCONFIGURATIONHELPER_H__