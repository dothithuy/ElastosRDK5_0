
#ifndef __CPACKAGEITEMINFODISPLAYNAMECOMPARATOR_H__
#define __CPACKAGEITEMINFODISPLAYNAMECOMPARATOR_H__

#include "_CPackageItemInfoDisplayNameComparator.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CPackageItemInfoDisplayNameComparator)
{
public:
    CARAPI constructor(
        /* [in] */ IPackageManager* pm);

    CARAPI Compare(
        /* [in] */ IPackageItemInfo* aa,
        /* [in] */ IPackageItemInfo* ab,
        /* [out] */ Int32* result);

private:
    // private final Collator   sCollator = Collator.getInstance();
    AutoPtr<IPackageManager> mPM;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CPACKAGEITEMINFODISPLAYNAMECOMPARATOR_H__
