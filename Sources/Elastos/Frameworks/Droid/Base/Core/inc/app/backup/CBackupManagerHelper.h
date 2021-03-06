#ifndef __CBACKUPMANAGERHELPER_H__
#define __CBACKUPMANAGERHELPER_H__

#include "_CBackupManagerHelper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace App {
namespace Backup {

CarClass(CBackupManagerHelper)
{
public:
    /**
     * Convenience method for callers who need to indicate that some other package
     * needs a backup pass.  This can be useful in the case of groups of packages
     * that share a uid.
     * <p>
     * This method requires that the application hold the "android.permission.BACKUP"
     * permission if the package named in the argument does not run under the same uid
     * as the caller.
     *
     * @param packageName The package name identifying the application to back up.
     */
    CARAPI DataChanged(
        /* [in] */ const String& packageName);

};

}
}
}
}

#endif // __CBACKUPMANAGERHELPER_H__
