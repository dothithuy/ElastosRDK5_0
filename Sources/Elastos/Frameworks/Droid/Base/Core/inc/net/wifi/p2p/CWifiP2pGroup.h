
#ifndef __CWIFIP2PGROUP_H__
#define __CWIFIP2PGROUP_H__

#include "_CWifiP2pGroup.h"
#include <ext/frameworkext.h>
#include <elastos/List.h>

using Elastos::Utility::List;
using Elastos::Utility::Regex::IPattern;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {

CarClass(CWifiP2pGroup)
{
public:
    CWifiP2pGroup();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& supplicantEvent);

    CARAPI constructor(
        /* [in] */ IWifiP2pGroup* source);

    CARAPI SetNetworkName(
        /* [in] */ const String& networkName);

    CARAPI GetNetworkName(
        /* [out] */ String* networkName);

    CARAPI SetIsGroupOwner(
        /* [in] */ Boolean isGo);

    CARAPI IsGroupOwner(
        /* [out] */ Boolean* isGroupOwner);

    CARAPI SetOwner(
        /* [in] */ IWifiP2pDevice* device);

    CARAPI GetOwner(
        /* [out] */ IWifiP2pDevice** device);

    CARAPI AddClient(
        /* [in] */ const String& address);

    CARAPI AddClientEx(
        /* [in] */ IWifiP2pDevice* device);

    CARAPI RemoveClient(
        /* [in] */ const String& address,
        /* [out] */ Boolean* ret);

    CARAPI RemoveClientEx(
        /* [in] */ IWifiP2pDevice* device,
        /* [out] */ Boolean* ret);

    CARAPI IsClientListEmpty(
        /* [out] */ Boolean* ret);

    CARAPI Contains(
        /* [in] */ IWifiP2pDevice* device,
        /* [out] */ Boolean* ret);

    CARAPI GetClientList(
        /* [out, callee] */ ArrayOf<IWifiP2pDevice*>** list);

    CARAPI SetPassphrase(
        /* [in] */ const String& passphrase);

    CARAPI GetPassphrase(
        /* [out] */ String* passphrase);

    CARAPI SetInterface(
        /* [in] */ const String& intf);

    CARAPI GetInterface(
        /* [out] */ String* interfaceName);

    CARAPI GetNetworkId(
        /* [out] */ Int32* networkId);

    CARAPI SetNetworkId(
        /* [in] */ Int32 netId);

    CARAPI ToString(
        /* [out] */ String* string);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    /** The network name */
    String mNetworkName;

    /** Group owner */
    AutoPtr<IWifiP2pDevice> mOwner;

    /** Group clients */
    List<AutoPtr<IWifiP2pDevice> > mClients;

    /** The passphrase used for WPA2-PSK */
    String mPassphrase;

    String mInterface;

    /** The network id in the wpa_supplicant */
    Int32 mNetId;

    /** Device is group owner */
    Boolean mIsGroupOwner;

    /** P2P group started string pattern */
    static AutoPtr<IPattern> mGroupStartedPattern;
};

}
}
}
}
}

#endif // __CWIFIP2PGROUP_H__
