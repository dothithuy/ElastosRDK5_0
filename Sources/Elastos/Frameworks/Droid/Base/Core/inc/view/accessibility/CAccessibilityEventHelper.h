#ifndef __CACCESSIBILITYEVENTHELPER_H__
#define __CACCESSIBILITYEVENTHELPER_H__

#include "_CAccessibilityEventHelper.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityEventHelper)
{
public:
    /**
     * Returns a cached instance if such is available or a new one is
     * instantiated with its type property set.
     *
     * @param eventType The event type.
     * @return An instance.
     */
    CARAPI Obtain(
        /* [in] */ Int32 eventType,
        /* [out] */ IAccessibilityEvent** event);

    /**
     * Returns a cached instance if such is available or a new one is
     * created. The returned instance is initialized from the given
     * <code>event</code>.
     *
     * @param event The other event.
     * @return An instance.
     */
    CARAPI ObtainEx(
        /* [in] */ IAccessibilityEvent* otherEvent,
        /* [out] */ IAccessibilityEvent** event);

    /**
     * Returns a cached instance if such is available or a new one is
     * instantiated.
     *
     * @return An instance.
     */
    CARAPI ObtainEx2(
        /* [out] */ IAccessibilityEvent** event);

    /**
     * Returns the string representation of an event type. For example,
     * {@link #TYPE_VIEW_CLICKED} is represented by the string TYPE_VIEW_CLICKED.
     *
     * @param eventType The event type
     * @return The string representation.
     */
    CARAPI EventTypeToString(
        /* [in] */ Int32 eventType,
        /* [out] */ String* str);

};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__CACCESSIBILITYEVENTHELPER_H__
