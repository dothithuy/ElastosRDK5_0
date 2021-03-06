
#ifndef _CAccessibilityManagerHelper_
#define _CAccessibilityManagerHelper_

#include "_CAccessibilityManagerHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityManagerHelper)
{
public:
    /**
     * Creates the singleton AccessibilityManager to be shared across users. This
     * has to be called before the local AccessibilityManager is created to ensure
     * it registers itself in the system correctly.
     * <p>
     * Note: Calling this method requires INTERACT_ACROSS_USERS_FULL or
     *       INTERACT_ACROSS_USERS permission.
     * </p>
     * @param context Context in which this manager operates.
     * @throws IllegalStateException if not called before the local
     *     AccessibilityManager is instantiated.
     *
     * @hide
     */
    CARAPI CreateAsSharedAcrossUsers(
        /* [in] */ IContext* context);

    /**
     * Get an AccessibilityManager instance (create one if necessary).
     *
     * @param context Context in which this manager operates.
     *
     * @hide
     */
    CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IAccessibilityManager** manager);
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //_CAccessibilityManagerHelper_
