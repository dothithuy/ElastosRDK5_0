
#ifndef __CERRORSTRINGS_H__
#define __CERRORSTRINGS_H__

#include "_CErrorStrings.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CarClass(CErrorStrings)
{
public:
    /**
     * Get the localized error message resource for the given error code.
     * If the code is unknown, we'll return a generic error message.
     */
    CARAPI GetString(
        /* [in] */ Int32 errorCode,
        /* [in] */ Elastos::Droid::Content::IContext* context,
        /* [out] */ String* str);

    /**
     * Get the localized error message resource for the given error code.
     * If the code is unknown, we'll return a generic error message.
     */
    CARAPI GetResource(
        /* [in] */ Int32 errorCode,
        /* [out] */ Int32* res);

private:
    static const String LOGTAG;
};

}
}
}
}

#endif // __CERRORSTRINGS_H__
