
#ifndef __CCONTACTSCONTRACTPHONELOOKUP_H__
#define __CCONTACTSCONTRACTPHONELOOKUP_H__

#include "_CContactsContractPhoneLookup.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractPhoneLookup)
{
public:
    /**
     * This utility class cannot be instantiated
     */
    CARAPI constructor();

    /**
     * The content:// style URI for this table. Append the phone number you want to lookup
     * to this URI and query it to perform a lookup. For example:
     * <pre>
     * Uri lookupUri = Uri.withAppendedPath(PhoneLookup.CONTENT_URI, Uri.encode(phoneNumber));
     * </pre>
     */
    CARAPI GetCONTENTFILTERURI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTPHONELOOKUP_H__