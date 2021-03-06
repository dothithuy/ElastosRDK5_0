
#ifndef __SYNCADAPTERSCACHE_H__
#define __SYNCADAPTERSCACHE_H__

#include <ext/frameworkext.h>
#include "content/pm/RegisteredServicesCache.h"

using namespace Elastos::Droid::Content::Pm;
using namespace Elastos::Droid::Content::Res;
using Elastos::Droid::Utility::IAttributeSet;
using Org::Xmlpull::V1::IXmlPullParser;
using Org::Xmlpull::V1::IXmlSerializer;

namespace Elastos {
namespace Droid {
namespace Content {

/**
 * A cache of services that export the {@link android.content.ISyncAdapter} interface.
 * @hide
 */
class SyncAdaptersCache
    : public RegisteredServicesCache
{
public:
    class MySerializer
        : public ElRefBase
        , public IXmlSerializerAndParser
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI WriteAsXml(
            /* [in] */ IInterface* item,
            /* [in] */ IXmlSerializer* serializer);

        CARAPI CreateFromXml(
            /* [in] */ IXmlPullParser* parser,
            /* [out] */ IInterface** obj);

    };

public:
    SyncAdaptersCache(
        /* [in] */ IContext* context);

    ~SyncAdaptersCache();

    CARAPI ParseServiceAttributes(
        /* [in] */ IResources* res,
        /* [in] */ const String& packageName,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IInterface** syncAdapterType);

private:
    static const String TAG;
    static const String SERVICE_INTERFACE;
    static const String SERVICE_META_DATA;
    static const String ATTRIBUTES_NAME;
    static const AutoPtr<SyncAdaptersCache::MySerializer> sSerializer;

};

}
}
}

#endif // __SYNCADAPTERSCACHE_H__
