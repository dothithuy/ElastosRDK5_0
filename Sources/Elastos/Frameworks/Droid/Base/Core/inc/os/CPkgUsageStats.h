#ifndef __CPkgUsageStats_H__
#define __CPkgUsageStats_H__

#include "_CPkgUsageStats.h"

using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {

CarClass(CPkgUsageStats)
{
public:
    CARAPI constructor(
        /* [in] */ const String& pkgName,
        /* [in] */ Int32 count,
        /* [in] */ Int64 time,
        /* [in] */ IObjectStringMap* lastResumeTimes);

    CARAPI constructor(
        /* [in] */ IParcel* source);

    CARAPI constructor(
        /* [in] */ Handle32 pStats);

    CARAPI DescribeContents(
        /* [out] */ Int32* contents);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* in);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    String mPackageName;
    Int32 mLaunchCount;
    Int64 mUsageTime;
    AutoPtr<IObjectStringMap> mComponentResumeTimes;
};

} // namespace Os
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__CPkgUsageStats_H__
