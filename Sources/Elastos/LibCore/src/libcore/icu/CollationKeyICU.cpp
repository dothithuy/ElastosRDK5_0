
#include "cmdef.h"
#include "CollationKeyICU.h"
#include <elastos/Math.h>

using Elastos::Text::EIID_ICollationKey;

extern "C" const InterfaceID EIID_CollationKeyICU =
    { 0x278e167c, 0xaae5, 0x4e71, { 0x9f, 0x92, 0x21, 0x83, 0x7a, 0x76, 0x0, 0x86 } };

namespace Libcore {
namespace ICU {

ECode CollationKeyICU::Init(
    /* [in] */ const String& source,
    /* [in] */ const ArrayOf<Byte>& bytes)
{
    CollationKey::Init(source);
    mBytes = bytes.Clone();
    mHashCode = 0;
    return NOERROR;
}

CollationKeyICU::~CollationKeyICU()
{
}

ECode CollationKeyICU::CompareTo(
    /* [in] */ IInterface* _other,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ICollationKey> other = ICollationKey::Probe(_other);
    if (other == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    // Get the bytes from the other collation key.
    AutoPtr<ArrayOf<Byte> > rhsBytes;
    if (other->Probe(EIID_CollationKeyICU) != NULL) {
        CollationKeyICU* c = reinterpret_cast<CollationKeyICU*>(other->Probe(EIID_CollationKeyICU));
        rhsBytes = c->mBytes;
    }
    else {
        other->ToByteArray((ArrayOf<Byte>**)&rhsBytes);
    }

    if (mBytes == NULL || mBytes->GetLength() == 0) {
        if (rhsBytes == NULL || rhsBytes->GetLength() == 0) {
            *result = 0;
            return NOERROR;
        }
        *result = -1;
        return NOERROR;
    }
    else {
        if (rhsBytes == NULL || rhsBytes->GetLength() == 0) {
            *result = 1;
            return NOERROR;
        }
    }

    Int32 count = Elastos::Core::Math::Min(mBytes->GetLength(), rhsBytes->GetLength());
    for (Int32 i = 0; i < count; ++i) {
        Int32 s = (*mBytes)[i] & 0xFF;
        Int32 t = (*rhsBytes)[i] & 0xFF;
        if (s < t) {
            *result = -1;
            return NOERROR;
        }
        if (s > t) {
            *result = 1;
            return NOERROR;
        }
    }
    if (mBytes->GetLength() < rhsBytes->GetLength()) {
        *result = -1;
        return NOERROR;
    }
    if (mBytes->GetLength() > rhsBytes->GetLength()) {
        *result = 1;
        return NOERROR;
    }
    *result = 0;
    return NOERROR;
}

ECode CollationKeyICU::GetSourceString(
    /* [out] */ String* sourceString)
{
    VALIDATE_NOT_NULL(sourceString);
    return CollationKey::GetSourceString(sourceString);
}

ECode CollationKeyICU::Equals(
    /* [in] */ IInterface * object,
    /* [out] */ Boolean * value)
{
    if (object == this->Probe(EIID_IInterface)) {
        *value = TRUE;
        return NOERROR;
    }
    AutoPtr<ICollationKey> res = (ICollationKey *) object->Probe(EIID_ICollationKey);
    if (!res) {
        *value = FALSE;
        return NOERROR;
    }
    Int32 out(0);
    CompareTo(res,&out);
    *value = out == 0;
    return NOERROR;
}

ECode CollationKeyICU::GetHashCode(
    /* [out] */ Int32 * value)
{
    if (mHashCode == 0) {
        if (mBytes != NULL && mBytes->GetLength() != 0) {
            Int32 len = mBytes->GetLength();
            Int32 inc = ((len - 32) / 32) + 1;
            for (Int32 i = 0; i < len;) {
                mHashCode = (mHashCode * 37) + (*mBytes)[i];
                i += inc;
            }
        }
        if (mHashCode == 0) {
            mHashCode = 1;
        }
    }
    *value = mHashCode;
    return NOERROR;
}

ECode CollationKeyICU::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    VALIDATE_NOT_NULL(array);
    if (mBytes != NULL && mBytes->GetLength() == 0) {
        *array = NULL;
        return NOERROR;
    }
    *array = mBytes->Clone();
    INTERFACE_ADDREF(*array);
    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
