
#include "CCoderResult.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace IO {
namespace Charset {

AutoPtr<ICoderResult> CCoderResult::CreateCoderResult(
    /* [in] */ Int32 type,
    /* [in] */ Int32 length)
{
    AutoPtr<CCoderResult> p;
    ASSERT_SUCCEEDED(CCoderResult::NewByFriend((CCoderResult**)&p));
    p->mType = type;
    p->mLength = length;

    return (ICoderResult*)p.Get();
}

const AutoPtr<ICoderResult> CCoderResult::UNDERFLOW = CreateCoderResult(TYPE_UNDERFLOW, 0);
const AutoPtr<ICoderResult> CCoderResult::OVERFLOW = CreateCoderResult(TYPE_OVERFLOW, 0);

Mutex CCoderResult::sMalformedMutex;
Mutex CCoderResult::sUnmappableMutex;

AutoPtr< HashMap<Int32, AutoPtr<ICoderResult> > > CCoderResult::sMalformedErrors = new HashMap<Int32, AutoPtr<ICoderResult> >();
AutoPtr< HashMap<Int32, AutoPtr<ICoderResult> > > CCoderResult::sUnmappableErrors = new HashMap<Int32, AutoPtr<ICoderResult> >();

ECode CCoderResult::constructor()
{
    return NOERROR;
}

ECode CCoderResult::GetUNDERFLOW(
    /* [out] */ ICoderResult** result)
{
    VALIDATE_NOT_NULL(result);
    *result = UNDERFLOW.Get();
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CCoderResult::GetOVERFLOW(
    /* [out] */ ICoderResult** result)
{
    VALIDATE_NOT_NULL(result);
    *result = OVERFLOW.Get();
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CCoderResult::MalformedForLength(
    /* [in] */ Int32 length,
    /* [out] */ ICoderResult** result)
{
    VALIDATE_NOT_NULL(result);

    if (length > 0) {
    // TODO:
    //     Integer key = Integer.valueOf(length);
    //     synchronized (_malformedErrors) {
    //         CoderResult r = _malformedErrors.get(key);
    //         if (null == r) {
    //             r = new CoderResult(TYPE_MALFORMED_INPUT, length);
    //             _malformedErrors.put(key, r);
    //         }
    //         return r;
    //     }
        // Temporary achieve
        Mutex::Autolock lock(sMalformedMutex);
        AutoPtr<ICoderResult> r;
        HashMap<Int32, AutoPtr<ICoderResult> >::Iterator iter = sMalformedErrors->Find(length);
        if (iter == sMalformedErrors->End()) {
            r = CreateCoderResult(TYPE_MALFORMED_INPUT, length);
            (*sMalformedErrors)[length] = r;
            *result = r;
        }
        else {
            *result = iter->mSecond;
        }
        INTERFACE_ADDREF(*result)
        return NOERROR;
    }
    // throw new IllegalArgumentException("length <= 0: " + length);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CCoderResult::UnmappableForLength(
    /* [in] */ Int32 length,
    /* [out] */ ICoderResult** result)
{
    VALIDATE_NOT_NULL(result);

    if (length > 0) {
    // TODO:
    //     Integer key = Integer.valueOf(length);
    //     synchronized (_unmappableErrors) {
    //         CoderResult r = _unmappableErrors.get(key);
    //         if (null == r) {
    //             r = new CoderResult(TYPE_UNMAPPABLE_CHAR, length);
    //             _unmappableErrors.put(key, r);
    //         }
    //         return r;
    //     }
        // Temporary achieve
        Mutex::Autolock lock(sUnmappableMutex);
        AutoPtr<ICoderResult> r;
        HashMap<Int32, AutoPtr<ICoderResult> >::Iterator iter = sUnmappableErrors->Find(length);
        if (iter == sUnmappableErrors->End()) {
            r = CreateCoderResult(TYPE_MALFORMED_INPUT, length);
            (*sUnmappableErrors)[length] = r;
            *result = r;
        }
        else {
            *result = iter->mSecond;
        }
        INTERFACE_ADDREF(*result)

    }
    // throw new IllegalArgumentException("length <= 0: " + length);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CCoderResult::IsUnderflow(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mType == TYPE_UNDERFLOW ? TRUE : FALSE;
    return NOERROR;
}

ECode CCoderResult::IsError(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (mType == TYPE_MALFORMED_INPUT ? TRUE : FALSE)
        || (mType == TYPE_UNMAPPABLE_CHAR ? TRUE : FALSE);
    return NOERROR;
}

ECode CCoderResult::IsMalformed(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mType == TYPE_MALFORMED_INPUT ? TRUE : FALSE;
    return NOERROR;
}

ECode CCoderResult::IsOverflow(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mType == TYPE_OVERFLOW ? TRUE : FALSE;
    return NOERROR;
}

ECode CCoderResult::IsUnmappable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mType == TYPE_UNMAPPABLE_CHAR ? TRUE : FALSE;
    return NOERROR;
}

ECode CCoderResult::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);

    if (mType == TYPE_MALFORMED_INPUT || mType == TYPE_UNMAPPABLE_CHAR) {
        *length = mLength;
        return NOERROR;
    }
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCoderResult::ThrowException()
{
    switch (mType) {
        case TYPE_UNDERFLOW:
            return E_BUFFER_UNDER_FLOW_EXCEPTION;
        case TYPE_OVERFLOW:
            return E_BUFFER_OVER_FLOW_EXCEPTION;
        case TYPE_UNMAPPABLE_CHAR:
            return E_UNMAPPABLE_CHARACTER_EXCEPTION;
        case TYPE_MALFORMED_INPUT:
            return E_MALFORMED_INPUT_EXCEPTION;
    }

    return E_CHARACTER_CODING_EXCEPTION;
}

ECode CCoderResult::ToString(
    /* [out] */ String* toString)
{
    VALIDATE_NOT_NULL(toString);

    String dsc;
    switch (mType) {
        case TYPE_UNDERFLOW: {
            dsc = String("UNDERFLOW error");
            break;
        }
        case TYPE_OVERFLOW: {
            dsc = String("OVERFLOW error");
            break;
        }
        case TYPE_UNMAPPABLE_CHAR: {
            dsc = String("Unmappable-character error with erroneous input length ");
            dsc += (StringUtils::Int32ToString(mLength));
            break;
        }
        case TYPE_MALFORMED_INPUT: {
            dsc = String("Malformed-input error with erroneous input length ");
            dsc += (StringUtils::Int32ToString(mLength));
            break;
        }
        default: {
            dsc = String("");
            break;
        }
    }

    *toString = __FILE__;
    *toString += (__FUNCTION__);
    *toString += ("[");
    *toString += (dsc);
    *toString += ("]");

    return NOERROR;
}

} // namespace Charset
} // namespace IO
} // namespace Elastos
