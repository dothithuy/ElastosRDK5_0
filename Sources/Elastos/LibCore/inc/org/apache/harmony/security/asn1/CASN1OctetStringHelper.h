
#ifndef __CASN1OCTETSTRINGHELPER_H__
#define __CASN1OCTETSTRINGHELPER_H__

#include "_CASN1OctetStringHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1OctetStringHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IASN1Type** instance);

private:
    // TODO: Add your private member variables here.
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CASN1OCTETSTRINGHELPER_H__
