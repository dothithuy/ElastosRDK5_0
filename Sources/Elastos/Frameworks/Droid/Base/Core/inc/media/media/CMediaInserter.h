
#ifndef __CMEDIAINSERTER_H__
#define __CMEDIAINSERTER_H__

#include "_CMediaInserter.h"
#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos/HashMap.h>

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::IIContentProvider;
using Elastos::Utility::HashMap;
using Elastos::Utility::List;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaInserter)
{
public:
    CMediaInserter();

    ~CMediaInserter();

    CARAPI Init(
        /* [in] */ IIContentProvider* provider,
        /* [in] */ Int32 bufferSizePerUri);

    CARAPI constructor(
        /* [in] */ IIContentProvider* provider,
        /* [in] */ Int32 bufferSizePerUri);

    CARAPI Insert(
        /* [in] */ IUri* tableUri,
        /* [in] */ IContentValues* values);

    CARAPI InsertwithPriority(
        /* [in] */ IUri* tableUri,
        /* [in] */ IContentValues* values);

    CARAPI FlushAll();

private:
    CARAPI Insert(
        /* [in] */ IUri* tableUri,
        /* [in] */ IContentValues* values,
        /* [in] */ Boolean priority);

    CARAPI FlushAllPriority();

    CARAPI Flush(
        /* [in] */ IUri* tableUri,
        /* [in] */ List< AutoPtr<IContentValues> >* list);

private:
    typedef List< AutoPtr<IContentValues> > ContentValuesList;
    typedef typename ContentValuesList::Iterator ContentValuesListIterator;
    typedef HashMap<AutoPtr<IUri>, AutoPtr<ContentValuesList> > UriContentValuesMap;
    typedef typename UriContentValuesMap::Iterator UriContentValuesMapIterator;
    UriContentValuesMap mRowMap;
    UriContentValuesMap mPriorityRowMap;

    AutoPtr<IIContentProvider> mProvider;

    Int32 mBufferSizePerUri;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CMEDIAINSERTER_H__
