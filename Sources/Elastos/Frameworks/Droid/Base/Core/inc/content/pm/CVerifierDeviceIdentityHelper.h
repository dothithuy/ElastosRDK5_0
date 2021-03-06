
#ifndef __CVERIFIERDEVICEIDENTITYHELPER_H__
#define __CVERIFIERDEVICEIDENTITYHELPER_H__

#include "_CVerifierDeviceIdentityHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CVerifierDeviceIdentityHelper)
{
public:
    /**
     * Generate a new device identity.
     *
     * @return random uniformly-distributed device identity
     */
    CARAPI Generate(
        /* [out] */ Elastos::Droid::Content::Pm::IVerifierDeviceIdentity** identity);

    CARAPI Parse(
        /* [in] */ const String& deviceIdentity,
        /* [out] */ Elastos::Droid::Content::Pm::IVerifierDeviceIdentity** identity);

private:
    /**
     * Generate a new device identity using a provided random number generator
     * class. This is used for testing.
     *
     * @param rng random number generator to retrieve the next long from
     * @return verifier device identity based on the input from the provided
     *         random number generator
     */
    static CARAPI Generate(
        /* [in] */ Elastos::Security::ISecureRandom* rng,
        /* [out] */ Elastos::Droid::Content::Pm::IVerifierDeviceIdentity** identity);

};

}
}
}
}

#endif // __CVERIFIERDEVICEIDENTITYHELPER_H__
