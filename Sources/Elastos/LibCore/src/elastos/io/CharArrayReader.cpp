
#include "cmdef.h"
#include "CharArrayReader.h"
#include <elastos/Character.h>

using Elastos::Core::Character;

namespace Elastos {
namespace IO {

CharArrayReader::CharArrayReader()
    : mPos(0)
    , mMarkedPos(-1)
    , mCount(0)
{
}

CharArrayReader::~CharArrayReader()
{
}

ECode CharArrayReader::Init(
    /* [in] */ ArrayOf<Char32>* buf)
{
    VALIDATE_NOT_NULL(buf);

    mBuf = buf;
    mCount = mBuf->GetLength();

    return NOERROR;
}

ECode CharArrayReader::Init(
    /* [in] */ ArrayOf<Char32>* buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    VALIDATE_NOT_NULL(buf);

    /*
     * The spec of this constructor is broken. In defining the legal values
     * of offset and length, it doesn't consider buffer's length. And to be
     * compatible with the broken spec, we must also test whether
     * (offset + length) overflows.
     */
    if (offset < 0 || offset > buf->GetLength() || length < 0 || offset + length < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mBuf = buf;
    mPos = offset;
    mMarkedPos = offset;

    /* This is according to spec */
    Int32 bufferLength = mBuf->GetLength();

    //??? android code
    //this.count = offset + length < bufferLength ? length : bufferLength;

    mCount = offset + length < bufferLength ? offset + length : bufferLength;

    return NOERROR;
}

ECode CharArrayReader::Close()
{
    Object::Autolock lock(mLock);

    if (IsOpen()) {
        mBuf = NULL;
    }
    return NOERROR;
}

Boolean CharArrayReader::IsOpen()
{
    return mBuf != NULL;
}

Boolean CharArrayReader::IsClosed()
{
    return mBuf == NULL;
}

ECode CharArrayReader::Mark(
    /* [in] */ Int32 readLimit)
{
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    mMarkedPos = mPos;
    return NOERROR;
}

ECode CharArrayReader::CheckNotClosed()
{
    if (IsClosed()) {
//      throw new IOException("CharArrayReader is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CharArrayReader::IsMarkSupported(
    /*[out]*/ Boolean* IsMarkSupported)
{
    assert(IsMarkSupported != NULL);

    *IsMarkSupported = TRUE;
    return NOERROR;
}

ECode CharArrayReader::Read(
    /*[out]*/ Int32* character)
{
    assert(character != NULL);
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    if (mPos == mCount) {
        *character = -1;
        return NOERROR;
    }
    *character = (*mBuf)[mPos++];

    return NOERROR;
}

ECode CharArrayReader::ReadCharsEx(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    assert(buffer != NULL);

    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    if (offset < 0 || offset > buffer->GetLength()) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer->GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + len);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    if (mPos < mCount) {
        Int32 bytesRead = mPos + length > mCount ? mCount - mPos : length;
        buffer->Copy(offset, mBuf, mPos, bytesRead);
        mPos += bytesRead;
        *number = bytesRead;

        return NOERROR;
    }
    *number = -1;
    return NOERROR;
}

ECode CharArrayReader::IsReady(
    /*[out]*/ Boolean* isReady)
{
    assert(isReady != NULL);
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    *isReady = mPos != mCount;

    return NOERROR;
}

ECode CharArrayReader::Reset()
{
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    mPos = mMarkedPos != -1 ? mMarkedPos : 0;

    return NOERROR;
}

ECode CharArrayReader::Skip(
    /* [in] */Int64 n,
    /* [out] */ Int64* number)
{
    assert(number != NULL);
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    if (n <= 0) {
        *number = 0;
        return NOERROR;
    }

    if (n < mCount - mPos) {
        mPos = mPos + (Int32)n;
        *number = n;
    }
    else {
        *number = mCount - mPos;
        mPos = mCount;
    }

    return NOERROR;
}

} // namespace IO
} // namespace Elastos
