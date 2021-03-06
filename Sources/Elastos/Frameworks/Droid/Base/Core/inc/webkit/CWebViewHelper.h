
#ifndef __CWEBVIEWHELPER_H__
#define __CWEBVIEWHELPER_H__

#include "_CWebViewHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebViewHelper)
{
public:
    CARAPI EnablePlatformNotifications();

    CARAPI DisablePlatformNotifications();

    CARAPI FindAddress(
        /* [in] */ const String& addr,
        /* [out] */ String* outAddr);

    CARAPI GetPluginList(
        /* [out] */ IPluginList** pluginList);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CWEBVIEWHELPER_H__
