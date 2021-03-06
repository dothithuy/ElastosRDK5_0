
#ifndef __CWIFICONFIGURATIONKEYMGMT_H__
#define __CWIFICONFIGURATIONKEYMGMT_H__

#include "_CWifiConfigurationKeyMgmt.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiConfigurationKeyMgmt)
{
public:
    CARAPI GetStrings(
        /* [out, callee] */ ArrayOf<String>** strings);

    CARAPI ToString(
        /* [in] */ Int32 index,
        /* [out] */ String* strValue);

public:
    static const AutoPtr< ArrayOf<String> > sStrings;
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __CWIFICONFIGURATIONKEYMGMT_H__
