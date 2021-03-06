
#include "cmdef.h"
#include "OutputStreamWriter.h"
#include "ByteBuffer.h"
#include "CharBuffer.h"
#include "CStringWrapper.h"

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace IO {

OutputStreamWriter::OutputStreamWriter()
{
    ASSERT_SUCCEEDED(ByteBuffer::Allocate(8192, (IByteBuffer**)&mBytes));
}

ECode OutputStreamWriter::Close()
{
    Object::Autolock lock(mLock);
    // if (mEncoder != NULL) {
    //     mEncoder->Flush(mBuf);
    if (mOut != NULL) {
         Flush();
         IFlushable::Probe(mOut)->Flush();
         mOut->Close();
    //     mEncoder = NULL;
         mBytes = NULL;
    }

    return NOERROR;
}

ECode OutputStreamWriter::Flush()
{
    return FlushBytes(TRUE);
}

ECode OutputStreamWriter::FlushBytes(
    /* [in] */ Boolean flushUnderlyingStream)
{
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckStatus());
    Int32 position;
    mBytes->GetPosition(&position);
    if (position > 0) {
        mBytes->Flip();
        AutoPtr<ArrayOf<Byte> > buf;
        mBytes->GetArray((ArrayOf<Byte>**)&buf);
        Int32 offset;
        mBytes->GetArrayOffset(&offset);
        mOut->WriteBytesEx(*buf, offset, position);
        mBytes->Clear();
    }

    if (flushUnderlyingStream) {
        FAIL_RETURN(IFlushable::Probe(mOut)->Flush());
    }

    return NOERROR;
}

ECode OutputStreamWriter::Convert(
    /* [in] */ ICharBuffer* chars)
{
    while (TRUE) {
//        CoderResult result = encoder.encode(chars, bytes, false);
        // TODO:
        Int32 count;
        chars->GetLength(&count);
        for(Int32 i = 0; i < count; ++i) {
            Char32 ch;
            chars->GetChar(&ch);
            mBytes->PutByte((Byte)ch);
        }
//        if (result.isOverflow()) {
//            // Make room and try again.
        FlushBytes(FALSE);
//        continue;
//        } else if (result.isError()) {
//            result.throwException();
//        }
        break;
    }
    return NOERROR;
}

ECode OutputStreamWriter::DrainEncoder()
{
    assert(0);
    // // Strictly speaking, I think it's part of the CharsetEncoder contract that you call
    // // encode with endOfInput true before flushing. Our ICU-based implementations don't
    // // actually need this, and you'd hope that any reasonable implementation wouldn't either.
    // // CharsetEncoder.encode doesn't actually pass the boolean through to encodeLoop anyway!
    // CharBuffer chars = CharBuffer.allocate(0);
    // while (true) {
    //     CoderResult result = encoder.encode(chars, bytes, true);
    //     if (result.isError()) {
    //         result.throwException();
    //     } else if (result.isOverflow()) {
    //         flushBytes(false);
    //         continue;
    //     }
    //     break;
    // }

    // // Some encoders (such as ISO-2022-JP) have stuff to write out after all the
    // // characters (such as shifting back into a default state). In our implementation,
    // // this is actually the first time ICU is told that we've run out of input.
    // CoderResult result = encoder.flush(bytes);
    // while (!result.isUnderflow()) {
    //     if (result.isOverflow()) {
    //         flushBytes(false);
    //         result = encoder.flush(bytes);
    //     } else {
    //         result.throwException();
    //     }
    // }
    return E_NOT_IMPLEMENTED;
}

ECode OutputStreamWriter::CheckStatus()
{
//     if (mEncoder == NULL) {
// //        throw new IOException("OutputStreamWriter is closed");
//     }
    return NOERROR;
}

ECode OutputStreamWriter::GetEncoding(
    /* [out] */ String *encoding)
{
    VALIDATE_NOT_NULL(encoding);

    *encoding = String("UTF-8");
    // if (mEncoder == NULL) {
    //     *encoding = String(NULL);
    //     return NOERROR;
    // }
    // return HistoricalNamesUtil.getHistoricalName(encoder.charset().name());
    return NOERROR;
}

ECode OutputStreamWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckStatus());
    AutoPtr<ArrayOf<Char32> > c = ArrayOf<Char32>::Alloc(1);
    (*c)[0] = (Char32)oneChar32;
    AutoPtr<ICharBuffer> chars;
    CharBuffer::WrapArray(c, (ICharBuffer**)&chars);
    return Convert(chars);
}

ECode OutputStreamWriter::WriteCharsEx(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckStatus());
    Int32 arrayLength = buffer.GetLength();
    if ((offset | count) < 0 || offset > arrayLength || arrayLength - offset < count) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoPtr<ICharBuffer> chars;
    CharBuffer::WrapArrayEx(const_cast<ArrayOf<Char32>*>(&buffer), offset, count, (ICharBuffer**)&chars);
    return Convert(chars);
}

ECode OutputStreamWriter::WriteStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    Object::Autolock lock(mLock);

    if (count < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new StringIndexOutOfBoundsException();
    }
    if (str.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("str == null");
    }
    if ((offset | count) < 0 || offset > str.GetLength() - count) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new StringIndexOutOfBoundsException();
    }
    FAIL_RETURN(CheckStatus());
    AutoPtr<ICharSequence> charSeq;
    CStringWrapper::New(String(str), (ICharSequence**)&charSeq);
    AutoPtr<ICharBuffer> chars;
    CharBuffer::WrapSequenceEx(charSeq, offset, count + offset, (ICharBuffer**)&chars);
    return Convert(chars);
}

ECode OutputStreamWriter::Init(
    /* [in] */ IOutputStream *out)
{
    return Init(out, String(NULL)/*Charset.defaultCharset()*/);
}

ECode OutputStreamWriter::Init(
    /* [in] */ IOutputStream *out,
    /* [in] */ const String &enc)
{
    AutoPtr<IInterface> obj;
    out->GetLock((IInterface**)&obj);
    LockObject* lockObj = (LockObject*)obj.Get();
    Writer::Init(lockObj);

    // if (charsetName == null) {
    //     throw new NullPointerException("charsetName == null");
    // }
    mOut = out;
    // try {
    //     encoder = Charset.forName(charsetName).newEncoder();
    // } catch (Exception e) {
    //     throw new UnsupportedEncodingException(charsetName);
    // }
    // encoder.onMalformedInput(CodingErrorAction.REPLACE);
    // encoder.onUnmappableCharacter(CodingErrorAction.REPLACE);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
