
#include "cmdef.h"
#include "ByteArrayOutputStream.h"

namespace Elastos {
namespace IO {

ByteArrayOutputStream::ByteArrayOutputStream()
    : mCount(0)
{
}

ByteArrayOutputStream::~ByteArrayOutputStream()
{
}

ECode ByteArrayOutputStream::Init(
    /* [in] */ Int32 size)
{
    if (size < 0) {
//      throw new IllegalArgumentException("size <= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBuf = ArrayOf<Byte>::Alloc(size);
    if (mBuf == NULL)
        return E_OUT_OF_MEMORY_ERROR;

    return NOERROR;
}

ECode ByteArrayOutputStream::Close()
{
    /**
     * Although the spec claims "A closed stream cannot perform output
     * operations and cannot be reopened.", this implementation must do
     * nothing.
     */
    return OutputStream::Close();
}

ECode ByteArrayOutputStream::Expand(
    /* [in] */ Int32 i)
{
    /* Can the buffer handle @i more bytes, if not expand it */
    if (mCount + i <= mBuf->GetLength()){
        return NOERROR;
    }
    AutoPtr<ArrayOf<Byte> > newbuf = ArrayOf<Byte>::Alloc((mCount + i) * 2);
    if (newbuf == NULL)
        return E_OUT_OF_MEMORY_ERROR;

    memcpy(newbuf->GetPayload(), mBuf->GetPayload(), mCount);
    mBuf = newbuf;

    return NOERROR;
}

ECode ByteArrayOutputStream::Reset()
{
    mCount = 0;
    return NOERROR;
}

ECode ByteArrayOutputStream::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)

    *size = mCount;
    return NOERROR;
}

ECode ByteArrayOutputStream::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte>** byteArray)
{
    VALIDATE_NOT_NULL(byteArray)

    AutoPtr< ArrayOf<Byte> > newArray = ArrayOf<Byte>::Alloc(mCount);
    newArray->Copy(mBuf, mCount);
    *byteArray = newArray;
    ARRAYOF_ADDREF(*byteArray);
    return NOERROR;
}

ECode ByteArrayOutputStream::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = String((const char*)mBuf->GetPayload(), mCount);
    return NOERROR;
}

ECode ByteArrayOutputStream::ToStringEx(
    /* [in] */ const String& enc,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = String((const char*)mBuf->GetPayload(), mCount) + enc;
    return NOERROR;
}

ECode ByteArrayOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    if (mCount == mBuf->GetLength()){
        Expand(1);
    }
    (*mBuf)[mCount++] = (Byte)oneByte;

    return NOERROR;
}

ECode ByteArrayOutputStream::WriteBytesEx(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    // avoid int overflow
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // removed redundant check, made implicit null check explicit,
    // used (offset | len) < 0 instead of (offset < 0) || (len < 0)
    // to safe one operation
    if ((offset | count) < 0 || count > buffer.GetLength() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    if (count == 0) {
        return NOERROR;
    }
    /* Expand if necessary */
    Expand(count);
    memcpy(mBuf->GetPayload() + mCount, buffer.GetPayload() + offset, count);
    mCount += count;

    return NOERROR;
}

ECode ByteArrayOutputStream::WriteTo(
    /* [in] */ IOutputStream* out)
{
    return out->WriteBytesEx(*mBuf, 0, mCount);
}

} // namespace IO
} // namespace Elastos
