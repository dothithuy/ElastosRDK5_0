#ifndef __READONLYDOUBLEARRAYBUFFER_H__
#define __READONLYDOUBLEARRAYBUFFER_H__

#include "DoubleArrayBuffer.h"

namespace Elastos {
namespace IO {

/**
 * DoubleArrayBuffer, ReadWriteDoubleArrayBuffer and ReadOnlyDoubleArrayBuffer
 * compose the implementation of array based double buffers.
 * <p>
 * ReadOnlyDoubleArrayBuffer extends DoubleArrayBuffer with all the write
 * methods throwing read only exception.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 *
 */
class ReadOnlyDoubleArrayBuffer
    : public IDoubleBuffer
    , public DoubleArrayBuffer
{
public:
    ReadOnlyDoubleArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Double>* backingArray,
        /* [in] */ Int32 arrayOffset);

    static CARAPI_(AutoPtr<ReadOnlyDoubleArrayBuffer>) Copy(
        /* [in] */ DoubleArrayBuffer* other,
        /* [in] */ Int32 markOfOther);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetPrimitiveArray(
        /* [out] */ Handle32* arrayHandle);

    CARAPI GetArray(
        /* [out, callee] */ ArrayOf<Double>** array);

    CARAPI GetElementSizeShift(
        /* [out] */ Int32* elementSizeShift);

    CARAPI GetEffectiveDirectAddress(
        /* [out] */ Int32* effectiveDirectAddress);

    CARAPI GetArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IDoubleBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI GetDouble(
        /* [out] */ Double* value);

    CARAPI GetDoubleEx(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    CARAPI GetDoubles(
        /* [out] */ ArrayOf<Double>* dst);

    CARAPI GetDoublesEx(
        /* [out] */ ArrayOf<Double>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 doubleCount);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutDouble(
        /* [in] */ Double d);

    CARAPI PutDoubleEx(
        /* [in] */ Int32 index,
        /* [in] */ Double d);

    CARAPI PutDoubles(
        /* [in] */ const ArrayOf<Double>& src);

    CARAPI PutDoublesEx(
        /* [in] */ const ArrayOf<Double>& src,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 doubleCount);

    CARAPI PutDoubleBuffer(
        /* [in] */ IDoubleBuffer* src);

    CARAPI Slice(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI GetCapacity(
        /* [out] */ Int32* cap);

    CARAPI Clear();

    CARAPI Flip();

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    CARAPI HasRemaining(
        /* [out] */ Boolean* hasRemaining);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Double>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* result);

    CARAPI GetLimit(
        /* [out] */ Int32* limit);

    CARAPI SetLimit(
        /* [in] */ Int32 newLimit);

    CARAPI Mark();

    CARAPI GetPosition(
        /* [out] */ Int32* pos);

    CARAPI SetPosition(
        /* [in] */ Int32 newPosition);

    CARAPI GetRemaining(
        /* [out] */ Int32* remaining);

    CARAPI Reset();

    CARAPI Rewind();

    CARAPI Equals(
        /* [in] */  IInterface* other,
        /* [out] */ Boolean* rst);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<ArrayOf<Double> > mArrayTemp;

};

} // namespace IO
} // namespace Elastos

#endif // __READONLYDOUBLEARRAYBUFFER_H__
