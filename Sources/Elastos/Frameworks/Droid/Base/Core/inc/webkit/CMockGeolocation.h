
#ifndef __CMOCKGEOLOCATION_H__
#define __CMOCKGEOLOCATION_H__

#include "_CMockGeolocation.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * Used to configure the mock Geolocation client for the LayoutTests.
 * @hide
 */
CarClass(CMockGeolocation)
{
public:
    CARAPI constructor(
        /* [in] */ IWebViewCore* webViewCore);

    /**
     * Sets use of the mock Geolocation client. Also resets that client.
     */
    CARAPI SetUseMock();

    /**
     * Set the position for the mock Geolocation service.
     */
    CARAPI SetPosition(
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Double accuracy);

    /**
     * Set the error for the mock Geolocation service.
     */
    CARAPI SetError(
        /* [in] */ Int32 code,
        /* [in] */ const String& message);

    CARAPI SetPermission(
        /* [in] */ Boolean allow);

private:
    // Native functions
    static CARAPI_(void) NativeSetUseMock(
        /* [in] */ IWebViewCore* webViewCore);

    static CARAPI_(void) NativeSetPosition(
        /* [in] */ IWebViewCore* webViewCore,
        /* [in] */ Double latitude,
        /* [in] */ Double longitude,
        /* [in] */ Double accuracy);

    static CARAPI_(void) NativeSetError(
        /* [in] */ IWebViewCore* webViewCore,
        /* [in] */ Int32 code,
        /* [in] */ const String& message);

    static CARAPI_(void) NativeSetPermission(
        /* [in] */ IWebViewCore* webViewCore,
        /* [in] */ Boolean allow);

private:
    AutoPtr<IWebViewCore> mWebViewCore;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__CMOCKGEOLOCATION_H__
