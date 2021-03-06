
#include "cmdef.h"
#include "NumberFormat.h"
#include "CNumberFormatField.h"
#include "CDecimalFormat.h"
#include "CLocaleData.h"

using Elastos::Text::IFieldPosition;
using Elastos::Text::CFieldPosition;
using Libcore::ICU::CLocaleData;
using Elastos::Math::IBigInteger;
using Elastos::Core::IByte;
using Elastos::Core::IInteger16;
using Elastos::Core::IInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::IStringBuffer;

namespace Elastos {
namespace Text {

static AutoPtr<INumberFormatField> sInit(const String& name)
{
     AutoPtr<CNumberFormatField> field;
     CNumberFormatField::NewByFriend(name, (CNumberFormatField**)&field);
     return (INumberFormatField*)field;
}

const AutoPtr<INumberFormatField> NumberFormat::Field::SIGN = sInit(String("sign"));
const AutoPtr<INumberFormatField> NumberFormat::Field::INTEGER = sInit(String("integer"));
const AutoPtr<INumberFormatField> NumberFormat::Field::FRACTION = sInit(String("fraction"));
const AutoPtr<INumberFormatField> NumberFormat::Field::EXPONENT = sInit(String("exponent"));
const AutoPtr<INumberFormatField> NumberFormat::Field::EXPONENT_SIGN = sInit(String("exponent sign"));
const AutoPtr<INumberFormatField> NumberFormat::Field::EXPONENT_SYMBOL = sInit(String("exponent symbol"));
const AutoPtr<INumberFormatField> NumberFormat::Field::DECIMAL_SEPARATOR = sInit(String("decimal separator"));
const AutoPtr<INumberFormatField> NumberFormat::Field::GROUPING_SEPARATOR = sInit(String("grouping separator"));
const AutoPtr<INumberFormatField> NumberFormat::Field::PERCENT = sInit(String("percent"));
const AutoPtr<INumberFormatField> NumberFormat::Field::PERMILLE = sInit(String("per mille"));
const AutoPtr<INumberFormatField> NumberFormat::Field::CURRENCY = sInit(String("currency"));

ECode NumberFormat::Field::Init(
   /* [in] */ const String& fn)
{
   return Format::Field::Init(fn);
}

NumberFormat::NumberFormat()
    : mGroupingUsed(TRUE)
    , mParseIntegerOnly(FALSE)
    , mMaximumIntegerDigits(40)
    , mMinimumIntegerDigits(1)
    , mMaximumFractionDigits(3)
    , mMinimumFractionDigits(0)
{}

ECode NumberFormat::FormatDouble(
    /* [in] */ Double value,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IFieldPosition> position;
    CFieldPosition::New(0, (IFieldPosition**)&position);
    AutoPtr<IStringBuffer> sb = new StringBuffer();
    AutoPtr<IStringBuffer> outsb;
    FormatDoubleEx(value, sb, position, (IStringBuffer **)&outsb);
    return outsb->ToString(result);
}

ECode NumberFormat::FormatInt64(
    /* [in] */ Int64 value,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IFieldPosition> position;
    CFieldPosition::New(0, (IFieldPosition**)&position);
    AutoPtr<IStringBuffer> sb = new StringBuffer();
    AutoPtr<IStringBuffer> outsb;
    FormatInt64Ex(value, sb, position, (IStringBuffer **)&outsb);
    return outsb->ToString(result);
}

ECode NumberFormat::FormatObjectEx(
    /* [in] */ IInterface* object,
    /* [in] */ IStringBuffer * buffer,
    /* [in] */ IFieldPosition* field,
    /* [out] */ IStringBuffer ** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    Int32 bitlen(0);
    AutoPtr<IBigInteger> outint = IBigInteger::Probe(object);;
    if (outint) {
        outint->BitLength(&bitlen);
    }

    if (IByte::Probe(object) != NULL
        || IByte::Probe(object) != NULL
        || IInteger16::Probe(object) != NULL
        || IInteger32::Probe(object) != NULL
        || IInteger64::Probe(object) != NULL
        || (outint != NULL && bitlen < 64)) {
        Int64 lv(0);
        INumber::Probe(object)->Int64Value(&lv);
        return FormatInt64Ex(lv, buffer, field, value);
    }
    else if (INumber::Probe(object) != NULL) {
        Double dv(0);
        INumber::Probe(object)->DoubleValue(&dv);
        return FormatDoubleEx(dv, buffer, field, value);
    }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode NumberFormat::GetAvailableLocales(
    /* [out] */ ArrayOf<ILocale*>** locales)
{
    VALIDATE_NOT_NULL(locales);
    *locales = NULL;

    AutoPtr<IICUHelper> ICUHelper;
    FAIL_RETURN(CICUHelper::AcquireSingleton((IICUHelper **)&ICUHelper));
    return ICUHelper->GetAvailableNumberFormatLocales(locales);
}

ECode NumberFormat::GetCurrency(
    /* [out] */ ICurrency** currency)
{
    VALIDATE_NOT_NULL(currency);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode NumberFormat::GetCurrencyInstance(
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetCurrencyInstance(locale, instance);
}

ECode NumberFormat::GetCurrencyInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<ILocaleDataHelper> helper;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&helper);
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(helper->Get(locale, (ILocaleData**)&localeData));
    String currencyPattern;
    localeData->GetCurrencyPattern(&currencyPattern);
    return GetInstance(currencyPattern, locale, instance);
}

ECode NumberFormat::GetIntegerInstance(
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<ILocaleHelper> pLocaleHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&pLocaleHelper);
    AutoPtr<ILocale> pLocale;
    pLocaleHelper->GetDefault((ILocale**)&pLocale);
    return GetIntegerInstance(pLocale, instance);
}

ECode NumberFormat::GetIntegerInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<INumberFormat> result;
    AutoPtr<ILocaleDataHelper> datahelper;
    AutoPtr<ILocaleData> data;
    FAIL_RETURN(CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper **)&datahelper));
    datahelper->Get(locale,(ILocaleData **)&data);

    String integerPattern;
    data->GetIntegerPattern(&integerPattern);
    AutoPtr<INumberFormat> format;
    ECode ec = GetInstance(integerPattern, locale, (INumberFormat**)&format);
    if (SUCCEEDED(ec) && format.Get()) {
        format->SetParseIntegerOnly(TRUE);
        *instance = format;
        INTERFACE_ADDREF(*instance);
    }
    return ec;
}

