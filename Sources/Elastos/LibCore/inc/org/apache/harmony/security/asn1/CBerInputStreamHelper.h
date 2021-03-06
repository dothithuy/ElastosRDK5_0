
#ifndef __CBERINPUTSTREAMHELPER_H__
#define __CBERINPUTSTREAMHELPER_H__

#include "_CBerInputStreamHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CBerInputStreamHelper)
{
public:
    CARAPI GetLength(
        /* [in] */ ArrayOf<Byte>* encoding,
        /* [out] */ Int32* length);
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CBERINPUTSTREAMHELPER_H__
