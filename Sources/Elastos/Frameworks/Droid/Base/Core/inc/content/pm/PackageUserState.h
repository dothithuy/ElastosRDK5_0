
#ifndef __CPACKAGEUSERSTATE_H__
#define __CPACKAGEUSERSTATE_H__

#include "ext/frameworkext.h"
#include <elastos/HashSet.h>

using Elastos::Utility::HashSet;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Per-user state information about a package.
 * @hide
 */
class PackageUserState : public ElRefBase
{
public:
    PackageUserState();

    PackageUserState(
        /* [in] */ PackageUserState* o);

    ~PackageUserState();

public:
    Boolean mStopped;
    Boolean mNotLaunched;
    Boolean mInstalled;
    Int32 mEnabled;

    AutoPtr<HashSet<String> > mDisabledComponents;
    AutoPtr<HashSet<String> > mEnabledComponents;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CPACKAGEUSERSTATE_H__
