
#ifndef __CWIFICONFIGURATIONPROTOCOL_H__
#define __CWIFICONFIGURATIONPROTOCOL_H__

#include "_CWifiConfigurationProtocol.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiConfigurationProtocol)
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

#endif // __CWIFICONFIGURATIONPROTOCOL_H__
