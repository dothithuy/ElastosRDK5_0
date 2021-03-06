
#ifndef __CVPNCONFIGHELPER_H__
#define __CVPNCONFIGHELPER_H__

#include "_CVpnConfigHelper.h"

using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

CarClass(CVpnConfigHelper)
{
public:
    CARAPI GetIntentForConfirmation(
        /* [out] */ IIntent** intent);

    CARAPI GetIntentForStatusPanel(
        /* [in] */ IContext* context,
        /* [in] */ IVpnConfig* config,
        /* [out] */ IPendingIntent** intent);
};

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__CVPNCONFIGHELPER_H__
