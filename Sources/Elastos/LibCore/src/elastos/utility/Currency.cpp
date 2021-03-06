
#include "Currency.h"
#include "CLocale.h"
#include "ICUUtil.h"
#include "CLocaleData.h"
#include "CLinkedHashSet.h"

using Libcore::ICU::CLocale;
using Libcore::ICU::ICUUtil;
using Libcore::ICU::LocaleData;
using Libcore::ICU::ILocaleData;
using Libcore::ICU::CLocaleData;

namespace Elastos{
namespace Utility{

HashMap<String, AutoPtr<ICurrency> > Currency::sCodesToCurrencies;
HashMap<AutoPtr<ILocale>, AutoPtr<ICurrency> > Currency::sLocalesToCurrencies;
Mutex Currency::mLock;

CAR_INTERFACE_IMPL(Currency, ICurrency)

Currency::Currency(
    /* [in] */ const String& currencyCode)
{
    mCurrencyCode = currencyCode;
    String usstr;
    CLocale::US->ToString(&usstr);
    String symbol = ICUUtil::GetCurrencySymbol(usstr, mCurrencyCode);
    if (symbol.IsNull()) {
        // throw new IllegalArgumentException("Unsupported ISO 4217 currency code: " + currencyCode);
        printf("Unsupported ISO 4217 currency code:  %s\n", mCurrencyCode.string());
    }
}

AutoPtr<ICurrency> Currency::GetInstance(
    /* [in] */ const String& currencyCode)
{
    Mutex::Autolock lock(mLock);

    AutoPtr<ICurrency> currency = sCodesToCurrencies[currencyCode];
    if (currency == NULL) {
        currency = new Currency(currencyCode);
        sCodesToCurrencies[currencyCode] = currency;
    }
    return currency;
}

ECode Currency::GetInstance(
    /* [in] */ ILocale* locale,
    /* [out] */ ICurrency** instance)
{
    Mutex::Autolock lock(mLock);

    AutoPtr<ICurrency> currency = sLocalesToCurrencies[locale];
    if (currency != NULL) {
        *instance = currency;
        INTERFACE_ADDREF(*instance)
        return NOERROR;
    }
    String country;
    locale->GetCountry(&country);
    String variant;
    locale->GetVariant(&variant);
    if (!variant.IsEmpty()
        && (variant.Equals("EURO")
        || variant.Equals("HK")
        || variant.Equals("PREEURO"))) {
        country = country + String("_") + variant;
    }

    String currencyCode = ICUUtil::GetCurrencyCode(country);
    if (currencyCode.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;;
    }
    else if (currencyCode.Equals("XXX")) {
        *instance = NULL;
        return NOERROR;
    }
    AutoPtr<ICurrency> result = GetInstance(currencyCode);
    sLocalesToCurrencies[locale] = result;
    *instance = result;
    INTERFACE_ADDREF(*instance)
    return NOERROR;
}

ECode Currency::GetAvailableCurrencies(
    /* [out] */ ISet** currencies)
{
    VALIDATE_NOT_NULL(currencies);
    FAIL_RETURN(CLinkedHashSet::New(currencies));
    AutoPtr<ArrayOf<String> > currencyCodes;
    FAIL_RETURN(ICUUtil::GetAvailableCurrencyCodes((ArrayOf<String>**)&currencyCodes));
    Int32 length = currencyCodes->GetLength();
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<ICurrency> currency = GetInstance((*currencyCodes)[i]);
        Boolean res;
        (*currencies)->Add((IInterface*)currency, &res);
    }
    return NOERROR;
}

ECode Currency::GetCurrencyCode(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mCurrencyCode;
    return NOERROR;
}

ECode Currency::GetDisplayName(
    /* [out] */ String* str)
{
    return GetDisplayNameEx(CLocale::GetDefault(), str);
}

ECode Currency::GetDisplayNameEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    String locstr;
    locale->ToString(&locstr);
    *str = ICUUtil::GetCurrencyDisplayName(locstr, mCurrencyCode);
    return NOERROR;
}

ECode Currency::GetSymbol(
    /* [out] */ String* str)
{
    return GetSymbolEx(CLocale::GetDefault(), str);
}

ECode Currency::GetSymbolEx(
    /* [in] */ ILocale* locale,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    String coustr;
    locale->GetCountry(&coustr);
    if (coustr.IsNullOrEmpty()) {
        *str = mCurrencyCode;
        return NOERROR;
    }

    // Check the locale first, in case the locale has the same currency.
    AutoPtr<ILocaleData> localeData = LocaleData::Get(locale);
    if (((CLocaleData*)localeData.Get())->mInternationalCurrencySymbol.Equals(mCurrencyCode)) {
        *str = ((CLocaleData*)localeData.Get())->mCurrencySymbol;
        return NOERROR;
    }

    // Try ICU, and fall back to the currency code if ICU has nothing.
    String symbol = ICUUtil::GetCurrencySymbol((locale->ToString(&coustr), coustr), mCurrencyCode);
    *str = !symbol.IsNull() ? symbol : mCurrencyCode;
    return NOERROR;
}

ECode Currency::GetDefaultFractionDigits(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    // In some places the code XXX is used as the fall back currency.
    // The RI returns -1, but ICU defaults to 2 for unknown currencies.
    if (mCurrencyCode.Equals("XXX")) {
        *value = -1;
        return NOERROR;
    }
    *value = ICUUtil::GetCurrencyFractionDigits(mCurrencyCode);
    return NOERROR;
}

ECode Currency::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mCurrencyCode;
    return NOERROR;
}

AutoPtr<IInterface> Currency::ReadResolve()
{
    return GetInstance(mCurrencyCode);
}

} // namespace Utility
} // namespace Elastos
