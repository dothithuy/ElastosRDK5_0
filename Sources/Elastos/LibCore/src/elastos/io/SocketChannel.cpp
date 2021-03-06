#include "cmdef.h"
#include "SocketChannel.h"

namespace Elastos {
namespace IO {
namespace Channels {

SocketChannel::SocketChannel(
    /* [in] */ ISelectorProvider* provider)
    : AbstractSelectableChannel(provider)
{
    assert(NULL != provider);
}

ECode SocketChannel::Open(
    /* [out] */ ISocketChannel** channel)
{
   return E_NOT_IMPLEMENTED;
}

ECode OpenEx(
    /* [in] */ ISocketAddress* addr,
    /* [out] */ ISocketChannel** channel)
{
    ECode ecRet = SocketChannel::Open(channel);
    if(NOERROR != ecRet)
    {
        Boolean bRet;
        // (*channel)->Connect(addr, &bRet);
        assert(0 && "TOOD");
    }
    return ecRet;
}

ECode SocketChannel::ValidOps(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)

    *ret = ISelectionKey::OP_CONNECT | ISelectionKey::OP_READ
            | ISelectionKey::OP_WRITE;
    return NOERROR;
}

ECode SocketChannel::ReadByteBuffersEx(
    /* [in] */ const ArrayOf<IByteBuffer*>& targets,
    /* [out] */ Int64* number)
{
    return ReadByteBuffers(targets, 0, targets.GetLength(), number);
}

ECode SocketChannel::WriteByteBuffersEx(
    /* [in] */ const ArrayOf<IByteBuffer*>& buffers,
    /* [out] */ Int64* number)
{
    return WriteByteBuffers(buffers, 0 , buffers.GetLength(), number);
}

} // namespace Channels
} // namespace IO
} // namespace Elastos

