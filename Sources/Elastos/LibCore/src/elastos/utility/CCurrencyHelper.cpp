
#include "CCurrencyHelper.h"
#include "Currency.h"

namespace Elastos{
namespace Utility{

ECode CCurrencyHelper::GetInstance(
    /* [in] */ const String& currencyCode,
    /* [out] */ ICurrency** outcur)
{
    VALIDATE_NOT_NULL(outcur)

    AutoPtr<ICurrency> cur = Currency::GetInstance(currencyCode);
    *outcur = cur;
    INTERFACE_ADDREF(*outcur)
    return NOERROR;
}

ECode CCurrencyHelper::GetInstanceEx(
    /* [in] */ ILocale* locale,
    /* [out] */ ICurrency** outcur)
{
    VALIDATE_NOT_NULL(outcur)

    return Currency::GetInstance(locale, outcur);
}

ECode CCurrencyHelper::GetAvailableCurrencies(
    /* [out] */ ISet** currencies)
{
    return Currency::GetAvailableCurrencies(currencies);
}

} // namespace Utility
} // namespace Elastos
