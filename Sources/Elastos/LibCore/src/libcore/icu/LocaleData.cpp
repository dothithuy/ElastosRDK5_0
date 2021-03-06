
#include "LocaleData.h"
#include "CLocaleData.h"
#include "CLocale.h"
#include "ICUUtil.h"
#include "elastos/StringUtils.h"

using Elastos::Core::StringUtils;

namespace Libcore {
namespace ICU {

HashMap< String, AutoPtr<ILocaleData> > LocaleData::sLocaleDataCache;
Mutex LocaleData::sLocaleDataCacheLock;

Boolean LocaleData::sIsInited = FALSE; //Init();

Boolean LocaleData::Init()
{
    if (!sIsInited) {
        sIsInited = TRUE;

        Get(CLocale::ROOT);
        Get(CLocale::US);
        Get(CLocale::GetDefault());
    }

    return sIsInited;
}

LocaleData::LocaleData()
    : mAmPm(NULL)
    , mEras(NULL)
    , mLongMonthNames(NULL)
    , mShortMonthNames(NULL)
    , mTinyMonthNames(NULL)
    , mLongStandAloneMonthNames(NULL)
    , mShortStandAloneMonthNames(NULL)
    , mTinyStandAloneMonthNames(NULL)
    , mLongWeekdayNames(NULL)
    , mShortWeekdayNames(NULL)
    , mTinyWeekdayNames(NULL)
    , mLongStandAloneWeekdayNames(NULL)
    , mShortStandAloneWeekdayNames(NULL)
    , mTinyStandAloneWeekdayNames(NULL)
{}

LocaleData::~LocaleData()
{
}

AutoPtr<ILocaleData> LocaleData::Get(
    /* [in] */ ILocale* _locale)
{
    AutoPtr<ILocale> locale = _locale;
    if (locale == NULL) {
        locale = CLocale::GetDefault();
    }
    String localeName;
    locale->ToString(&localeName);
    {
        Mutex::Autolock lock(&sLocaleDataCacheLock);

        HashMap< String, AutoPtr<ILocaleData> >::Iterator it =
                sLocaleDataCache.Find(localeName);
        if (it != sLocaleDataCache.End()) {
            return (ILocaleData*)it->mSecond.Get();
        }
    }
    AutoPtr<ILocaleData> newLocaleData = InitLocaleData(locale);
    {
        Mutex::Autolock lock(&sLocaleDataCacheLock);

        HashMap< String, AutoPtr<ILocaleData> >::Iterator it =
                sLocaleDataCache.Find(localeName);
        if (it != sLocaleDataCache.End()) {
            return (ILocaleData*)it->mSecond.Get();
        }
        sLocaleDataCache[localeName] = newLocaleData;
        return (ILocaleData*)newLocaleData.Get();
    }
}

String LocaleData::ToString()
{
    return String("LocaleData");
    // return Objects.toString(this);
}

ECode LocaleData::GetDateFormat(
    /* [in] */ DateFormat style,
    /* [out] */ String* format)
{
    VALIDATE_NOT_NULL(format);

    switch (style) {
    case DateFormat_SHORT:
        *format = mShortDateFormat;
        return NOERROR;
    case DateFormat_MEDIUM:
        *format = mMediumDateFormat;
        return NOERROR;
    case DateFormat_LONG:
        *format = mLongDateFormat;
        return NOERROR;
    case DateFormat_FULL:
        *format = mFullDateFormat;
        return NOERROR;
    }
    // throw new AssertionError();
    return E_ASSERTION_ERROR;
}

ECode LocaleData::GetTimeFormat(
    /* [in] */ DateFormat style,
    /* [out] */ String* format)
{
    VALIDATE_NOT_NULL(format);

    switch (style) {
    case DateFormat_SHORT:
        *format = mShortTimeFormat;
        return NOERROR;
    case DateFormat_MEDIUM:
        *format = mMediumTimeFormat;
        return NOERROR;
    case DateFormat_LONG:
        *format = mLongTimeFormat;
        return NOERROR;
    case DateFormat_FULL:
        *format = mFullTimeFormat;
        return NOERROR;
    }
    // throw new AssertionError();
    return E_ASSERTION_ERROR;
}

ECode LocaleData::GetFirstDayOfWeek(
    /* [out] */ IInteger32** day)
{
    VALIDATE_NOT_NULL(day);
    *day = mFirstDayOfWeek;
    INTERFACE_ADDREF(*day);
    return NOERROR;
}

ECode LocaleData::GetMinimalDaysInFirstWeek(
    /* [out] */ IInteger32** days)
{
    VALIDATE_NOT_NULL(days);
    *days = mMinimalDaysInFirstWeek;
    INTERFACE_ADDREF(*days);
    return NOERROR;
}

ECode LocaleData::GetAmPm(
    /* [out] */ ArrayOf<String>** amPm)
{
    VALIDATE_NOT_NULL(amPm);
    *amPm = mAmPm;
    INTERFACE_ADDREF(*amPm)
    return NOERROR;
}

ECode LocaleData::GetEras(
    /* [out] */ ArrayOf<String>** eras)
{
    VALIDATE_NOT_NULL(eras);
    *eras = mEras;
    INTERFACE_ADDREF(*eras)
    return NOERROR;
}

ECode LocaleData::GetLongMonthNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mLongMonthNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetShortMonthNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mShortMonthNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetLongStandAloneMonthNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mLongStandAloneMonthNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetTinyMonthNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mTinyMonthNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetShortStandAloneMonthNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mShortStandAloneMonthNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetTinyStandAloneMonthNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mTinyStandAloneMonthNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetLongWeekdayNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mLongWeekdayNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetShortWeekdayNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mShortWeekdayNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetTinyWeekdayNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mTinyWeekdayNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetLongStandAloneWeekdayNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mLongStandAloneWeekdayNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetShortStandAloneWeekdayNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mShortStandAloneWeekdayNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetTinyStandAloneWeekdayNames(
    /* [out] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mTinyStandAloneWeekdayNames;
    INTERFACE_ADDREF(*names)
    return NOERROR;
}

ECode LocaleData::GetYesterday(
    /* [out] */ String* yesterday)
{
    VALIDATE_NOT_NULL(yesterday);
    *yesterday = mYesterday;
    return NOERROR;
}

ECode LocaleData::GetToday(
    /* [out] */ String* today)
{
    VALIDATE_NOT_NULL(today);
    *today = mToday;
    return NOERROR;
}

ECode LocaleData::GetTomorrow(
    /* [out] */ String* tomorrow)
{
    VALIDATE_NOT_NULL(tomorrow);
    *tomorrow = mTomorrow;
    return NOERROR;
}

ECode LocaleData::GetZeroDigit(
    /* [out] */ Char32* zeroDigit)
{
    VALIDATE_NOT_NULL(zeroDigit);
    *zeroDigit = mZeroDigit;
    return NOERROR;
}

ECode LocaleData::GetDecimalSeparator(
    /* [out] */ Char32* decimalSeparator)
{
    VALIDATE_NOT_NULL(decimalSeparator);
    *decimalSeparator = mDecimalSeparator;
    return NOERROR;
}

ECode LocaleData::GetGroupingSeparator(
    /* [out] */ Char32* groupingSeparator)
{
    VALIDATE_NOT_NULL(groupingSeparator);
    *groupingSeparator = mGroupingSeparator;
    return NOERROR;
}

ECode LocaleData::GetPatternSeparator(
    /* [out] */ Char32* patternSeparator)
{
    VALIDATE_NOT_NULL(patternSeparator);
    *patternSeparator = mPatternSeparator;
    return NOERROR;
}

ECode LocaleData::GetPercent(
    /* [out] */ Char32* percent)
{
    VALIDATE_NOT_NULL(percent);
    *percent = mPercent;
    return NOERROR;
}

ECode LocaleData::GetPerMill(
    /* [out] */ Char32* perMill)
{
    VALIDATE_NOT_NULL(perMill);
    *perMill = mPerMill;
    return NOERROR;
}

ECode LocaleData::GetMonetarySeparator(
    /* [out] */ Char32* monetarySeparator)
{
    VALIDATE_NOT_NULL(monetarySeparator);
    *monetarySeparator = mMonetarySeparator;
    return NOERROR;
}

ECode LocaleData::GetMinusSign(
    /* [out] */ Char32* minusSign)
{
    VALIDATE_NOT_NULL(minusSign);
    *minusSign = mMinusSign;
    return NOERROR;
}

ECode LocaleData::GetExponentSeparator(
    /* [out] */ String* exponentSeparator)
{
    VALIDATE_NOT_NULL(exponentSeparator);
    *exponentSeparator = mExponentSeparator;
    return NOERROR;
}

ECode LocaleData::GetInfinity(
    /* [out] */ String* infinity)
{
    VALIDATE_NOT_NULL(infinity);
    *infinity = mInfinity;
    return NOERROR;
}

ECode LocaleData::GetNaN(
    /* [out] */ String* naN)
{
    VALIDATE_NOT_NULL(naN);
    *naN = mNaN;
    return NOERROR;
}

ECode LocaleData::GetCurrencySymbol(
    /* [out] */ String* currencySymbol)
{
    VALIDATE_NOT_NULL(currencySymbol);
    *currencySymbol = mCurrencySymbol;
    return NOERROR;
}

ECode LocaleData::GetInternationalCurrencySymbol(
    /* [out] */ String* internationalCurrencySymbol)
{
    VALIDATE_NOT_NULL(internationalCurrencySymbol);
    *internationalCurrencySymbol = mInternationalCurrencySymbol;
    return NOERROR;
}

ECode LocaleData::GetNumberPattern(
    /* [out] */ String* numberPattern)
{
    VALIDATE_NOT_NULL(numberPattern);
    *numberPattern = mNumberPattern;
    return NOERROR;
}

ECode LocaleData::GetIntegerPattern(
    /* [out] */ String* integerPattern)
{
    VALIDATE_NOT_NULL(integerPattern);
    *integerPattern = mIntegerPattern;
    return NOERROR;
}

ECode LocaleData::GetCurrencyPattern(
    /* [out] */ String* currencyPattern)
{
    VALIDATE_NOT_NULL(currencyPattern);
    *currencyPattern = mCurrencyPattern;
    return NOERROR;
}

ECode LocaleData::GetPercentPattern(
    /* [out] */ String* percentPattern)
{
    VALIDATE_NOT_NULL(percentPattern);
    *percentPattern = mPercentPattern;
    return NOERROR;
}

AutoPtr<ILocaleData> LocaleData::InitLocaleData(
    /* [in] */ ILocale* locale)
{
    AutoPtr<CLocaleData> localeObj;
    CLocaleData::NewByFriend((CLocaleData**)&localeObj);
    LocaleData* localeData = (LocaleData*)localeObj.Get();
    String localeName;
    locale->ToString(&localeName);
    if (!ICUUtil::InitLocaleDataImpl(localeName, localeData)) {
        return NULL;
    }
    if (!localeData->mFullTimeFormat.IsNull()) {
        // There are some full time format patterns in ICU that use the pattern character 'v'.
        // Java doesn't accept this, so we replace it with 'z' which has about the same result
        // as 'v', the timezone name.
        // 'v' -> "PT", 'z' -> "PST", v is the generic timezone and z the standard tz
        // "vvvv" -> "Pacific Time", "zzzz" -> "Pacific Standard Time"
        localeData->mFullTimeFormat = (localeData->mFullTimeFormat).Replace('v', 'z');
    }
    if (!localeData->mNumberPattern.IsNull()) {
        // The number pattern might contain positive and negative subpatterns. Arabic, for
        // example, might look like "#,##0.###;#,##0.###-" because the minus sign should be
        // written last. Macedonian supposedly looks something like "#,##0.###;(#,##0.###)".
        // (The negative subpattern is optional, though, and not present in most locales.)
        // By only swallowing '#'es and ','s after the '.', we ensure that we don't
        // accidentally eat too much.
        // localeData.integerPattern = localeData.numberPattern.replaceAll("\\.[#,]*", "");
        StringUtils::ReplaceAll(localeData->mNumberPattern, String("\\.[#,]*"), String("") ,
            &localeData->mIntegerPattern);
    }
    return (ILocaleData*)localeObj.Get();
}

} // namespace ICU
} // namespace Libcore
