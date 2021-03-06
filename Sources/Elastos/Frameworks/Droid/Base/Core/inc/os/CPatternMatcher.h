#ifndef __CPATTERNMATCHER_H__
#define __CPATTERNMATCHER_H__

#include "_CPatternMatcher.h"
#include "os/PatternMatcher.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CPatternMatcher), public PatternMatcher
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ Int32 type);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetType(
        /* [out] */ Int32* path);

    CARAPI Match(
        /* [in] */ const String& str,
        /* [out] */ Boolean* isMatch);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ToString(
        /* [out] */ String* str);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __CPATTERNMATCHER_H__
