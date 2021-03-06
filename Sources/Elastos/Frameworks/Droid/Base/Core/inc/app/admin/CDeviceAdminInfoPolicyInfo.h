
#ifndef __CDEVICEADMININFOPOLICYINFO_H__
#define __CDEVICEADMININFOPOLICYINFO_H__

#include "_CDeviceAdminInfoPolicyInfo.h"
#include "ext/frameworkext.h"


namespace Elastos {
namespace Droid {
namespace App {
namespace Admin {

CarClass(CDeviceAdminInfoPolicyInfo)
{
public:
    CARAPI constructor(
        /* [in] */ Int32 identIn,
        /* [in] */ const String& tagIn,
        /* [in] */ Int32 labelIn,
        /* [in] */ Int32 descriptionIn);

    CARAPI GetIdent(
        /* [out] */ Int32* ident);

    CARAPI SetIdent(
        /* [in] */ Int32 ident);

    CARAPI GetTag(
        /* [out] */ String* tag);

    CARAPI SetTag(
        /* [in] */ const String& tag);

    CARAPI GetLabel(
        /* [out] */ Int32* label);

    CARAPI SetLabel(
        /* [in] */ Int32 label);

    CARAPI GetDescription(
        /* [out] */ Int32* description);

    CARAPI SetDescription(
        /* [in] */ Int32 description);

private:
    Int32 mIdent;
    String mTag;
    Int32 mLabel;
    Int32 mDescription;
};

} // namespace Admin
} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CDEVICEADMININFOPOLICYINFO_H__
