
#include "cmdef.h"
#include "ByteBuffer.h"
#include "ReadWriteHeapByteBuffer.h"
#include "ReadWriteDirectByteBuffer.h"
#include "NioUtils.h"
#include "Memory.h"

namespace Elastos {
namespace IO {

// {AAD41A09-77D1-491c-A2A0-0C7DB0B379E6}
extern "C" const InterfaceID EIID_ByteBuffer =
    { 0xaad41a09, 0x77d1, 0x491c, { 0xa2, 0xa0, 0xc, 0x7d, 0xb0, 0xb3, 0x79, 0xe6 } };

ECode ByteBuffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);
    if (capacity < 0) {
        // throw new IllegalArgumentException("capacity < 0: " + capacity);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<ReadWriteHeapByteBuffer> bufObj = new ReadWriteHeapByteBuffer(capacity);
    *buf = bufObj->Probe(EIID_IByteBuffer);
    INTERFACE_ADDREF(*buf);
    return NOERROR;
}

ECode ByteBuffer::AllocateDirect(
    /* [in] */ Int32 capacity,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);
    if (capacity < 0) {
        // throw new IllegalArgumentException("capacity < 0: " + capacity);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<ReadWriteDirectByteBuffer> bufObj = new ReadWriteDirectByteBuffer(capacity);
    *buf = bufObj->Probe(EIID_IByteBuffer);
    INTERFACE_ADDREF(*buf);
    return NOERROR;
}

ECode ByteBuffer::WrapArray(
    /* [in] */ ArrayOf<Byte>* array,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);
    AutoPtr<ReadWriteHeapByteBuffer> bufObj = new ReadWriteHeapByteBuffer(array);
    *buf = bufObj->Probe(EIID_IByteBuffer);
    INTERFACE_ADDREF(*buf);
    return NOERROR;
}

ECode ByteBuffer::WrapArrayEx(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 byteCount,
    /* [out] */ IByteBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);
    assert(array != NULL);

    Int32 arrayLength = array->GetLength();
    if ((start | byteCount) < 0 || start > arrayLength || arrayLength - start < byteCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    AutoPtr<ReadWriteHeapByteBuffer> bufObj = new ReadWriteHeapByteBuffer(array);
    bufObj->mPosition = start;
    bufObj->mLimit = start + byteCount;
    *buf = bufObj->Probe(EIID_IByteBuffer);
    INTERFACE_ADDREF(*buf);
    return NOERROR;
}

ByteBuffer::ByteBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ MemoryBlock* block)
    : Buffer(0, capacity, block)
    , mOrder(ByteOrder_BIG_ENDIAN)
{}

ECode ByteBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return ProtectedArray(array);
}

ECode ByteBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode ByteBuffer::CompareTo(
    /* [in] */ IByteBuffer* other,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    assert(other != NULL);

    Int32 thisRemaining = 0;
    Int32 otherRemaining = 0;
    GetRemaining(&thisRemaining);
    other->GetRemaining(&otherRemaining);
    Int32 compareRemaining = thisRemaining < otherRemaining ?
                       thisRemaining : otherRemaining;
    Int32 thisPos = mPosition;
    Int32 otherPos = 0;
    other->GetPosition(&otherPos);
    Byte thisByte, otherByte;
    while (compareRemaining > 0) {
        this->GetByteEx(thisPos, &thisByte);
        other->GetByteEx(otherPos, &otherByte);
        if (thisByte != otherByte) {
            *result = thisByte < otherByte ? -1 : 1;
            return NOERROR;
        }
        ++thisPos;
        ++otherPos;
        --compareRemaining;
    }
    GetRemaining(&thisRemaining);
    other->GetRemaining(&otherRemaining);
    *result = thisRemaining - otherRemaining;
    return NOERROR;
}

ECode ByteBuffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* isEquals)
{
    ByteBuffer* otherObj = (ByteBuffer*)(other->Probe(EIID_ByteBuffer));
    if (NULL == otherObj) {
        *isEquals = FALSE;
        return NOERROR;
    }

    Int32 remaining = 0;
    Int32 otherRemaining = 0;
    GetRemaining(&remaining);
    otherObj->GetRemaining(&otherRemaining);
    if (remaining != otherRemaining) {
        *isEquals = FALSE;
        return NOERROR;
    }

    Int32 myPosition = mPosition;
    Int32 otherPosition = otherObj->mPosition;
    Boolean equalSoFar = TRUE;
    while (equalSoFar && (myPosition < mLimit)) {
        Byte value, otherValue;
        GetByteEx(myPosition++, &value);
        otherObj->GetByteEx(otherPosition++, &otherValue);
        equalSoFar = (value == otherValue);
    }

    *isEquals = equalSoFar;
    return NOERROR;
}

