
#include "cmdef.h"
#include "FloatToByteBufferAdapter.h"
#include "ByteBuffer.h"
#include "DirectByteBuffer.h"
#include "HeapByteBuffer.h"
#include "ReadWriteDirectByteBuffer.h"
#include "ReadWriteHeapByteBuffer.h"
#include "elastos/StringBuilder.h"
#include "cutils/log.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

FloatToByteBufferAdapter::FloatToByteBufferAdapter(
    /* [in] */ IByteBuffer* byteBuffer)
    : FloatBuffer(((ByteBuffer*)byteBuffer->Probe(EIID_ByteBuffer))->mCapacity / sizeof(Float))
    , mByteBuffer(byteBuffer)
{
    mByteBuffer->Clear();
    mEffectiveDirectAddress = (reinterpret_cast<ByteBuffer*>(byteBuffer->Probe(EIID_ByteBuffer)))->mEffectiveDirectAddress;
}

ECode FloatToByteBufferAdapter::AsFloatBuffer(
    /* [in] */ IByteBuffer* byteBuffer,
    /* [out] */ IFloatBuffer** floatBuffer)
{
    VALIDATE_NOT_NULL(floatBuffer)
    assert(byteBuffer != NULL);
    AutoPtr<IByteBuffer> slice;
    FAIL_RETURN(byteBuffer->Slice((IByteBuffer**)&slice))
    slice->SetOrder((reinterpret_cast<ByteBuffer*>(byteBuffer->Probe(EIID_ByteBuffer)))->mOrder);
    *floatBuffer = (IFloatBuffer*) new FloatToByteBufferAdapter(slice);
    INTERFACE_ADDREF(*floatBuffer);
    return NOERROR;
}

PInterface FloatToByteBufferAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IFloatBuffer) {
        return (IFloatBuffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_FloatBuffer) {
        return reinterpret_cast<PInterface>((FloatBuffer*)this);
    }

    return NULL;
}


UInt32 FloatToByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FloatToByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode FloatToByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IFloatBuffer*)this) {
        *pIID = EIID_IFloatBuffer;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetPrimitiveArray(
    /* [out] */ Handle32* arrayHandle)
{
    mArrayTemp = NULL;
    GetArray((ArrayOf<Float>**)&mArrayTemp);
    if (mArrayTemp == NULL)
    {
        return mByteBuffer->GetPrimitiveArray(arrayHandle);
        *arrayHandle = 0;
        return NOERROR;
    }
    Float* primitiveArray = mArrayTemp->GetPayload();
    *arrayHandle = reinterpret_cast<Handle32>(primitiveArray);
    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return FloatBuffer::GetArray(array);
}

ECode FloatToByteBufferAdapter::GetElementSizeShift(
    /* [out] */ Int32* elementSizeShift)
{
    return Buffer::GetElementSizeShift(elementSizeShift);
}

ECode FloatToByteBufferAdapter::GetEffectiveDirectAddress(
    /* [out] */ Int32* effectiveDirectAddress)
{
    return Buffer::GetEffectiveDirectAddress(effectiveDirectAddress);
}

ECode FloatToByteBufferAdapter::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return FloatBuffer::GetArrayOffset(offset);
}

