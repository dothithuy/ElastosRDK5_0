
#ifndef __CTRUSTEDCERTIFICATESTOREHELPER_H__
#define __CTRUSTEDCERTIFICATESTOREHELPER_H__

#include "_CTrustedCertificateStoreHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CTrustedCertificateStoreHelper)
{
public:
    CARAPI IsSystem(
        /* [in] */ const String& alias,
        /* [out] */ Boolean * pIsSystem);

    CARAPI IsUser(
        /* [in] */ const String& alias,
        /* [out] */ Boolean * pIsUser);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CTRUSTEDCERTIFICATESTOREHELPER_H__
