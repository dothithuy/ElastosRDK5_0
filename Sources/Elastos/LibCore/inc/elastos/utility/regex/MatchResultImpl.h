
#ifndef __REGEX_MATCHRESULTIMPL_H__
#define __REGEX_MATCHRESULTIMPL_H__

#include <cmdef.h>
#include "Elastos.CoreLibrary_server.h"

namespace Elastos {
namespace Utility {
namespace Regex {


/**
 * Holds the results of a successful match of a regular expression against a
 * given string. Only used internally, thus sparsely documented (though the
 * defining public interface has full documentation).
 *
 * @see java.util.regex.MatchResult
 */
class MatchResultImpl
    : public ElRefBase
    , public IMatchResult
{
public:
    MatchResultImpl(
        /* [in] */ const String& text,
        /* [in] */ const ArrayOf<Int32>& offsets);

    CAR_INTERFACE_DECL()

    CARAPI End(
        /* [out] */ Int32* index);

    CARAPI EndEx(
        /* [in] */ Int32 group,
        /* [out] */ Int32* index);

    CARAPI Group(
        /* [out] */ String* text);

    CARAPI GroupEx(
        /* [in] */ Int32 group,
        /* [out] */ String* text);

    CARAPI GroupCount(
        /* [out] */ Int32* count);

    CARAPI StartEx(
        /* [in] */ Int32 group,
        /* [out] */ Int32* index);

    CARAPI Start(
        /* [out] */ Int32* index);

private:
    /**
     * Holds the original input text.
     */
    String mText;

    /**
     * Holds the offsets of the groups in the input text. The first two
     * elements specifiy start and end of the zero group, the next two specify
     * group 1, and so on.
     */
    AutoPtr<ArrayOf<Int32> > mOffsets;
};

} // namespace Regex
} // namespace Utility
} // namespace Elastos

#endif //__REGEX_MATCHRESULTIMPL_H__
