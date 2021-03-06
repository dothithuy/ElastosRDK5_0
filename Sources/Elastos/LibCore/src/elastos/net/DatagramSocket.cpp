
#include "cmdef.h"
#include "DatagramSocket.h"
#include "CInet4Address.h"
#include "CDatagramPacket.h"
#include "CInetSocketAddress.h"
#include "CInteger32.h"
#include "CBoolean.h"
#include "CPlainDatagramSocketImpl.h"
#include "CLibcore.h"
#include "COsConstants.h"
#include <cutils/log.h>

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Libcore::IO::IOs;
using Libcore::IO::CLibcore;
using Libcore::IO::COsConstants;

namespace Elastos {
namespace Net {

const String DatagramSocket::TAG("DatagramSocket");
AutoPtr<IDatagramSocketImplFactory> DatagramSocket::mFactory;
Mutex DatagramSocket::sLock;

DatagramSocket::DatagramSocket()
    : mPort(-1)
    , mIsBound(FALSE)
    , mIsConnected(FALSE)
    , mPendingConnectException(NOERROR)
    , mIsClosed(FALSE)
{}

ECode DatagramSocket::Init()
{
    return Init(0);
}

ECode DatagramSocket::Init(
    /* [in] */ Int32 aPort)
{
    FAIL_RETURN(CheckPort(aPort));
    return CreateSocket(aPort, CInet4Address::ANY.Get());
}

ECode DatagramSocket::Init(
    /* [in] */ Int32 aport,
    /* [in] */ IInetAddress* addr)
{
    FAIL_RETURN(CheckPort(aport));
    return CreateSocket(aport, addr == NULL ? CInet4Address::ANY.Get() : addr);
}

ECode DatagramSocket::Init(
    /* [in] */ ISocketAddress* localAddr)
{
    if (localAddr != NULL) {
        IInetSocketAddress* netSocketAddr = IInetSocketAddress::Probe(localAddr);
        if (netSocketAddr == NULL) {
            ALOGD("Error in DatagramSocket: Local address not an InetSocketAddress!");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        Int32 port;
        netSocketAddr->GetPort(&port);
        FAIL_RETURN(CheckPort(port));
    }
    if (mFactory != NULL) {
        mFactory->CreateDatagramSocketImpl((IDatagramSocketImpl**)&mImpl);
    }
    else {
       CPlainDatagramSocketImpl::New((IDatagramSocketImpl**)&mImpl);
    }
    mImpl->Create();
    if (localAddr != NULL) {
        // try {
        ECode ec = Bind(localAddr);
        if (FAILED(ec)) {
            Close();
            ALOGD("Error in DatagramSocket: failed to bind socket!");
            return ec;
        }
        // } catch (SocketException e) {
        //    close();
        //    throw e;
        // }
    }
    // SocketOptions.SO_BROADCAST is set by default for DatagramSocket
    return SetBroadcast(TRUE);
}

ECode DatagramSocket::CheckPort(
    /* [in] */ Int32 aPort)
{
    if (aPort < 0 || aPort > 65535) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode DatagramSocket::Close()
{
    mIsClosed = TRUE;
    return mImpl->Close();
}

ECode DatagramSocket::Disconnect()
{
    Boolean isClosed = FALSE;
    Boolean isConnected = FALSE;
    if ((IsClosed(&isClosed), isClosed) || (IsConnected(&isConnected), !isConnected)) {
        return NOERROR;
    }
    mImpl->Disconnect();
    mAddress = NULL;
    mPort = -1;
    mIsConnected = FALSE;
    return NOERROR;
}

ECode DatagramSocket::CreateSocket(
    /* [in] */ Int32 aPort,
    /* [in] */ IInetAddress* addr)
{
    Mutex::Autolock lock(GetSelfLock());

    mImpl = NULL;
    if (mFactory != NULL) {
        mFactory->CreateDatagramSocketImpl((IDatagramSocketImpl**)&mImpl);
    }
    else {
        FAIL_RETURN(CPlainDatagramSocketImpl::New((IDatagramSocketImpl**)&mImpl));
    }
    mImpl->Create();

    // try {
    ECode ec = mImpl->Bind(aPort, addr);
    if (FAILED(ec)) {
        Close();
        ALOGD("Error in DatagramSocket: failed to create socket!");
        return ec;
    }
    mIsBound = TRUE;
    // } catch (SocketException e) {
    //    close();
    //    throw e;
    // }

    return NOERROR;
}

ECode DatagramSocket::GetInetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    *address = mAddress;
    INTERFACE_ADDREF(*address);
    return NOERROR;
}

ECode DatagramSocket::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isClosed = FALSE;
    if (IsClosed(&isClosed), isClosed) {
        *address = NULL;
        return NOERROR;
    }

    Boolean isBound = FALSE;
    if (IsBound(&isBound), !isBound) {
        *address = CInet4Address::ANY;
        INTERFACE_ADDREF(*address);
        return NOERROR;
    }
    AutoPtr<IInetAddress> anAddr;
    mImpl->GetLocalAddress((IInetAddress**)&anAddr);
    *address = anAddr;
    INTERFACE_ADDREF(*address);
    return NOERROR;
}

ECode DatagramSocket::GetLocalPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    Boolean isClosed = FALSE;
    if (IsClosed(&isClosed), isClosed) {
        *port = -1;
        return NOERROR;
    }
    Boolean isBound = FALSE;
    if (IsBound(&isBound), !isBound) {
        *port = 0;
        return NOERROR;
    }
    return mImpl->GetLocalPort(port);
}

ECode DatagramSocket::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    *port = mPort;
    return NOERROR;
}

