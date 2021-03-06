#ifndef __CINPUTMETHODMANAGERSERVICEUSERSWITCHOBSERVER_H__
#define __CINPUTMETHODMANAGERSERVICEUSERSWITCHOBSERVER_H__

#include "_CInputMethodManagerServiceUserSwitchObserver.h"
#include "CInputMethodManagerService.h"

using Elastos::Droid::Os::IRemoteCallback;

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CInputMethodManagerServiceUserSwitchObserver)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 inputMethodManagerService);

    CARAPI OnUserSwitching(
        /* [in] */ Int32 newUserId,
        /* [in] */ IRemoteCallback* reply);

    CARAPI OnUserSwitchComplete(
        /* [in] */ Int32 newUserId);

private:
    CInputMethodManagerService* mHost;
};

} // namespace Elastos;
} // namespace Droid;
} // namespace Server;
#endif