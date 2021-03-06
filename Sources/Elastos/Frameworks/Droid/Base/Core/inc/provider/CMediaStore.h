
#ifndef __CMEDIASTORE_H__
#define __CMEDIASTORE_H__

#include "_CMediaStore.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IBitmapFactoryOptions;
using Elastos::Droid::Database::ICursor;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CMediaStore)
{
public:
    /**
     * Uri for querying the state of the media scanner.
     */
    CARAPI GetMediaScannerUri(
        /* [out] */ IUri** uri);

    /**
     * Get the media provider's version.
     * Applications that import data from the media provider into their own caches
     * can use this to detect that the media provider changed, and reimport data
     * as needed. No other assumptions should be made about the meaning of the version.
     * @param context Context to use for performing the query.
     * @return A version string, or null if the version could not be determined.
     */
    CARAPI GetVersion(
        /* [in] */ IContext* context,
        /* [out] */ String* version);

private:
    static const String TAG;

    static const String CONTENT_AUTHORITY_SLASH;
};

} //namespace Provider
} //namespace Droid
} //namespace Elastos

#endif //__CMEDIASTORE_H__