
#ifndef __WEBVIEWFACTORY_H__
#define __WEBVIEWFACTORY_H__

#include "ext/frameworkext.h"
#include "webkit/WebViewFactoryProvider.h"
#include <elastos/Mutex.h>

using Elastos::Core::IClassLoader;
using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Top level factory, used creating all the main WebView implementation classes.
 */
class WebViewFactory
{
public:
    static CARAPI_(AutoPtr<IWebViewFactoryProvider>) GetProvider();

private:
    // TODO: This allows us to have the legacy and Chromium WebView coexist for development
    // and side-by-side testing. After transition, remove this when no longer required.
    static CARAPI_(AutoPtr<IWebViewFactoryProvider>) LoadChromiumProvider();

    static CARAPI_(AutoPtr<IWebViewFactoryProvider>) GetFactoryByName(
        /* [in] */ const String& providerName,
        /* [in] */ IClassLoader* loader);

private:
    // Default Provider factory class name.
    // TODO: When the Chromium powered WebView is ready, it should be the default factory class.
    static const String DEFAULT_WEBVIEW_FACTORY;
    static const String CHROMIUM_WEBVIEW_FACTORY;
    static const String CHROMIUM_WEBVIEW_JAR;

    static const String LOGTAG;

    static const Boolean DEBUG = FALSE;

    // Cache the factory both for efficiency, and ensure any one process gets all webviews from the
    // same provider.
    static AutoPtr<IWebViewFactoryProvider> sProviderInstance;
    static Mutex sProviderLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__WEBVIEWFACTORY_H__
