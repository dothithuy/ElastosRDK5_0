
#include "NativeCollation.h"
#include "ElStringByteSink.h"
#include <unicode/ucol.h>
#include <unicode/ucoleitr.h>
#include <utils/UniquePtr.h>

namespace Libcore {
namespace ICU {

HashMap<Int32, UnicodeString*> NativeCollation::sUstrMap;
Mutex NativeCollation::sUstrMapLock;

#define UCOL_MAX_BUFFER 128

static UCollator* toCollator(
    /* [in] */ Int32 address)
{
    return reinterpret_cast<UCollator*>(static_cast<uintptr_t>(address));
}

static UCollationElements* toCollationElements(
   /* [in] */ Int32 address)
{
   return reinterpret_cast<UCollationElements*>(static_cast<uintptr_t>(address));
}

/**
* Checks if an error has occurred, throwing a suitable exception if so.
* @param env JNI environment
* @param errorCode code to determine if it is an error
* @return 0 if errorCode is not an error, 1 if errorCode is an error, but the
*         creation of the exception to be thrown fails
 * @exception thrown if errorCode represents an error
*/
static ECode maybeThrowIcuException(
    /* [in] */ UErrorCode errorCode)
{
    if (errorCode <= U_ZERO_ERROR) {
        return NOERROR;
    }

    switch (errorCode) {
        case U_ILLEGAL_ARGUMENT_ERROR:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case U_INDEX_OUTOFBOUNDS_ERROR:
        case U_BUFFER_OVERFLOW_ERROR:
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        case U_UNSUPPORTED_ERROR:
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        default:
            return E_RUNTIME_EXCEPTION;
    }
}

void NativeCollation::CloseCollator(
    /* [in] */ Int32 collatoraddress)
{
    ucol_close(toCollator(collatoraddress));
}

void NativeCollation::CloseElements(
    /* [in] */ Int32 address)
{
    ucol_closeElements(toCollationElements(address));
    Mutex::Autolock lock(sUstrMapLock);
    HashMap<Int32, UnicodeString*>::Iterator find = sUstrMap.Find(address);
    if (find != sUstrMap.End()) {
        delete find->mSecond;
        sUstrMap.Erase(address);
    }
}

Int32 NativeCollation::Compare(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ const String& source,
    /* [in] */ const String& target)
{
    if (source.IsNull()) {
        return 0;
    }
    if (target.IsNull()) {
        return 0;
    }

    return ucol_strcoll(toCollator(collatoraddress),
            UnicodeString::fromUTF8(source.string()).getBuffer(),
            UnicodeString::fromUTF8(source.string()).length(),
            UnicodeString::fromUTF8(target.string()).getBuffer(),
            UnicodeString::fromUTF8(target.string()).length());
}

ECode NativeCollation::GetAttribute(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ Int32 type,
    /* [out] */ Int32* result)
{
    UErrorCode status = U_ZERO_ERROR;
    *result = ucol_getAttribute(toCollator(collatoraddress),
            (UColAttribute)type, &status);
    return maybeThrowIcuException(status);
}

ECode NativeCollation::GetCollationElementIterator(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ const String& source,
    /* [out] */ Int32* coleitr)
{
    if (source.IsNull()) {
        *coleitr = -1;
        return NOERROR;
    }

    UnicodeString* ustr = new UnicodeString(UnicodeString::fromUTF8(source.string()));
    UErrorCode status = U_ZERO_ERROR;
    UCollationElements* result = ucol_openElements(toCollator(collatoraddress),
        ustr->getBuffer(), ustr->length(), &status);
    ECode ec = maybeThrowIcuException(status);
    if (FAILED(ec)) {
        delete ustr;
        return ec;
    }
    *coleitr = static_cast<Int32>(reinterpret_cast<uintptr_t>(result));
    Mutex::Autolock lock(sUstrMapLock);
    sUstrMap[*coleitr] = ustr;
    return NOERROR;
}

Int32 NativeCollation::GetMaxExpansion(
    /* [in] */ Int32 address,
    /* [in] */ Int32 order)
{
    return ucol_getMaxExpansion(toCollationElements(address), order);
}

Int32 NativeCollation::GetOffset(
    /* [in] */ Int32 address)
{
    return ucol_getOffset(toCollationElements(address));
}

String NativeCollation::GetRules(
    /* [in] */ Int32 collatoraddress)
{
    int32_t length = 0;
    UnicodeString rules = ucol_getRules(toCollator(collatoraddress), &length);
    String s("");
    ElStringByteSink sink(&s);
    rules.toUTF8(sink);
    return s;
}

AutoPtr<ArrayOf<Byte> > NativeCollation::GetSortKey(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ const String& source)
{
    if (source.IsNull()) {
        return NULL;
    }
    const UCollator* collator  = toCollator(collatoraddress);
    uint8_t byteArray[UCOL_MAX_BUFFER/*128*/ * 2];
    UniquePtr<uint8_t[]> largerByteArray;
    uint8_t* usedByteArray = byteArray;
    UnicodeString ustring = UnicodeString::fromUTF8(source.string());
    const UChar* chars = ustring.getBuffer();
    size_t charCount = ustring.length();
    size_t byteArraySize = ucol_getSortKey(collator, chars, charCount, usedByteArray, sizeof(byteArray) - 1);
    if (byteArraySize > sizeof(byteArray) - 1) {
        // didn't fit, try again with a larger buffer.
        largerByteArray.reset(new uint8_t[byteArraySize + 1]);
        usedByteArray = largerByteArray.get();
        byteArraySize = ucol_getSortKey(collator, chars, charCount, usedByteArray, byteArraySize);
    }
    if (byteArraySize == 0) {
        return NULL;
    }

    AutoPtr<ArrayOf<Byte> > result = ArrayOf<Byte>::Alloc(byteArraySize);
    for (size_t i = 0; i < byteArraySize; ++i) {
        (*result)[i] = usedByteArray[i];
    }

    return result;
}

ECode NativeCollation::Next(
    /* [in] */ Int32 address,
    /* [out] */ Int32* result)
{
    UErrorCode status = U_ZERO_ERROR;
    *result = ucol_next(toCollationElements(address), &status);
    return maybeThrowIcuException(status);
}

ECode NativeCollation::OpenCollator(
    /* [in] */ const String& locale,
    /* [out] */ Int32* address)
{
    if (locale.string() == NULL) {
        *address = 0;
        return NOERROR;
    }
    UErrorCode status = U_ZERO_ERROR;
    UCollator* c = ucol_open(locale.string(), &status);
    FAIL_RETURN(maybeThrowIcuException(status));
    *address = static_cast<Int32>(reinterpret_cast<uintptr_t>(c));
    return NOERROR;
}

ECode NativeCollation::OpenCollatorFromRules(
    /* [in] */ const String& rules,
    /* [in] */ Int32 normalizationmode,
    /* [in] */ Int32 collationstrength,
    /* [out] */ Int32* address)
{
    if (rules.IsNull()) {
        *address = -1;
        return NOERROR;
    }

    UErrorCode status = U_ZERO_ERROR;
    UCollator* c = ucol_openRules(UnicodeString::fromUTF8(rules.string()).getBuffer(),
            UnicodeString::fromUTF8(rules.string()).length(),
            UColAttributeValue(normalizationmode),
            UCollationStrength(collationstrength),NULL, &status);
    FAIL_RETURN(maybeThrowIcuException(status));
    *address = static_cast<Int32>(reinterpret_cast<uintptr_t>(c));
    return NOERROR;
}

ECode NativeCollation::Previous(
    /* [in] */ Int32 address,
    /* [out] */ Int32* result)
{
    UErrorCode status = U_ZERO_ERROR;
    *result = ucol_previous(toCollationElements(address), &status);
    return maybeThrowIcuException(status);
}

void NativeCollation::Reset(
    /* [in] */ Int32 address)
{
    ucol_reset(toCollationElements(address));
}

ECode NativeCollation::SafeClone(
    /* [in] */ Int32 collatoraddress,
    /* [out] */ Int32* address)
{
    UErrorCode status = U_ZERO_ERROR;
    Int32 bufferSize = U_COL_SAFECLONE_BUFFERSIZE;
    UCollator* c = ucol_safeClone(toCollator(collatoraddress), NULL, &bufferSize, &status);
    FAIL_RETURN(maybeThrowIcuException(status));
    *address = static_cast<Int32>(reinterpret_cast<uintptr_t>(c));
    return NOERROR;
}

ECode NativeCollation::SetAttribute(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ Int32 type,
    /* [in] */ Int32 value)
{
    UErrorCode status = U_ZERO_ERROR;
    ucol_setAttribute(toCollator(collatoraddress), (UColAttribute)type,
            (UColAttributeValue)value, &status);
    return maybeThrowIcuException(status);
}

ECode NativeCollation::SetOffset(
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset)
{
    UErrorCode status = U_ZERO_ERROR;
    ucol_setOffset(toCollationElements(address), offset, &status);
    return maybeThrowIcuException(status);
}

ECode NativeCollation::SetText(
    /* [in] */ Int32 address,
    /* [in] */ const String& source)
{
    UnicodeString* ustr = new UnicodeString(UnicodeString::fromUTF8(source.string()));
    UErrorCode status = U_ZERO_ERROR;
    ucol_setText(toCollationElements(address),
        ustr->getBuffer(), ustr->length(), &status);

    ECode ec = maybeThrowIcuException(status);
    if (FAILED(ec)) {
        delete ustr;
        return ec;
    }
    Mutex::Autolock lock(sUstrMapLock);
    delete sUstrMap[address];
    sUstrMap[address] = ustr;

    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
