
#ifndef __CDOMERRORIMPL_H__
#define __CDOMERRORIMPL_H__

#include "_CDOMErrorImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xml {
namespace Dom {

CarClass(CDOMErrorImpl)
{
public:
    CARAPI GetSeverity(
        /* [out] */ Int16 * pValue);

    CARAPI GetMessage(
        /* [out] */ String * pStr);

    CARAPI GetType(
        /* [out] */ String * pStr);

    CARAPI GetRelatedException(
        /* [out] */ IObject ** ppObj);

    CARAPI GetRelatedData(
        /* [out] */ IObject ** ppObj);

    CARAPI GetLocation(
        /* [out] */ Org::W3c::Dom::IDOMLocator ** ppDomloc);

    CARAPI constructor(
        /* [in] */ Int32 severity,
        /* [in] */ const String& type);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CDOMERRORIMPL_H__
