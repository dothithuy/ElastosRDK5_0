
#include "CGsmAlphabetHelper.h"
#include "CGsmAlphabet.h"
#include <ext/frameworkdef.h>

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Telephony {


ECode CGsmAlphabetHelper::CharToGsm(
    /* [in] */ Char32 c,
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    return CGsmAlphabet::CharToGsm(c, val);
}

ECode CGsmAlphabetHelper::CharToGsmEx(
    /* [in] */ Char32 c,
    /* [in] */ Boolean throwException,
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    return CGsmAlphabet::CharToGsmEx(c, throwException, val);
}

ECode CGsmAlphabetHelper::CharToGsmExtended(
    /* [in] */ Char32 c,
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    return CGsmAlphabet::CharToGsmExtended(c, val);
}

ECode CGsmAlphabetHelper::GsmToChar(
    /* [in] */ Int32 gsmChar,
    /* [out] */ Char32* res)
{
    VALIDATE_NOT_NULL(res);
    return CGsmAlphabet::GsmToChar(gsmChar, res);
}

ECode CGsmAlphabetHelper::GsmExtendedToChar(
    /* [in] */ Int32 gsmChar,
    /* [out] */ Char32* res)
{
    VALIDATE_NOT_NULL(res);
    return CGsmAlphabet::GsmExtendedToChar(gsmChar, res);
}

ECode CGsmAlphabetHelper::StringToGsm7BitPackedWithHeader(
    /* [in] */ const String& data,
    /* [in] */ ArrayOf<Byte>* header,
    /* [out, callee] */ ArrayOf<Byte>** res)
{

    return CGsmAlphabet::StringToGsm7BitPackedWithHeader(data, header, res);
}

ECode CGsmAlphabetHelper::StringToGsm7BitPackedWithHeaderEx(
    /* [in] */ const String& data,
    /* [in] */ ArrayOf<Byte>* header,
    /* [in] */ Int32 languageTable,
    /* [in] */ Int32 languageShiftTable,
    /* [out, callee] */ ArrayOf<Byte>** res)
{

    return CGsmAlphabet::StringToGsm7BitPackedWithHeaderEx(data, header, languageTable, languageShiftTable, res);
}

ECode CGsmAlphabetHelper::StringToGsm7BitPacked(
    /* [in] */ const String& data,
    /* [out, callee] */ ArrayOf<Byte>** res)
{

    return CGsmAlphabet::StringToGsm7BitPacked(data, res);
}

ECode CGsmAlphabetHelper::StringToGsm7BitPackedEx(
    /* [in] */ const String& data,
    /* [in] */ Int32 languageTable,
    /* [in] */ Int32 languageShiftTable,
    /* [out, callee] */ ArrayOf<Byte>** res)
{

    return CGsmAlphabet::StringToGsm7BitPackedEx(data, languageTable, languageShiftTable, res);
}

ECode CGsmAlphabetHelper::StringToGsm7BitPackedEx2(
    /* [in] */ const String& data,
    /* [in] */ Int32 startingSeptetOffset,
    /* [in] */ Boolean throwException,
    /* [in] */ Int32 languageTable,
    /* [in] */ Int32 languageShiftTable,
    /* [out, callee] */ ArrayOf<Byte>** res)
{

    return CGsmAlphabet::StringToGsm7BitPackedEx2(data, startingSeptetOffset, throwException, languageTable, languageShiftTable, res);
}

ECode CGsmAlphabetHelper::Gsm7BitPackedToString(
    /* [in] */ ArrayOf<Byte>* pdu,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 lengthSeptets,
    /* [out] */ String* res)
{
    VALIDATE_NOT_NULL(res);
    return CGsmAlphabet::Gsm7BitPackedToString(pdu, offset, lengthSeptets, res);
}

ECode CGsmAlphabetHelper::Gsm7BitPackedToStringEx(
    /* [in] */ ArrayOf<Byte>* pdu,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 lengthSeptets,
    /* [in] */ Int32 numPaddingBits,
    /* [in] */ Int32 languageTable,
    /* [in] */ Int32 shiftTable,
    /* [out] */ String* res)
{
    VALIDATE_NOT_NULL(res);
    return CGsmAlphabet::Gsm7BitPackedToStringEx(pdu, offset, lengthSeptets, numPaddingBits, languageTable, shiftTable, res);
}

ECode CGsmAlphabetHelper::Gsm8BitUnpackedToString(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ String* res)
{
    VALIDATE_NOT_NULL(res);
    return CGsmAlphabet::Gsm8BitUnpackedToString(data, offset, length, res);
}

ECode CGsmAlphabetHelper::Gsm8BitUnpackedToStringEx(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const String& characterset,
    /* [out] */ String* res)
{
    VALIDATE_NOT_NULL(res);
    return CGsmAlphabet::Gsm8BitUnpackedToStringEx(data, offset, length, characterset, res);
}

ECode CGsmAlphabetHelper::StringToGsm8BitPacked(
    /* [in] */ const String& s,
    /* [out, callee] */ ArrayOf<Byte>** res)
{

    return CGsmAlphabet::StringToGsm8BitPacked(s, res);
}

ECode CGsmAlphabetHelper::StringToGsm8BitUnpackedField(
    /* [in] */ const String& s,
    /* [in] */ ArrayOf<Byte>* dest,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{

    return CGsmAlphabet::StringToGsm8BitUnpackedField(s, dest, offset, length);
}

ECode CGsmAlphabetHelper::CountGsmSeptets(
    /* [in] */ Char32 c,
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    return CGsmAlphabet::CountGsmSeptets(c, val);
}

ECode CGsmAlphabetHelper::CountGsmSeptetsEx(
    /* [in] */ Char32 c,
    /* [in] */ Boolean throwsException,
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    return CGsmAlphabet::CountGsmSeptetsEx(c, throwsException, val);
}

ECode CGsmAlphabetHelper::CountGsmSeptetsUsingTables(
    /* [in] */ ICharSequence* s,
    /* [in] */ Boolean use7bitOnly,
    /* [in] */ Int32 languageTable,
    /* [in] */ Int32 languageShiftTable,
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    return CGsmAlphabet::CountGsmSeptetsUsingTables(s, use7bitOnly, languageTable, languageShiftTable, val);
}

ECode CGsmAlphabetHelper::CountGsmSeptetsEx2(
    /* [in] */ ICharSequence* s,
    /* [in] */ Boolean use7bitOnly,
    /* [out] */ IGsmAlphabetTextEncodingDetails** res)
{
    VALIDATE_NOT_NULL(res);
    return CGsmAlphabet::CountGsmSeptetsEx2(s, use7bitOnly, res);
}

ECode CGsmAlphabetHelper::FindGsmSeptetLimitIndex(
    /* [in] */ const String& s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 limit,
    /* [in] */ Int32 langTable,
    /* [in] */ Int32 langShiftTable,
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    return CGsmAlphabet::FindGsmSeptetLimitIndex(s, start, limit, langTable, langShiftTable, val);
}

ECode CGsmAlphabetHelper::SetEnabledSingleShiftTables(
    /* [in] */ ArrayOf<Int32>* tables)
{

    return CGsmAlphabet::SetEnabledSingleShiftTables(tables);
}

ECode CGsmAlphabetHelper::SetEnabledLockingShiftTables(
    /* [in] */ ArrayOf<Int32>* tables)
{

    return CGsmAlphabet::SetEnabledLockingShiftTables(tables);
}

ECode CGsmAlphabetHelper::GetEnabledSingleShiftTables(
    /* [out, callee] */ ArrayOf<Int32>** res)
{
    VALIDATE_NOT_NULL(res);
    return CGsmAlphabet::GetEnabledSingleShiftTables(res);
}

ECode CGsmAlphabetHelper::GetEnabledLockingShiftTables(
    /* [out, callee] */ ArrayOf<Int32>** res)
{
    VALIDATE_NOT_NULL(res);
    return CGsmAlphabet::GetEnabledLockingShiftTables(res);
}

}
}
}
}