ECode NumberFormat::GetInstance(
    /* [out] */ INumberFormat** instance)
{
    return GetNumberInstance(instance);
}

ECode NumberFormat::GetInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    return GetNumberInstance(locale, instance);
}

ECode NumberFormat::GetInstance(
    /* [in] */ const String& pattern,
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<IDecimalFormat> format;
    FAIL_RETURN(CDecimalFormat::New(pattern, locale, (IDecimalFormat**)&format));
    *instance = format;
    INTERFACE_ADDREF(*instance);
    return NOERROR;
}

ECode NumberFormat::GetMaximumFractionDigits(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = mMaximumFractionDigits;
    return NOERROR;
}

ECode NumberFormat::GetMaximumIntegerDigits(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = mMaximumIntegerDigits;
    return NOERROR;
}

ECode NumberFormat::GetMinimumFractionDigits(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = mMinimumFractionDigits;
    return NOERROR;
}

ECode NumberFormat::GetMinimumIntegerDigits(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = mMinimumIntegerDigits;
    return NOERROR;
}

ECode NumberFormat::GetNumberInstance(
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetNumberInstance(locale, instance);
}

ECode NumberFormat::GetNumberInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<ILocaleDataHelper> helper;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&helper);
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(helper->Get(locale, (ILocaleData**)&localeData));
    String numberPattern;
    localeData->GetNumberPattern(&numberPattern);
    return GetInstance(numberPattern, locale, instance);
}

ECode NumberFormat::GetPercentInstance(
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<ILocaleHelper> localeHelper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&localeHelper);
    AutoPtr<ILocale> locale;
    FAIL_RETURN(localeHelper->GetDefault((ILocale**)&locale));
    return GetPercentInstance(locale, instance);
}

ECode NumberFormat::GetPercentInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ INumberFormat** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<ILocaleDataHelper> helper;
    CLocaleDataHelper::AcquireSingleton((ILocaleDataHelper**)&helper);
    AutoPtr<ILocaleData> localeData;
    FAIL_RETURN(helper->Get(locale, (ILocaleData**)&localeData));
    String percentPattern;
    localeData->GetPercentPattern(&percentPattern);
    return GetInstance(percentPattern, locale, instance);
}