Boolean DatagramSocket::IsMulticastSocket()
{
    return FALSE;
}

ECode DatagramSocket::GetReceiveBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    Mutex::Autolock lock(GetSelfLock());

    FAIL_RETURN(CheckOpen());
    AutoPtr<IInteger32> optVal;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::SO_RCVBUF, (IInterface**)&optVal));
    return optVal->GetValue(size);
}

ECode DatagramSocket::GetSendBufferSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    Mutex::Autolock lock(GetSelfLock());

    FAIL_RETURN(CheckOpen());
    AutoPtr<IInteger32> optVal;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::SO_SNDBUF, (IInterface**)&optVal));
    return optVal->GetValue(size);
}

ECode DatagramSocket::GetSoTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);

    Mutex::Autolock lock(GetSelfLock());

    FAIL_RETURN(CheckOpen());
    AutoPtr<IInteger32> optVal;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::SO_TIMEOUT, (IInterface**)&optVal));
    return optVal->GetValue(timeout);
}

ECode DatagramSocket::Receive(
    /* [in] */ IDatagramPacket* pack)
{
    Mutex::Autolock lock(GetSelfLock());
    FAIL_RETURN(CheckOpen());
    FAIL_RETURN(EnsureBound());
    if (pack == NULL) {
        ALOGD("Error in DatagramSocket: pack == null!");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (FAILED(mPendingConnectException)) {
        return E_SOCKET_EXCEPTION;
    }
    pack->ResetLengthForReceive();
    return mImpl->Receive(pack);
}

ECode DatagramSocket::Send(
    /* [in] */ IDatagramPacket* pack)
{
    FAIL_RETURN(CheckOpen());
    FAIL_RETURN(EnsureBound());

    AutoPtr<IInetAddress> packAddr;
    pack->GetAddress((IInetAddress**)&packAddr);
    if (mAddress != NULL) { // The socket is connected
        if (packAddr != NULL) {
            Int32 port;
            pack->GetPort(&port);
            Boolean flag = FALSE;
            mAddress->Equals(packAddr, &flag);
            if (!flag || mPort != port) {
                ALOGD("Error in DatagramSocket: Packet address mismatch with connected address!");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
        }
        else {
            pack->SetAddress(mAddress);
            pack->SetPort(mPort);
        }
    }
    else {
        // not connected so the target address is not allowed to be null
        if (packAddr == NULL) {
            return E_NULL_POINTER_EXCEPTION;
        }
    }
    return mImpl->Send(pack);
}

ECode DatagramSocket::SetNetworkInterface(
    /* [in] */ INetworkInterface* netInterface)
{
    if (netInterface == NULL) {
        ALOGD("Error in DatagramSocket: netInterface == null!");
        return E_NULL_POINTER_EXCEPTION;
    }
    //try {
    AutoPtr<IFileDescriptor> outfd;
    mImpl->GetFileDescriptor((IFileDescriptor**)&outfd);
    Int32 fd;
    outfd->GetDescriptor(&fd);
    String netname;
    netInterface->GetName(&netname);
    ECode ec = CLibcore::sOs->SetsockoptIfreq(fd, COsConstants::sSOL_SOCKET, COsConstants::sSO_BINDTODEVICE, netname);
    if (FAILED(ec)) {
        return E_SOCKET_EXCEPTION;
    }

    return NOERROR;
    //} catch (ErrnoException errnoException) {
            //throw errnoException.rethrowAsSocketException();
    //}
}

ECode DatagramSocket::SetSendBufferSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(GetSelfLock());

    if (size < 1) {
        ALOGD("Error in DatagramSocket: send buffer size < 1!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(CheckOpen());
    AutoPtr<IInteger32> optVal;
    CInteger32::New(size, (IInteger32**)&optVal);
    return mImpl->SetOption(ISocketOptions::SO_SNDBUF, optVal);
}

ECode DatagramSocket::SetReceiveBufferSize(
    /* [in] */ Int32 size)
{
    Mutex::Autolock lock(GetSelfLock());

    if (size < 1) {
        ALOGD("Error in DatagramSocket: receive buffer size < 1!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(CheckOpen());
    AutoPtr<IInteger32> optVal;
    CInteger32::New(size, (IInteger32**)&optVal);
    return mImpl->SetOption(ISocketOptions::SO_RCVBUF, optVal);
}

ECode DatagramSocket::SetSoTimeout(
    /* [in] */ Int32 timeout)
{
    Mutex::Autolock lock(GetSelfLock());

    if (timeout < 0) {
        ALOGD("Error in DatagramSocket: timeout < 0!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(CheckOpen());
    AutoPtr<IInteger32> optVal;
    CInteger32::New(timeout, (IInteger32**)&optVal);
    return mImpl->SetOption(ISocketOptions::SO_TIMEOUT, optVal);
}

ECode DatagramSocket::SetDatagramSocketImplFactory(
    /* [in] */ IDatagramSocketImplFactory* fac)
{
    Mutex::Autolock lock(&sLock);
    if (mFactory != NULL) {
        ALOGD("Error in DatagramSocket: Factory already set!");
        return E_SOCKET_EXCEPTION;
    }
    mFactory = fac;
    return NOERROR;
}

ECode DatagramSocket::CheckOpen()
{
    Boolean isClosed = FALSE;
    if (IsClosed(&isClosed), isClosed) {
        ALOGD("Error in DatagramSocket: Socket is closed!");
        return E_SOCKET_EXCEPTION;
    }
    return NOERROR;
}

ECode DatagramSocket::EnsureBound()
{
    Boolean isBound = FALSE;
    if (IsBound(&isBound), !isBound) {
        FAIL_RETURN(mImpl->Bind(0, CInet4Address::ANY.Get()));
        mIsBound = TRUE;
    }
    return NOERROR;
}

ECode DatagramSocket::Bind(
    /* [in] */ ISocketAddress* localAddr)
{
    FAIL_RETURN(CheckOpen());
    Int32 localPort = 0;
    AutoPtr<IInetAddress> addr = CInet4Address::ANY;
    if (localAddr != NULL) {
        IInetSocketAddress* inetAddr = IInetSocketAddress::Probe(localAddr);
        if (inetAddr == NULL) {
            ALOGD("Error in DatagramSocket: Socket is closedLocal address not an InetSocketAddress!");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        addr = NULL;
        inetAddr->GetAddress((IInetAddress**)&addr);
        if (addr == NULL) {
            String hostName;
            inetAddr->GetHostName(&hostName);
            ALOGD("Error in DatagramSocket: Host is unresolved: %s!", hostName.string());
            return E_SOCKET_EXCEPTION;
        }
        inetAddr->GetPort(&localPort);
        FAIL_RETURN(CheckPort(localPort));
    }
    FAIL_RETURN(mImpl->Bind(localPort, addr));
    mIsBound = TRUE;
    return NOERROR;
}

ECode DatagramSocket::Connect(
    /* [in] */ ISocketAddress* peer)
{
    if (peer == NULL) {
        ALOGD("Error in DatagramSocket: remoteAddr == null!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    IInetSocketAddress* isa = IInetSocketAddress::Probe(peer);
    if (isa == NULL) {
        ALOGD("Error in DatagramSocket: Remote address not an InetSocketAddress!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IInetAddress> addr;
    isa->GetAddress((IInetAddress**)&addr);
    if (addr == NULL) {
        String hostName;
        isa->GetHostName(&hostName);
        ALOGD("Error in DatagramSocket: Host is unresolved: %s!", hostName.string());
        return E_SOCKET_EXCEPTION;
    }

    {
        Mutex::Autolock lock(mLock);
        FAIL_RETURN(CheckOpen());
        FAIL_RETURN(EnsureBound());

        mAddress = addr;
        Int32 port;
        isa->GetPort(&port);
        mPort = port;
        mIsConnected = TRUE;

        return mImpl->Connect(addr, port);
    }
}

ECode DatagramSocket::Connect(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int32 port)
{
    if (address == NULL) {
        ALOGD("Error in DatagramSocket: failed to connect, address == null!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // try {
    AutoPtr<IInetSocketAddress> addr;
    CInetSocketAddress::New(address, port, (IInetSocketAddress**)&addr);
    mPendingConnectException = Connect(addr.Get());
    // } catch (SocketException connectException) {
    //     // TODO: or just use SneakyThrow? There's a clear API bug here.
    //     pendingConnectException = connectException;
    // }
    return NOERROR;
}

ECode DatagramSocket::IsBound(
    /* [out] */ Boolean* isBound)
{
    VALIDATE_NOT_NULL(isBound);
    *isBound = mIsBound;
    return NOERROR;
}

ECode DatagramSocket::IsConnected(
    /* [out] */ Boolean* isConnected)
{
    VALIDATE_NOT_NULL(isConnected);
    *isConnected = mIsConnected;
    return NOERROR;
}

ECode DatagramSocket::GetRemoteSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isConnected;
    if (IsConnected(&isConnected), !isConnected) {
        *address = NULL;
        return NOERROR;
    }
    AutoPtr<IInetAddress> addr;
    Int32 port;
    GetInetAddress((IInetAddress**)&addr);
    GetPort(&port);
    return CInetSocketAddress::New(addr, port, (IInetSocketAddress**)address);
}

ECode DatagramSocket::GetLocalSocketAddress(
    /* [out] */ ISocketAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Boolean isBound;
    if (IsBound(&isBound), !isBound) {
        *address = NULL;
        return NOERROR;
    }
    AutoPtr<IInetAddress> addr;
    Int32 port;
    GetLocalAddress((IInetAddress**)&addr);
    GetLocalPort(&port);
    return CInetSocketAddress::New(addr, port, (IInetSocketAddress**)address);
}

ECode DatagramSocket::SetReuseAddress(
    /* [in] */ Boolean reuse)
{
    FAIL_RETURN(CheckOpen());
    AutoPtr<IBoolean> optVal;
    CBoolean::New(reuse, (IBoolean**)&optVal);
    return mImpl->SetOption(ISocketOptions::SO_REUSEADDR, optVal);
}

ECode DatagramSocket::GetReuseAddress(
    /* [out] */ Boolean* reuse)
{
    VALIDATE_NOT_NULL(reuse);

    FAIL_RETURN(CheckOpen());
    AutoPtr<IBoolean> optVal;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::SO_REUSEADDR, (IInterface**)&optVal));
    return optVal->GetValue(reuse);
}

ECode DatagramSocket::SetBroadcast(
    /* [in] */ Boolean broadcast)
{
    FAIL_RETURN(CheckOpen());
    AutoPtr<IBoolean> optVal;
    CBoolean::New(broadcast, (IBoolean**)&optVal);
    return mImpl->SetOption(ISocketOptions::_SO_BROADCAST, optVal);
}

ECode DatagramSocket::GetBroadcast(
    /* [out] */ Boolean* broadcast)
{
    VALIDATE_NOT_NULL(broadcast);

    FAIL_RETURN(CheckOpen());
    AutoPtr<IBoolean> optVal;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::_SO_BROADCAST, (IInterface**)&optVal));
    return optVal->GetValue(broadcast);
}

ECode DatagramSocket::SetTrafficClass(
    /* [in] */ Int32 value)
{
    FAIL_RETURN(CheckOpen());
    if (value < 0 || value > 255) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IInteger32> optVal;
    CInteger32::New(value, (IInteger32**)&optVal);
    return mImpl->SetOption(ISocketOptions::IP_TOS, optVal);
}

ECode DatagramSocket::GetTrafficClass(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    FAIL_RETURN(CheckOpen());
    AutoPtr<IInteger32> optVal;
    FAIL_RETURN(mImpl->GetOption(ISocketOptions::IP_TOS, (IInterface**)&optVal));
    return optVal->GetValue(value);
}

ECode DatagramSocket::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);
    *isClosed = mIsClosed;
    return NOERROR;
}

ECode DatagramSocket::GetChannel(
    /* [out] */ IDatagramChannel** channel)
{
    VALIDATE_NOT_NULL(channel);
    *channel = NULL;
    return NOERROR;
}

ECode DatagramSocket::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fd)
{
    return mImpl->GetFileDescriptor((IFileDescriptor**)&fd);;
}

} // namespace Net
} // namespace Elastos
