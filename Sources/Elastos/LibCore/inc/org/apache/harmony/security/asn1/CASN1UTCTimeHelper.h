
#ifndef __CASN1UTCTIMEHELPER_H__
#define __CASN1UTCTIMEHELPER_H__

#include "_CASN1UTCTimeHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1UTCTimeHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IASN1UTCTime** instance);
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CASN1UTCTIMEHELPER_H__
