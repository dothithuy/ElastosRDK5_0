
#include "content/CClipDescriptionHelper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CClipDescriptionHelper::CompareMimeTypes(
    /* [in] */ const String& concreteType,
    /* [in] */ const String& desiredType,
    /* [out] */ Boolean* isMatch)
{
    VALIDATE_NOT_NULL(isMatch)
    const Int32 typeLength = desiredType.GetLength();
    if (typeLength == 3 && desiredType.Equals(String("*/*"))) {
        *isMatch = TRUE;
        return NOERROR;
    }

    const Int32 slashpos = desiredType.IndexOf('/');
    if (slashpos > 0) {
        if (typeLength == slashpos+2 && desiredType.GetChar(slashpos+1) == '*') {
            if (desiredType.RegionMatches(0, concreteType, 0, slashpos+1)) {
                *isMatch = TRUE;
                return NOERROR;
            }
        }
        else if (desiredType.Equals(concreteType)) {
            *isMatch = TRUE;
            return NOERROR;
        }
    }

    *isMatch = FALSE;
    return NOERROR;
}

}
}
}

