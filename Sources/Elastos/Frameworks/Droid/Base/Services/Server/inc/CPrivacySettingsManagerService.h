
#ifndef __CPRIVACYSETTINGSMANAGERSERVICE_H__
#define __CPRIVACYSETTINGSMANAGERSERVICE_H__

#include "_CPrivacySettingsManagerService.h"

using namespace Elastos::Droid::Privacy;
using namespace Elastos::Droid::Content;

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CPrivacySettingsManagerService)
{
public:
    CPrivacySettingsManagerService();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI GetSettings(
        /* [in] */ const String& packageName,
        /* [out] */ IPrivacySettings** privacySettings);

    CARAPI SaveSettings(
        /* [in] */ IPrivacySettings* settings,
        /* [out] */ Boolean* result);

    CARAPI DeleteSettings(
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* result);

    CARAPI GetVersion(
        /* [out] */ Double* version);

    CARAPI Notification(
        /* [in] */ const String& packageName,
        // /* [in] */ Int32 uid,
        /* [in] */ Byte accessMode,
        /* [in] */ const String& dataType,
        /* [in] */ const String& output);

    CARAPI RegisterObservers();

    CARAPI AddObserver(
        /* [in] */ const String& packageName);

    CARAPI PurgeSettings(
        /* [out] */ Boolean* result);

    CARAPI SetEnabled(
        /* [in]  */ Boolean isEnable,
        /* [out] */ Boolean* rst);

    CARAPI SetNotificationsEnabled(
        /* [in] */  Boolean isEnable,
        /* [out] */ Boolean* rst);

    CARAPI SetBootCompleted();

private:
    static Boolean sSendNotifications;
    static const String TAG;
    AutoPtr<IContext> mContext;
    AutoPtr<IPrivacyPersistenceAdapter> mPersistence;
    Boolean mEnabled;
    Boolean mNotificationEnabled;
    Boolean mBootCompleted;
};

}
}
}
#endif // __CPRIVACYSETTINGSMANAGERSERVICE_H__
