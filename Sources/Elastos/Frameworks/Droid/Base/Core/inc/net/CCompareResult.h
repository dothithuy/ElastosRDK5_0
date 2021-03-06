
#ifndef __CCOMPARERESULT_H__
#define __CCOMPARERESULT_H__

#include "_CCompareResult.h"
#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos/StringBuffer.h>

using Elastos::Utility::List;
using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CCompareResult)
{
public:
    CARAPI constructor();

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI GetRemoved(
        /* [out] */ IObjectContainer** result);

    CARAPI AddRemoved(
        /* [in] */ IInterface* removed);

    CARAPI GetAdded(
        /* [out] */ IObjectContainer** result);

    CARAPI AddAdded(
        /* [in] */ IInterface* added);

public:
    List< AutoPtr<IInterface> > mRemoved;
    List< AutoPtr<IInterface> > mAdded;
};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __CCOMPARERESULT_H__
