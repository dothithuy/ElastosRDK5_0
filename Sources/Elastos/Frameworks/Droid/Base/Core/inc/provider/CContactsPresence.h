
#ifndef __CCONTACTSPRESENCE_H__
#define __CCONTACTSPRESENCE_H__

#include "_CContactsPresence.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Widget::IImageView;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsPresence)
{
public:
    CARAPI constructor();

    /**
     * The content:// style URL for this table
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);

    /**
     * Gets the resource ID for the proper presence icon.
     *
     * @param status the status to get the icon for
     * @return the resource ID for the proper presence icon
     * @deprecated see {@link android.provider.ContactsContract}
     */
    // @Deprecated
    CARAPI GetPresenceIconResourceId(
        /* [in] */ Int32 status,
        /* [out] */ Int32* id);

    /**
     * Sets a presence icon to the proper graphic
     *
     * @param icon the icon to to set
     * @param serverStatus that status
     * @deprecated see {@link android.provider.ContactsContract}
     */
    // @Deprecated
    CARAPI SetPresenceIcon(
        /* [in] */ IImageView* icon,
        /* [in] */ Int32 serverStatus);
};

} // Provider
} // Droid
} // Elastos

#endif //__CCONTACTSPRESENCE_H__