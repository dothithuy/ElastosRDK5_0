
#ifndef __CCONTACTSCONTRACTPROVIDERSTATUS_H__
#define __CCONTACTSCONTRACTPROVIDERSTATUS_H__

#include "_CContactsContractProviderStatus.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractProviderStatus)
{
public:
    /**
     * Not instantiable.
     */
    CARAPI constructor();

    /**
     * The content:// style URI for this table.  Requests to this URI can be
     * performed on the UI thread because they are always unblocking.
     *
     * @hide
     */
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTPROVIDERSTATUS_H__