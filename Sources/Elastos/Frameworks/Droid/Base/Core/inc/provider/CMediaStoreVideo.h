
#ifndef __CMEDIASTOREVIDEO_H__
#define __CMEDIASTOREVIDEO_H__

#include "_CMediaStoreVideo.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Database::ICursor;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CMediaStoreVideo)
{
public:
    CARAPI Query(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* uri,
        /* [in] */ ArrayOf<String>* projection,
        /* [out] */ ICursor** cursor);
};

} //namespace Provider
} //namespace Droid
} //namespace Elastos

#endif //__CMEDIASTOREVIDEO_H__