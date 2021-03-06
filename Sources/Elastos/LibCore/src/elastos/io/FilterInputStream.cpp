
#include "FilterInputStream.h"

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(FilterInputStream, IFilterInputStream)

FilterInputStream::FilterInputStream()
{
}

FilterInputStream::~FilterInputStream()
{
}

ECode FilterInputStream::constructor(
    /* [in] */ IInputStream* in)
{
    mIn = in;
    return NOERROR;
}

ECode FilterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return mIn->Available(number);
}

ECode FilterInputStream::Close()
{
    return mIn->Close();
}

ECode FilterInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    Object::Autolock lock(mLock);
    return mIn->Mark(readLimit);
}

ECode FilterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return mIn->IsMarkSupported(supported);
}

ECode FilterInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return mIn->Read(value);
}

ECode FilterInputStream::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);
    return mIn->Read(buffer, byteOffset, byteCount, number);
}

ECode FilterInputStream::Reset()
{
    Object::Autolock lock(mLock);
    return mIn->Reset();
}

ECode FilterInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(value);
    return mIn->Skip(byteCount, number);
}

} // namespace IO
} // namespace Elastos