ECode NumberFormat::IsGroupingUsed(
    /* [out] */ Boolean* isGroupingUsed)
{
    VALIDATE_NOT_NULL(isGroupingUsed);
    *isGroupingUsed = mGroupingUsed;
    return NOERROR;
}

ECode NumberFormat::IsParseIntegerOnly(
    /* [out] */ Boolean* isParseIntegerOnly)
{
    VALIDATE_NOT_NULL(isParseIntegerOnly);
    *isParseIntegerOnly = mParseIntegerOnly;
    return NOERROR;
}

ECode NumberFormat::Parse(
    /* [in] */ const String& string,
    /* [out] */ INumber** number)
{
    VALIDATE_NOT_NULL(number);
    *number = NULL;

    AutoPtr<IParsePosition> pos;
    CParsePosition::New(0, (IParsePosition**)&pos);
    FAIL_RETURN(ParseEx(string, pos, number));
    Int32 index;
    pos->GetIndex(&index);
    if (index == 0) {
        return E_PARSE_EXCEPTION;
    }
    return NOERROR;
}

ECode NumberFormat::ParseObjectEx(
    /* [in] */ const String& string,
    /* [in] */ IParsePosition* position,
    /* [out] */ IInterface** object)
{
    return ParseEx(string, position, (INumber**)object);
}

ECode NumberFormat::SetCurrency(
   /* [in] */ ICurrency* currency)
{
    VALIDATE_NOT_NULL(currency);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode NumberFormat::SetGroupingUsed(
    /* [in] */ Boolean value)
{
    mGroupingUsed = value;
    return NOERROR;
}

ECode NumberFormat::SetMaximumFractionDigits(
    /* [in] */ Int32 value)
{
    mMaximumFractionDigits = value < 0 ? 0 : value;
    if (mMaximumFractionDigits < mMinimumFractionDigits) {
        mMinimumFractionDigits = mMaximumFractionDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetMaximumIntegerDigits(
    /* [in] */ Int32 value)
{
    mMaximumIntegerDigits = value < 0 ? 0 : value;
    if (mMaximumIntegerDigits < mMinimumIntegerDigits) {
        mMinimumIntegerDigits = mMaximumIntegerDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetMinimumFractionDigits(
    /* [in] */ Int32 value)
{
    mMinimumFractionDigits = value < 0 ? 0 : value;
    if (mMaximumFractionDigits < mMinimumFractionDigits) {
        mMaximumFractionDigits = mMinimumFractionDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetMinimumIntegerDigits(
    /* [in] */ Int32 value)
{
    mMinimumIntegerDigits = value < 0 ? 0 : value;
    if (mMaximumIntegerDigits < mMinimumIntegerDigits) {
        mMaximumIntegerDigits = mMinimumIntegerDigits;
    }
    return NOERROR;
}

ECode NumberFormat::SetParseIntegerOnly(
    /* [in] */ Boolean value)
{
    mParseIntegerOnly = value;
    return NOERROR;
}

ECode NumberFormat::GetRoundingMode(
    /* [out] */ RoundingMode* roundingMode)
{
    VALIDATE_NOT_NULL(roundingMode);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode NumberFormat::SetRoundingMode(
    /* [in] */ RoundingMode roundingMode)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode NumberFormat::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (IDateFormat::Probe(object) == this->Probe(EIID_IDateFormat)) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_INumberFormat) == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<INumberFormat> obj = (INumberFormat*)object;
    Boolean gu, pio;
    obj->IsGroupingUsed(&gu);
    obj->IsParseIntegerOnly(&pio);
    Int32 mfd1, mid1, mfd2, mid2;
    obj->GetMaximumFractionDigits(&mfd1);
    obj->GetMaximumIntegerDigits(&mid1);
    obj->GetMinimumFractionDigits(&mfd2);
    obj->GetMinimumIntegerDigits(&mid2);

    *result = mGroupingUsed == gu
            && mParseIntegerOnly == pio
            && mMaximumFractionDigits == mfd1
            && mMaximumIntegerDigits == mid1
            && mMinimumFractionDigits == mfd2
            && mMinimumIntegerDigits == mid2;
    return NOERROR;
}

} // namespace Text
} // namespace Elastos
