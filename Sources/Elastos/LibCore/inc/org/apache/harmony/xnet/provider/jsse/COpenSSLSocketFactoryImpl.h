
#ifndef __COPENSSLSOCKETFACTORYIMPL_H__
#define __COPENSSLSOCKETFACTORYIMPL_H__

#include "_COpenSSLSocketFactoryImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(COpenSSLSocketFactoryImpl)
{
public:
    CARAPI CreateSocket(
        /* [out] */ Elastos::Net::ISocket ** ppSock);

    CARAPI CreateSocketEx(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [out] */ Elastos::Net::ISocket ** ppSock);

    CARAPI CreateSocketEx2(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ Elastos::Net::IInetAddress * pLocalHost,
        /* [in] */ Int32 localPort,
        /* [out] */ Elastos::Net::ISocket ** ppSock);

    CARAPI CreateSocketEx3(
        /* [in] */ Elastos::Net::IInetAddress * pHost,
        /* [in] */ Int32 port,
        /* [out] */ Elastos::Net::ISocket ** ppSock);

    CARAPI CreateSocketEx4(
        /* [in] */ Elastos::Net::IInetAddress * pAddress,
        /* [in] */ Int32 port,
        /* [in] */ Elastos::Net::IInetAddress * pLocalAddress,
        /* [in] */ Int32 localPort,
        /* [out] */ Elastos::Net::ISocket ** ppSock);

    CARAPI GetDefaultCipherSuites(
        /* [out, callee] */ ArrayOf<String> ** ppSuites);

    CARAPI GetSupportedCipherSuites(
        /* [out, callee] */ ArrayOf<String> ** ppSuites);

    CARAPI CreateSocketEx5(
        /* [in] */ Elastos::Net::ISocket * pS,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ Boolean autoClose,
        /* [out] */ Elastos::Net::ISocket ** ppSock);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Elastosx::Net::Ssl::ISSLParameters * pSslParameters);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __COPENSSLSOCKETFACTORYIMPL_H__
