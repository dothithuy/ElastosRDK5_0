
#ifndef __JARHANDLER_H__
#define __JARHANDLER_H__

#include "Elastos.CoreLibrary_server.h"
#include "URLStreamHandler.h"

using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;

namespace Elastos {
namespace Net {
namespace Url {

class JarHandler
    : public ElRefBase
    , public IJarHandler
    , public URLStreamHandler
{
public:
    CAR_INTERFACE_DECL();

    virtual CARAPI OpenConnectionEx(
        /* [in] */ IURL* u,
        /* [in] */ IProxy* proxy,
        /* [out] */ IURLConnection** urlConnection);

    virtual CARAPI SetURL(
        /* [in] */ IURL* u,
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file,
        /* [in] */ const String& ref);

    virtual CARAPI SetURLEx(
        /* [in] */ IURL* u,
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& authority,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& file,
        /* [in] */ const String& query,
        /* [in] */ const String& ref);

    virtual CARAPI ToExternalFormEx(
        /* [in] */ IURL* url,
        /* [in] */ Boolean escapeIllegalCharacters,
        /* [out] */ String* s);

    virtual CARAPI EqualsEx(
        /* [in] */ IURL* url1,
        /* [in] */ IURL* url2,
        /* [out] */ Boolean* isEquals);

    virtual CARAPI GetDefaultPort(
        /* [out] */ Int32* portNum);

    virtual CARAPI GetHostAddress(
        /* [in] */ IURL* url,
        /* [out] */ IInetAddress** hostAddress);

    virtual CARAPI GetHashCode(
        /* [in] */ IURL* url,
        /* [out] */ Int32* code);

    virtual CARAPI HostsEqual(
        /* [in] */ IURL* a,
        /* [in] */ IURL* b,
        /* [out] */ Boolean* isEqual);

    virtual CARAPI SameFile(
        /* [in] */ IURL* url1,
        /* [in] */ IURL* url2,
        /* [out] */ Boolean* isSame);

protected:
    CARAPI OpenConnection(
        /* [in] */ IURL* u,
        /* [out] */ IURLConnection** urlConnection);

    CARAPI ParseURL(
        /* [in] */ IURL* url,
        /* [in] */ const String& spec,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI ToExternalForm(
        /* [in] */ IURL* url,
        /* [out] */ String* s);
};

} // namespace Url
} // namespace Net
} // namespace Elastos

#endif //__JARHANDLER_H__
