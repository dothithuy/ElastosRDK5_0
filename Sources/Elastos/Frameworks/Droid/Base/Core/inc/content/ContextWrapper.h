
#ifndef __CONTEXTWRAPPER_H__
#define __CONTEXTWRAPPER_H__

#include "ext/frameworkdef.h"
#include "Context.h"

using Elastos::Core::IClassLoader;
using namespace Elastos::IO;
using namespace Elastos::Droid::Database;
using namespace Elastos::Droid::Graphics;
using namespace Elastos::Droid::View;
using namespace Elastos::Droid::Os;
using namespace Elastos::Droid::Utility;
using namespace Elastos::Droid::Content::Pm;
using namespace Elastos::Droid::Content::Res;
using namespace Elastos::Droid::Database::Sqlite;
using namespace Elastos::Droid::Graphics::Drawable;

namespace Elastos {
namespace Droid {
namespace Content {

class ContextWrapper: public Context
{

public:
    ContextWrapper();

    ContextWrapper(
        /* [in] */ IContext* base);

    ~ContextWrapper();

    /**
     * @return the base context as set by the constructor or setBaseContext
     */
    CARAPI GetBaseContext(
        /* [out] */ IContext** context);

    CARAPI GetAssets(
        /* [out] */ IAssetManager** assetManager);

    CARAPI GetResources(
        /* [out] */ IResources** resources);

    CARAPI GetPackageManager(
        /* [out] */ IPackageManager** packageManager);

    CARAPI GetContentResolver(
        /* [out] */ IContentResolver** resolver);

    CARAPI GetMainLooper(
        /* [out] */ ILooper** looper);

    CARAPI GetApplicationContext(
        /* [out] */ IContext** ctx);

    CARAPI SetTheme(
        /* [in] */ Int32 resId);

    CARAPI GetThemeResId(
        /* [out] */ Int32* resId);

    CARAPI GetTheme(
        /* [out] */ IResourcesTheme** theme);

    CARAPI GetClassLoader(
        /* [out] */ IClassLoader** loader);

    CARAPI GetPackageName(
        /* [out] */ String* packageName);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI GetPackageResourcePath(
        /* [out] */ String* path);

    CARAPI GetPackageCodePath(
        /* [out] */ String* codePath);

    CARAPI GetSharedPrefsFile(
        /* [in] */ const String& name,
        /* [out] */ IFile** file);

    CARAPI GetSharedPreferences(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ ISharedPreferences** prefs);

    CARAPI OpenFileInput(
        /* [in] */ const String& name,
        /* [out] */ IFileInputStream** fileInputStream);

    CARAPI OpenFileOutput(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ IFileOutputStream** fileOutputStream);

    CARAPI DeleteFile(
        /* [in] */ const String& name,
        /* [out] */ Boolean* succeeded);

    CARAPI GetFileStreamPath(
        /* [in] */ const String& name,
        /* [out] */ IFile** file);

    CARAPI GetFilesDir(
        /* [out] */ IFile** filesDir);

    CARAPI GetExternalFilesDir(
        /* [in] */ const String& type,
        /* [out] */ IFile** filesDir);

    CARAPI GetObbDir(
        /* [out] */ IFile** obbDir);

    CARAPI GetCacheDir(
        /* [out] */ IFile** cacheDir);

    CARAPI GetExternalCacheDir(
        /* [out] */ IFile** externalDir);

    CARAPI GetFileList(
        /* [out, callee] */ ArrayOf<String>** fileList);

    CARAPI GetDir(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ IFile** dir);

    CARAPI OpenOrCreateDatabase(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [out] */ ISQLiteDatabase** sqliteDB);

    CARAPI OpenOrCreateDatabaseEx(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [in] */ IDatabaseErrorHandler* errorHandler,
        /* [out] */ ISQLiteDatabase** sqliteDB);

    CARAPI DeleteDatabase(
        /* [in] */ const String& name,
        /* [out] */ Boolean* succeeded);

    CARAPI GetDatabasePath(
        /* [in] */ const String& name,
        /* [out] */ IFile** path);

    CARAPI GetDatabaseList(
        /* [out, callee] */ ArrayOf<String>** databaseList);

    CARAPI GetWallpaper(
        /* [out] */ IDrawable** drawable);

    CARAPI PeekWallpaper(
        /* [out] */ IDrawable** drawable);

    CARAPI GetWallpaperDesiredMinimumWidth(
        /* [out] */ Int32* minWidth);

    CARAPI GetWallpaperDesiredMinimumHeight(
        /* [out] */ Int32* minHeight);

    CARAPI SetWallpaper(
        /* [in] */ IBitmap* bitmap);

