
#ifndef __CBROWSERCONTRACTSYNCSTATE_H__
#define __CBROWSERCONTRACTSYNCSTATE_H__

#include "_CBrowserContractSyncState.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContractSyncState)
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
        /* [out] */ ArrayOf<Byte>* value);

    /**
     * @see android.provider.SyncStateContract.Helpers#get
     */
    CARAPI GetWithUri(
        /* [in] */ IContentProviderClient* provider,
        /* [in] */ IAccount* account,
        /* [out] */ Pair<IUri*, ArrayOf<Byte> >* value);

    /**
     * @see android.provider.SyncStateContract.Helpers#set
     */
    CARAPI Set(
        /* [in] */ IContentProviderClient* provider,
        /* [in] */ IAccount* account,
        /* [in] */ ArrayOf<Byte> data);

    /**
     * @see android.provider.SyncStateContract.Helpers#newSetOperation
     */
    CARAPI NewSetOperation(
        /* [in] */ IAccount* account,
        /* [in] */ ArrayOf<Byte> data,
        /* [out] */ IContentProviderOperation** operation);
};

}
}
}

#endif //__CBROWSERCONTRACTSYNCSTATE_H__