ECode FloatToByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> byteBuf;
    FAIL_RETURN(mByteBuffer->AsReadOnlyBuffer((IByteBuffer**)&byteBuf))
    AutoPtr<FloatToByteBufferAdapter> buf = new FloatToByteBufferAdapter(byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    buf->mByteBuffer->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode FloatToByteBufferAdapter::Compact()
{
    Boolean isReadOnly = FALSE;
    mByteBuffer->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    mByteBuffer->SetLimit(mLimit * sizeof(Float));
    mByteBuffer->SetPosition(mPosition * sizeof(Float));
    mByteBuffer->Compact();
    mByteBuffer->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = IBuffer::UNSET_MARK;
    return NOERROR;
}

ECode FloatToByteBufferAdapter::CompareTo(
    /* [in] */ IFloatBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return FloatBuffer::CompareTo(otherBuffer, result);
}

ECode FloatToByteBufferAdapter::Duplicate(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Duplicate((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    AutoPtr<FloatToByteBufferAdapter> buf = new FloatToByteBufferAdapter(bb);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    *buffer = (IFloatBuffer*)buf->Probe(EIID_IFloatBuffer);
    INTERFACE_ADDREF(*buffer)
    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetFloat(
    /* [out] */ Float* value)
{
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    return mByteBuffer->GetFloatEx(mPosition++ * sizeof(Float), value);
}

ECode FloatToByteBufferAdapter::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->GetFloatEx(index * sizeof(Float), value);
}

ECode FloatToByteBufferAdapter::GetFloats(
    /* [out] */ ArrayOf<Float>* dst)
{
    return FloatBuffer::GetFloats(dst);
}

ECode FloatToByteBufferAdapter::GetFloatsEx(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Float));
    mByteBuffer->SetPosition(mPosition * sizeof(Float));
    if (mByteBuffer->Probe(EIID_DirectByteBuffer) != NULL) {
        FAIL_RETURN(((DirectByteBuffer*)mByteBuffer->Probe(EIID_DirectByteBuffer))->GetFloatsEx(dst, dstOffset, floatCount))
    }
    else {
        FAIL_RETURN(((HeapByteBuffer*)mByteBuffer->Probe(EIID_HeapByteBuffer))->GetFloatsEx(dst, dstOffset, floatCount))
    }
    mPosition += floatCount;
    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return mByteBuffer->GetOrder(order);
}

ECode FloatToByteBufferAdapter::PutFloat(
    /* [in] */ Float c)
{
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    return mByteBuffer->PutFloatEx(mPosition++ * sizeof(Float), c);
}

ECode FloatToByteBufferAdapter::PutFloatEx(
    /* [in] */ Int32 index,
    /* [in] */ Float c)
{
    FAIL_RETURN(CheckIndex(index))
    return mByteBuffer->PutFloatEx(index * sizeof(Float), c);
}

ECode FloatToByteBufferAdapter::PutFloats(
    /* [in] */ const ArrayOf<Float>& src)
{
    return FloatBuffer::PutFloats(src);
}

ECode FloatToByteBufferAdapter::PutFloatsEx(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 floatCount)
{
    mByteBuffer->SetLimit(mLimit * sizeof(Float));
    mByteBuffer->SetPosition(mPosition * sizeof(Float));
    if (mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer) != NULL ) {
        ReadWriteDirectByteBuffer* tmp = reinterpret_cast<ReadWriteDirectByteBuffer*>(
            mByteBuffer->Probe(EIID_ReadWriteDirectByteBuffer));
        FAIL_RETURN(tmp->PutFloats(src, srcOffset, floatCount))
    }
    else {
        FAIL_RETURN(((ReadWriteHeapByteBuffer*)mByteBuffer.Get())->PutFloats(src, srcOffset, floatCount))
    }
    mPosition += floatCount;
    return NOERROR;
}

ECode FloatToByteBufferAdapter::PutFloatBuffer(
    /* [in] */ IFloatBuffer* src)
{
    return FloatBuffer::PutFloatBuffer(src);
}

ECode FloatToByteBufferAdapter::Slice(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)
    mByteBuffer->SetLimit(mLimit * sizeof(Float));
    mByteBuffer->SetPosition(mPosition * sizeof(Float));
    AutoPtr<IByteBuffer> bb;
    FAIL_RETURN(mByteBuffer->Slice((IByteBuffer**)&bb))
    bb->SetOrder(((ByteBuffer*)mByteBuffer->Probe(EIID_ByteBuffer))->mOrder);
    *buffer = (IFloatBuffer*) new FloatToByteBufferAdapter(bb);
    INTERFACE_ADDREF(*buffer)
    mByteBuffer->Clear();
    return NOERROR;
}

ECode FloatToByteBufferAdapter::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    StringBuilder buf;
    buf += "FloatToByteBufferAdapter";
    buf += ", status: capacity=";
    buf += mCapacity;
    buf += " position=";
    buf += mPosition;
    buf += " limit=";
    buf += mLimit;
    return buf.ToString(str);
}

ECode FloatToByteBufferAdapter::GetCapacity(
    /* [out] */ Int32* cap)
{
    return FloatBuffer::GetCapacity(cap);
}

ECode FloatToByteBufferAdapter::Clear()
{
    return FloatBuffer::Clear();
}

ECode FloatToByteBufferAdapter::Flip()
{
    return FloatBuffer::Flip();
}

ECode FloatToByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return FloatBuffer::HasArray(hasArray);
}

ECode FloatToByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return FloatBuffer::HasRemaining(hasRemaining);
}

ECode FloatToByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return mByteBuffer->IsDirect(isDirect);
}

ECode FloatToByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mByteBuffer->IsReadOnly(isReadOnly);
}

CARAPI FloatToByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

CARAPI FloatToByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

CARAPI FloatToByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode FloatToByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return FloatBuffer::GetLimit(limit);
}

ECode FloatToByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return FloatBuffer::SetLimit(newLimit);
}

ECode FloatToByteBufferAdapter::Mark()
{
    return FloatBuffer::Mark();
}

ECode FloatToByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return FloatBuffer::GetPosition(pos);
}

ECode FloatToByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return FloatBuffer::SetPosition(newPosition);
}

ECode FloatToByteBufferAdapter::GetRemaining(
    /* [out] */ Int32* remaining)
{
    return FloatBuffer::GetRemaining(remaining);
}

ECode FloatToByteBufferAdapter::Reset()
{
    return FloatBuffer::Reset();
}

ECode FloatToByteBufferAdapter::Rewind()
{
    return FloatBuffer::Rewind();
}

} // namespace IO
} // namespace Elastos
