
#ifndef __CATTRIBUTETYPEANDVALUE_H__
#define __CATTRIBUTETYPEANDVALUE_H__

#include "_CAttributeTypeAndValue.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X501 {

CarClass(CAttributeTypeAndValue)
{
public:
    CARAPI AppendName(
        /* [in] */ const String& attrFormat,
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI GetType(
        /* [out] */ Org::Apache::Harmony::Security::Utils::IObjectIdentifierInUtils** ppIdentifier);

    CARAPI constructor(
        /* [in] */ const String& sOid,
        /* [in] */ Org::Apache::Harmony::Security::X501::IAttributeValue * pValue);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CATTRIBUTETYPEANDVALUE_H__
