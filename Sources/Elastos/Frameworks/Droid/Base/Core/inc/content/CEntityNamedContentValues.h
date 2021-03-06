
#ifndef __CENTITYNAMEDCONTENTVALUES_H__
#define __CENTITYNAMEDCONTENTVALUES_H__

#include "_CEntityNamedContentValues.h"
#include <ext/frameworkext.h>

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContentValues;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CEntityNamedContentValues)
{
public:
    CEntityNamedContentValues();

    ~CEntityNamedContentValues();

    CARAPI GetUri(
        /* [out] */ IUri** uri);

    CARAPI GetValues(
        /* [out] */ IContentValues** values);

    CARAPI constructor(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values);

private:
    AutoPtr<IUri> mUri;
    AutoPtr<IContentValues> mContentValues;

};

} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CENTITYNAMEDCONTENTVALUES_H__
