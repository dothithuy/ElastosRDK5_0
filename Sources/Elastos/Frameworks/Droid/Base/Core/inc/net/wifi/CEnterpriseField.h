
#ifndef __CENTERPRISEFIELD_H__
#define __CENTERPRISEFIELD_H__

#include "_CEnterpriseField.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CEnterpriseField)
{
public:
    CARAPI constructor(
        /* [in] */ const String& varName);

    CARAPI SetValue(
        /* [in] */ const String& value);

    CARAPI GetVarName(
        /* [out] */ String* varName);

    CARAPI GetValue(
        /* [out] */ String* value);

private:
    String mVarName;
    String mValue;
};

}
}
}
}

#endif // __CENTERPRISEFIELD_H__
