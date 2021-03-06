
#ifndef __CDOMCONFIGURATIONIMPL_H__
#define __CDOMCONFIGURATIONIMPL_H__

#include "_CDOMConfigurationImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xml {
namespace Dom {

CarClass(CDOMConfigurationImpl)
{
public:
    CARAPI SetParameter(
        /* [in] */ const String& name,
        /* [in] */ IObject * pValue);

    CARAPI GetParameter(
        /* [in] */ const String& name,
        /* [out] */ IObject ** ppObj);

    CARAPI CanSetParameter(
        /* [in] */ const String& name,
        /* [in] */ IObject * pValue,
        /* [out] */ Boolean * pReslut);

    CARAPI GetParameterNames(
        /* [out] */ Org::W3c::Dom::IDOMStringList ** ppDomlist);

    CARAPI Normalize(
        /* [in] */ Org::W3c::Dom::INode * pNode);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CDOMCONFIGURATIONIMPL_H__
