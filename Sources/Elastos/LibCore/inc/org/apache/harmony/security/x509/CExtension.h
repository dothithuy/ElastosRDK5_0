
#ifndef __CEXTENSION_H__
#define __CEXTENSION_H__

#include "_CExtension.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CExtension)
{
public:
    CARAPI GetExtnID(
        /* [out] */ String * pExtnID);

    CARAPI GetCritical(
        /* [out] */ Boolean * pCritical);

    CARAPI GetExtnValue(
        /* [out, callee] */ ArrayOf<Byte> ** ppExtnValue);

    CARAPI GetRawExtnValue(
        /* [out, callee] */ ArrayOf<Byte> ** ppRawExtnValue);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI Equals(
        /* [in] */ IInterface * pExt,
        /* [out] */ Boolean * pResult);

    CARAPI GetHashCode(
        /* [out] */ Int32 * pHashCode);

    CARAPI GetDecodedExtensionValue(
        /* [out] */ Org::Apache::Harmony::Security::X509::IExtensionValue ** ppExtensionValue);

    CARAPI GetKeyUsageValue(
        /* [out] */ Org::Apache::Harmony::Security::X509::IKeyUsage ** ppUsageValue);

    CARAPI GetBasicConstraintsValue(
        /* [out] */ Org::Apache::Harmony::Security::X509::IBasicConstraints ** ppBasicConstrainsValue);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI constructor(
        /* [in] */ const String& extnID,
        /* [in] */ Boolean critical,
        /* [in] */ Org::Apache::Harmony::Security::X509::IExtensionValue * pExtnValueObject);

    CARAPI constructor(
        /* [in] */ const String& extnID,
        /* [in] */ Boolean critical,
        /* [in] */ ArrayOf<Byte> * pExtnValue);

    CARAPI constructor(
        /* [in] */ ArrayOf<Int32> * pExtnID,
        /* [in] */ Boolean critical,
        /* [in] */ ArrayOf<Byte> * pExtnValue2);

    CARAPI constructor(
        /* [in] */ const String& extnID,
        /* [in] */ ArrayOf<Byte> * pExtnValue);

    CARAPI constructor(
        /* [in] */ ArrayOf<Int32> * pExtnID,
        /* [in] */ ArrayOf<Byte> * pExtnValue2);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CEXTENSION_H__
