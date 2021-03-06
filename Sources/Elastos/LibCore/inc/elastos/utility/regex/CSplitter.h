
#ifndef __REGEX_SPLITTER_H__
#define __REGEX_SPLITTER_H__

#include <_CSplitter.h>
#include <cmdef.h>
#include <elastos/List.h>

using Elastos::Utility::List;

namespace Elastos {
namespace Utility {
namespace Regex {

/**
 * Used to make {@code String.split} fast (and to help {@code Pattern.split} too).
 * @hide
 */
CarClass(CSplitter)
{
public:

    /**
     * Returns a result equivalent to {@code s.split(separator, limit)} if it's able
     * to compute it more cheaply than ICU, or null if the caller should fall back to
     * using ICU.
     */
    CARAPI FastSplit(
        /* [in] */ const String& re,
        /* [in] */ const String& input,
        /* [in] */ Int32 limit,
        /* [out, callee] */ ArrayOf<String>** array);

    CARAPI Split(
        /* [in] */ IPattern* pattern,
        /* [in] */ const String& re,
        /* [in] */ const String& input,
        /* [in] */ Int32 limit,
        /* [out, callee] */ ArrayOf<String>** array);

private:
    CARAPI FinishSplit(
        /* [in] */ List<String>& list,
        /* [in] */ const String& input,
        /* [in] */ Int32 begin,
        /* [in] */ Int32 maxSize,
        /* [in] */ Int32 limit,
        /* [out, callee] */ ArrayOf<String>** array);

private:
    // The RI allows regular expressions beginning with ] or }, but that's probably a bug.
    static const String METACHARACTERS;
};

} // namespace Regex
} // namespace Utility
} // namespace Elastos

#endif //__SPLITTER_H__
