
#ifndef __CCONTACTSCONTRACTSYNCSTATE_H__
#define __CCONTACTSCONTRACTSYNCSTATE_H__

#include "_CContactsContractSyncState.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Accounts::IAccount;
using Elastos::Droid::Content::IContentProviderClient;
using Elastos::Droid::Content::IContentProviderOperation;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractSyncState)
{
public:
    /**
     * This utility class cannot be instantiated
     */
    CARAPI constructor();

    /**
     * The content:// style URI for this table
     */
    CARAPI GetCONTENTURI(
        /* [out] */ IUri** uri);

    /**
     * @see android.provider.SyncStateContract.Helpers#get
     */
    CARAPI Get(
        /* [in] */ IContentProviderClient* provider,
        /* [in] */ IAccount* account,
        /* [out] */ ArrayOf<Byte>** value);

    /**
     * @see android.provider.SyncStateContract.Helpers#set
     */
    CARAPI Set(
        /* [in] */ IContentProviderClient* provider,
        /* [in] */ IAccount* account,
        /* [in] */ const ArrayOf<Byte>& data);

    /**
     * @see android.provider.SyncStateContract.Helpers#newSetOperation
     */
    CARAPI NewSetOperation(
        /* [in] */ IAccount* account,
        /* [in] */ const ArrayOf<Byte>& data,
        /* [out] */ IContentProviderOperation** operation);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTSYNCSTATE_H__