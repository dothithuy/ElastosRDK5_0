
#include "MapEntry.h"
#include "elastos/ObjectUtils.h"

using Elastos::Core::ObjectUtils;

namespace Elastos {
namespace Utility {

CAR_INTERFACE_IMPL_LIGHT(MapEntry::Type, IInterface)

CAR_INTERFACE_IMPL_LIGHT(MapEntry, IMapEntry)

ECode MapEntry::Clone(
    /* [out] */ IInterface** outface)
{
    // try {
    //     return super.clone();
    // } catch (CloneNotSupportedException e) {
    //     throw new AssertionError(e);
    // }
    return E_ASSERTION_ERROR;
}

ECode MapEntry::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if ((IInterface*)this == object->Probe(EIID_IInterface)) {
        *result = TRUE;
        return NOERROR;
    }
    if (IMapEntry::Probe(object)) {
        AutoPtr<IMapEntry> entry = IMapEntry::Probe(object);
        AutoPtr<IInterface> key, value;
        entry->GetKey((IInterface**)&key);
        entry->GetValue((IInterface**)&value);
        *result = (mKey == NULL ? key == NULL : ObjectUtils::Equals(mKey, key))
                && (mValue == NULL ? value == NULL : ObjectUtils::Equals(mValue, value));
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode MapEntry::GetKey(
    /* [out] */ IInterface** key)
{
    VALIDATE_NOT_NULL(key)

    *key = mKey;
    INTERFACE_ADDREF(*key)
    return NOERROR;
}

ECode MapEntry::GetValue(
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)

    *value = mValue;
    INTERFACE_ADDREF(*value)
    return NOERROR;
}

ECode MapEntry::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)

    *hashCode = (mKey == NULL ? 0 : ObjectUtils::GetHashCode(mKey))
            ^ (mValue == NULL ? 0 : ObjectUtils::GetHashCode(mValue));
    return NOERROR;
}

ECode MapEntry::SetValue(
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** oldObject)
{
    VALIDATE_NOT_NULL(oldObject)

    AutoPtr<IInterface> result = mValue;
    mValue = object;
    *oldObject = result;
    INTERFACE_ADDREF(*oldObject)
    return NOERROR;
}

ECode MapEntry::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = ObjectUtils::ToString(mKey) + String("=") + ObjectUtils::ToString(mValue);
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos