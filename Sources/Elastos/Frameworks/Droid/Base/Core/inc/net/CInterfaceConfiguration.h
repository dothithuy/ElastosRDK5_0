
#ifndef __CINTERFACECONFIGURATION_H__
#define __CINTERFACECONFIGURATION_H__

#include "ext/frameworkdef.h"
#include "_CInterfaceConfiguration.h"
#include <elastos/HashSet.h>

using Elastos::Utility::HashSet;
using Elastos::Net::IInetAddress;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CInterfaceConfiguration)
{
public:
    CARAPI constructor();

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI GetFlags(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI HasFlag(
        /* [in] */ const String& flag,
        /* [out] */ Boolean* result);

    CARAPI ClearFlag(
        /* [in] */ const String& flag);

    CARAPI SetFlag(
        /* [in] */ const String& flag);

    /**
     * Set flags to mark interface as up.
     */
    CARAPI SetInterfaceUp();

    /**
     * Set flags to mark interface as down.
     */
    CARAPI SetInterfaceDown();

    CARAPI GetLinkAddress(
        /* [out] */ ILinkAddress** result);

    CARAPI SetLinkAddress(
        /* [in] */ ILinkAddress* addr);

    CARAPI GetHardwareAddress(
        /* [out] */ String* result);

    CARAPI SetHardwareAddress(
        /* [in] */ const String& hwAddr);

    /**
     * This function determines if the interface is up and has a valid IP
     * configuration (IP address has a non zero octet).
     *
     * Note: It is supposed to be quick and hence should not initiate
     * any network activity
     */
    CARAPI IsActive(
        /* [out] */ Boolean* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* src);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    static CARAPI ValidateFlag(
        /* [in] */ const String& flag);

private:
    String mHwAddr;
    AutoPtr<ILinkAddress> mAddr;
    HashSet<String> mFlags;

    static const String FLAG_UP;
    static const String FLAG_DOWN;
};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__CINTERFACECONFIGURATION_H__
