
#ifndef __CCONTACTS_H__
#define __CCONTACTS_H__

#include "_CContacts.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContacts)
{
public:
    /**
     * no public constructor since this is a utility class
     */
    CARAPI constructor();

    /**
     * The content:// style URL for this provider
     * @deprecated see {@link android.provider.ContactsContract}
     */
    //@Deprecated
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);

private:
    static const CString TAG;
};

} // Provider
} // Droid
} // Elastos

#endif //__CCONTACTS_H__