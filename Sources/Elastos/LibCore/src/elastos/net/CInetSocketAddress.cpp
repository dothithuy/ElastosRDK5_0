
#include "cmdef.h"
#include "CInetSocketAddress.h"
#include "CInet4Address.h"
#include "InetAddress.h"
#include "StringUtils.h"

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Net {

const Int64 CInetSocketAddress::sSerialVersionUID = 5076001401234631237ll;

ECode CInetSocketAddress::constructor()
{
    // These will be filled in the native implementation of recvfrom.
    mAddr = NULL;
    mHostname = NULL;
    mPort = -1;
}

ECode CInetSocketAddress::constructor(
    /* [in] */ Int32 port)
{
    return constructor(NULL, port);
}

ECode CInetSocketAddress::constructor(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    if (port < 0 || port > 65535) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (address == NULL) {
        mAddr = CInet4Address::ANY;
    }
    else {
        mAddr = address;
    }
    mAddr->GetHostName(&mHostname);
    mPort = port;

    return NOERROR;
}

ECode CInetSocketAddress::constructor(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    return constructor(host, port, TRUE);
}

ECode CInetSocketAddress::constructor(
    /* [in] */ const String& hostname,
    /* [in] */ Int32 port,
    /* [in] */ Boolean needResolved)
{
    if (hostname.IsNull() || port < 0 || port > 65535) {
//        throw new IllegalArgumentException("host=" + hostname + ", port=" + port);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String _hostname = hostname;
    AutoPtr<IInetAddress> addr;
    if (needResolved) {
        // SecurityManager smgr = System.getSecurityManager();
        // if (smgr != null) {
        //     smgr.checkConnect(hostname, port);
        // }
        // try {
        ECode ec = InetAddress::GetByName(hostname, (IInetAddress**)&addr);
        if (ec == NOERROR) {
            _hostname = NULL;
        }
        // } catch (UnknownHostException ignored) {
        // }
    }
    mAddr = addr;
    mHostname = _hostname;
    mPort = port;

    return NOERROR;
}

ECode CInetSocketAddress::CreateUnresolved(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [out] */ IInetSocketAddress** addr)
{
    VALIDATE_NOT_NULL(addr);

    return CInetSocketAddress::New(host, port, FALSE, addr);
}

ECode CInetSocketAddress::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    *port = mPort;

    return NOERROR;
}

ECode CInetSocketAddress::GetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    *address = mAddr;
    INTERFACE_ADDREF(*address);

    return NOERROR;
}

ECode CInetSocketAddress::GetHostName(
    /* [out] */ String* hostname)
{
    VALIDATE_NOT_NULL(hostname);

    if (mAddr != NULL) {
        return mAddr->GetHostName(hostname);
    }
    else {
        *hostname = mHostname;
        return NOERROR;
    }
}

ECode CInetSocketAddress::GetHostString(
    /* [out] */ String* hostString)
{
    VALIDATE_NOT_NULL(hostString)

    String str;
    *hostString = (!mHostname.IsNull()) ? mHostname : (mAddr->GetHostAddress(&str), str);
    return NOERROR;
}

ECode CInetSocketAddress::IsUnresolved(
    /* [out] */ Boolean* isUnresolved)
{
    VALIDATE_NOT_NULL(isUnresolved);

    *isUnresolved = mAddr == NULL;

    return NOERROR;
}

ECode CInetSocketAddress::Equals(
    /*[in] */ IInterface* socketAddr,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (this->Probe(EIID_IInterface) == socketAddr) {
        *result = TRUE;
        return NOERROR;
    }
    if (!(IInetSocketAddress::Probe(socketAddr))) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<CInetSocketAddress> iSockAddr = (CInetSocketAddress*) IInetSocketAddress::Probe(socketAddr);

    // check the ports as we always need to do this
    if (mPort != iSockAddr->mPort) {
        *result = FALSE;
        return NOERROR;
    }

    // we only use the hostnames in the comparison if the addrs were not
    // resolved
    if ((mAddr == NULL) && (iSockAddr->mAddr == NULL)) {
        *result = mHostname.Equals(iSockAddr->mHostname);
        return NOERROR;
    }

    // addrs were resolved so use them for the comparison
    if (mAddr == NULL) {
        // if we are here we know iSockAddr is not null so just return
        // false
        *result = FALSE;
        return NOERROR;
    }
    return mAddr->Equals(iSockAddr->mAddr, result);
}

ECode CInetSocketAddress::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    String str;
    *result = ((mAddr != NULL) ? (mAddr->ToString(&str), str) : mHostname)
                + String(":")
                + StringUtils::Int32ToString(mPort);
    return NOERROR;
}

ECode CInetSocketAddress::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)

    if (mAddr == NULL) {
        *hashCode = mHostname.GetHashCode() + mPort;
        return NOERROR;
    }
    Int32 addrcode = 0;
    *hashCode = (mAddr->GetHashCode(&addrcode), addrcode) + mPort;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
