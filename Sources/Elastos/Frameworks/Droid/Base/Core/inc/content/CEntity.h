
#ifndef __CENTITY_H__
#define __CENTITY_H__

#include "_CEntity.h"
#include <ext/frameworkext.h>
#include <elastos/List.h>
#include <elastos/StringBuilder.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility;
using namespace Elastos::Droid::Net;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CEntity)
{
public:
    CEntity();

    ~CEntity();

    CARAPI GetEntityValues(
        /* [out] */ IContentValues** entityValues);

    CARAPI GetSubValues(
        /* [out] */ IObjectContainer** subValues);

    CARAPI AddSubValue(
        /* [in] */ IUri* uri,
        /* [in] */ IContentValues* values);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI constructor(
        /* [in] */ IContentValues* values);

private:
    AutoPtr<IContentValues> mValues;
    List<AutoPtr<IEntityNamedContentValues> > mSubValues;

};

}
}
}

#endif // __CENTITY_H__