ECode ByteBuffer::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return GetBytesEx(dst, 0, dst->GetLength());
}

ECode ByteBuffer::GetBytesEx(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    assert(dst != NULL);
    Int32 arrayLength = dst->GetLength();
    if ((dstOffset | byteCount) < 0 || dstOffset > arrayLength || arrayLength - dstOffset < byteCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 srcRemaining = 0;
    GetRemaining(&srcRemaining);
    if (byteCount > srcRemaining) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    for (Int32 i = dstOffset; i < dstOffset + byteCount; ++i) {
        Byte value;
        GetByte(&value);
        (*dst)[i] = value;
    }
    return NOERROR;
}

ECode ByteBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode ByteBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    VALIDATE_NOT_NULL(byteOrder);
    *byteOrder = mOrder;
    return NOERROR;
}

ECode ByteBuffer::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return SetOrderImpl(byteOrder);
}

ECode ByteBuffer::SetOrderImpl(
    /* [in] */ ByteOrder byteOrder)
{
    // if (byteOrder == null) {
    //     byteOrder = ByteOrder.LITTLE_ENDIAN;
    // }
    if (byteOrder != ByteOrder_BIG_ENDIAN && byteOrder != ByteOrder_LITTLE_ENDIAN) {
        byteOrder = ByteOrder_LITTLE_ENDIAN;
    }
    mOrder = byteOrder;
    return NOERROR;
}

ECode ByteBuffer::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return PutBytesEx(src, 0, src.GetLength());
}

ECode ByteBuffer::PutBytesEx(
    /* [in] */ const ArrayOf<Byte>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 byteCount)
{
    Int32 arrayLength = src.GetLength();
    if ((srcOffset | byteCount) < 0 || srcOffset > arrayLength || arrayLength - srcOffset < byteCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (byteCount > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    for (Int32 i = srcOffset; i < srcOffset + byteCount; ++i) {
        PutByte(src[i]);
    }
    return NOERROR;
}

ECode ByteBuffer::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    assert(src != NULL);
    if (src == (IByteBuffer*)(this->Probe(EIID_IByteBuffer))) {
        // throw new IllegalArgumentException("src == this");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 thisByteCount = 0;
    Int32 srcByteCount = 0;
    GetRemaining(&thisByteCount);
    src->GetRemaining(&srcByteCount);
    if (srcByteCount > thisByteCount) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }

    AutoPtr< ArrayOf<Byte> > srcObject;
    Boolean isDirect = FALSE;
    Int32 srcOffset = 0;
    src->GetPosition(&srcOffset);
    src->IsDirect(&isDirect);
    if (isDirect) {
        FAIL_RETURN(src->GetArray((ArrayOf<Byte>**)&srcObject))
    }
    else {
        AutoPtr<HeapByteBuffer> heapBuffer = reinterpret_cast<HeapByteBuffer*>(src->Probe(EIID_HeapByteBuffer));
        srcObject = NioUtils::GetUnsafeArray(heapBuffer);
        srcOffset += NioUtils::GetUnsafeArrayOffset(heapBuffer);
    }

    AutoPtr< ArrayOf<Byte> > dstObject;
    Int32 dstOffset = 0;
    GetPosition(&dstOffset);
    IsDirect(&isDirect);
    if (isDirect) {
        FAIL_RETURN(GetArray((ArrayOf<Byte>**)&dstObject))
    }
    else {
        HeapByteBuffer* heapBuffer = reinterpret_cast<HeapByteBuffer*>(this->Probe(EIID_HeapByteBuffer));
        dstObject = NioUtils::GetUnsafeArray(heapBuffer);
        dstOffset += NioUtils::GetUnsafeArrayOffset(heapBuffer);
    }

    FAIL_RETURN(Memory::Memmove(dstObject, dstOffset, *srcObject, srcOffset, srcByteCount))
    Int32 limit = 0;
    src->GetLimit(&limit);
    src->SetPosition(limit);
    SetPosition(dstOffset + srcByteCount);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
