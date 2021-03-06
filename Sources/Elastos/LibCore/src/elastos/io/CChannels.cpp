
#include "CChannels.h"
#include "Channels.h"
#include "cmdef.h"
#include "ByteBuffer.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "CStreams.h"
#include "elastos/Math.h"

using Elastos::IO::InputStream;
using Elastos::IO::OutputStream;

namespace Elastos {
namespace IO {
namespace Channels{

class IChannelInputStream;
class IChannelOutputStream;

CChannels::CChannels() {}

CChannels::~CChannels() {}

ECode CChannels::constructor()
{
    return NOERROR;
}

ECode CChannels::NewChannel(
    /* [in] */ IInputStream* inputStream,
    /* [out] */ IReadableByteChannel** inputChannel)
{
    //*inputChannel = Channels::NewChannel(inputStream);
    return NOERROR;
}

ECode CChannels::NewChannelEx(
    /* [in] */ IOutputStream* outputStream,
    /* [out] */ IWritableByteChannel** outputChannel)
{
    //*outputChannel = Channels::NewChannel(outputStream);
    return NOERROR;
}

ECode CChannels::NewInputStream(
    /* [in] */ IReadableByteChannel * pChannel,
    /* [out] */ IInputStream ** ppInputStream)
{
    return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewOutputStream(
    /* [in] */ IWritableByteChannel * pChannel,
    /* [out] */ IOutputStream ** ppOutputStream)
{
    return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewReader(
    /* [in] */ IReadableByteChannel * pChannel,
    /* [in] */ ICharsetDecoder * pDecoder,
    /* [in] */ Int32 minBufferCapacity,
    /* [out] */ IReader ** ppReader)
{
    return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewReaderEx(
    /* [in] */ IReadableByteChannel * pChannel,
    /* [in] */ const String& charsetName,
    /* [out] */ IReader ** ppReader)
{
    return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewWriter(
    /* [in] */ IWritableByteChannel * pChannel,
    /* [in] */ ICharsetEncoder * pEncoder,
    /* [in] */ Int32 minBufferCapacity,
    /* [out] */ IWriter ** ppWriter)
{
//    IChannelOutputStream *channelOutputStream = new IChannelOutputStream(pChannel);
    return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewWriterEx(
    /* [in] */ IWritableByteChannel * pChannel,
    /* [in] */ const String& charsetName,
    /* [out] */ IWriter ** ppWriter)
{
    return E_NOT_IMPLEMENTED;
}

static ECode CheckBlocking(IChannel *channel)
{
    if(ISelectableChannel::Probe(channel) != NULL)
    {
        Boolean ret;
        ((ISelectableChannel*)channel)->IsBlocking(&ret);
        if(ret)
        {
            return E_ILLEGAL_BLOCKING_MODE_EXCEPTION;
        }
    }
    return NOERROR;
}

class IChannelInputStream : public IInputStream
{
private:
     IReadableByteChannel *mChannel;

public:
    IChannelInputStream(IReadableByteChannel *channel)
    {
        assert(channel != NULL);
        mChannel = channel;
    }

    ECode Read()
    {
        Int32 nRead;
        AutoPtr<IStreams> stream;
        CStreams::AcquireSingleton((IStreams**)&stream);
        stream->ReadSingleByte(this, &nRead);
        return nRead;
    }

    ECode ReadEx(ArrayOf<Byte> *target, Int32 offset, Int32 length)
    {
        IByteBuffer *buffer;
        ByteBuffer::WrapArrayEx(target, offset, length, &buffer);
        CheckBlocking(mChannel);
        Int32 nRead;
        mChannel->ReadByteBuffer(buffer, &nRead);
        return nRead;
    }

    ECode Available()
    {
        if(IFileChannel::Probe(mChannel) != NULL)
        {
            /* !!! NOT SURE */
            IFileChannel *fileChannel = (IFileChannel*)mChannel;

            Int64 result, size, position;
            fileChannel->GetSize(&size);
            fileChannel->GetPosition(&position);
            result = size - position;
            return result > Elastos::Core::Math::INT32_MAX_VALUE ? Elastos::Core::Math::INT32_MAX_VALUE : (Int32)result;
        }
        else
        {
            Int32 ret;
            IInputStream::Available(&ret);
            return ret;
        }
    }

    ECode Close()
    {
        mChannel->Close();
        return NOERROR;
    }

};

class IChannelOutputStream : public IOutputStream
{
private:
        IWritableByteChannel *mChannel;
        Mutex mLock;

public:
        IChannelOutputStream(IWritableByteChannel *channel)
        {
            assert(channel != NULL);
            INTERFACE_ADDREF(channel);
            mChannel = channel;
        }

        virtual ~IChannelOutputStream()
        {
            if (NULL != mChannel)
            {
                mChannel->Release();
            }
        }

        ECode Write(Int32 oneByte)
        {
            Int32 value;
            mLock.Lock();
            value = oneByte;
            Write(value);
            mLock.Unlock();
            return NOERROR;
        }

        ECode WriteEx(ArrayOf<Byte>* source, Int32 offset, Int32 length)
        {
            AutoPtr<IByteBuffer> buffer;
            ByteBuffer::WrapArrayEx(source, offset, length, (IByteBuffer **)&buffer);
            CheckBlocking(mChannel);
            Int32 total = 0;
            Int32 nWrite = 0;
            while(total < length)
            {
               mChannel->WriteByteBuffer(buffer, &nWrite);
               total += nWrite;
            }
            return NOERROR;
        }

        ECode Close()
        {
            mChannel->Close();
            return NOERROR;
        }
};

} // namespace Channels
} // namespace IO
} // namespace Elastos
