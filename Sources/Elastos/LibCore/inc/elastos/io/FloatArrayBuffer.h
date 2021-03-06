#ifndef __FLOATARRAYBUFFER_H__
#define __FLOATARRAYBUFFER_H__

#include "FloatBuffer.h"

namespace Elastos {
namespace IO {
/**
 * FloatArrayBuffer, ReadWriteFloatArrayBuffer and ReadOnlyFloatArrayBuffer
 * compose the implementation of array based float buffers.
 * <p>
 * FloatArrayBuffer implements all the shared readonly methods and is extended
 * by the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class FloatArrayBuffer
    : public FloatBuffer
{
public:
    FloatArrayBuffer(
        /* [in] */ ArrayOf<Float>* array);

    FloatArrayBuffer(
        /* [in] */ Int32 capacity);

    FloatArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Float>* backingArray,
        /* [in] */ Int32 offset);

    CARAPI GetFloat(
        /* [out] */ Float* value);

    CARAPI GetFloatEx(
        /* [in] */ Int32 index,
        /* [out] */ Float* value);

    CARAPI GetFloatsEx(
        /* [out] */ ArrayOf<Float>* dst,
        /* [in] */ Int32 dstOffset,
        /* [in] */ Int32 floatCount);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

public:
    AutoPtr< ArrayOf<Float> > mBackingArray;

    Int32 mOffset;
};
} // namespace IO
} // namespace Elastos
#endif // __FLOATARRAYBUFFER_H__