    CARAPI SetWallpaperEx(
        /* [in] */ IInputStream* data);

    CARAPI ClearWallpaper();

    CARAPI StartActivity(
        /* [in] */ IIntent* intent);

    CARAPI StartActivityAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI StartActivityEx(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options);

    CARAPI StartActivityAsUserEx(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options,
        /* [in] */ IUserHandle* userId);

    CARAPI StartActivities(
        /* [in] */ ArrayOf<IIntent*>* intents);

    CARAPI StartActivitiesEx(
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ IBundle* options);

    CARAPI StartActivitiesAsUser(
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ IBundle* options,
        /* [in] */ IUserHandle* userHandle);

    CARAPI StartIntentSender(
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags);

    CARAPI StartIntentSenderEx(
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags,
        /* [in] */ IBundle* options);

    CARAPI SendBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI SendBroadcastEx(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission);

    CARAPI SendOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission);

    CARAPI SendOrderedBroadcastEx(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI SendBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI SendBroadcastAsUserEx(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& receiverPermission);

    CARAPI SendOrderedBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& receiverPermission,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI SendStickyBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI SendStickyOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI RemoveStickyBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI SendStickyBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI SendStickyOrderedBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI RemoveStickyBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI RegisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [out] */ IIntent** stickyIntent);

    CARAPI RegisterReceiverEx(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [out] */ IIntent** stickyIntent);

    CARAPI RegisterReceiverAsUser(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IUserHandle* user,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [out] */ IIntent** stickyIntent);

    CARAPI UnregisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver);

    CARAPI StartService(
        /* [in] */ IIntent* service,
        /* [out] */ IComponentName** name);

    CARAPI StopService(
        /* [in] */ IIntent* service,
        /* [out] */ Boolean* succeeded);

    CARAPI StartServiceAsUser(
        /* [in] */ IIntent* service,
        /* [in] */ IUserHandle* user,
        /* [out] */ IComponentName** name);

    CARAPI StopServiceAsUser(
        /* [in] */ IIntent* service,
        /* [in] */ IUserHandle* user,
        /* [out] */ Boolean* succeeded);

    CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    CARAPI BindServiceEx(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userHandle,
        /* [out] */ Boolean* succeeded);

    CARAPI UnbindService(
        /* [in] */ IServiceConnection* conn);

    CARAPI StartInstrumentation(
        /* [in] */ IComponentName* className,
        /* [in] */ const String& profileFile,
        /* [in] */ IBundle* arguments,
        /* [out] */ Boolean* succeeded);

    CARAPI GetSystemService(
        /* [in] */ const String& name,
        /* [out] */ IInterface** object);

    CARAPI CheckPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Int32* permissionId);

    CARAPI CheckCallingPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* permissionId);

    CARAPI CheckCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* permissionId);

    CARAPI EnforcePermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ const String& message);

    CARAPI EnforceCallingPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& message);

    CARAPI EnforceCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& message);

    CARAPI GrantUriPermission(
        /* [in] */ const String& toPackage,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI RevokeUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI CheckUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* permissionId);

    CARAPI CheckCallingUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* permissionId);

    CARAPI CheckCallingOrSelfUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* permissionId);

    CARAPI CheckUriPermissionEx(
        /* [in] */ IUri* uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* permissionId);

    CARAPI EnforceUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI EnforceCallingUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI EnforceCallingOrSelfUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI EnforceUriPermissionEx(
        /* [in] */ IUri* uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI CreatePackageContext(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [out] */ IContext** ctx);

    CARAPI CreatePackageContextAsUser(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [in] */ IUserHandle* user,
        /* [out] */ IContext** ctx);

    CARAPI CreateConfigurationContext(
        /* [in] */ IConfiguration* overrideConfiguration,
        /* [out] */ IContext** ctx);

    CARAPI CreateDisplayContext(
        /* [in] */ IDisplay* display,
        /* [out] */ IContext** ctx);

    CARAPI GetCompatibilityInfo(
        /* [in] */ Int32 displayId,
        /* [out] */ ICompatibilityInfoHolder** infoHolder);

    CARAPI IsRestricted(
        /* [out] */ Boolean* isRestricted);

protected:
    CARAPI Init(
        /* [in] */ IContext* context);

    /**
     * Set the base context for this ContextWrapper.  All calls will then be
     * delegated to the base context.  Throws
     * IllegalStateException if a base context has already been set.
     *
     * @param base The new base context for this wrapper.
     */
    CARAPI AttachBaseContext(
        /* [in] */ IContext* base);

protected:
    AutoPtr<IContext> mBase;

};

}
}
}

#endif // __CONTEXTWRAPPER_H__
