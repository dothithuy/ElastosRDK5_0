#ifndef __CURSORTOBULKCURSORADAPTOR_H__
#define __CURSORTOBULKCURSORADAPTOR_H__

#include "ext/frameworkdef.h"
#include "_CCursorToBulkCursorAdaptor.h"
#include "database/ContentObserver.h"
#include "database/CBulkCursorDescriptor.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Database {

/**
 * Wraps a BulkCursor around an existing Cursor making it remotable.
 *
 * {@hide}
 */
CarClass(CCursorToBulkCursorAdaptor)
{
private:
    class ContentObserverProxy : public ContentObserver
    {
    public:
        ContentObserverProxy(
            /* [in] */ IIContentObserver* remoteObserver,
            /* [in] */ IProxyDeathRecipient* recipient);

        CARAPI UnlinkToDeath(
            /* [in] */ IProxyDeathRecipient* recipient,
            /* [out] */ Boolean* result);

        CARAPI DeliverSelfNotifications(
            /* [out] */ Boolean* result);

        CARAPI OnChangeEx(
            /* [in] */ Boolean selfChange,
            /* [in] */ IUri* uri);

    protected:
        AutoPtr<IIContentObserver> mRemote;
    };

public:
    CARAPI GetBulkCursorDescriptor(
        /* [out] */ CBulkCursorDescriptor** result);

    CARAPI GetWindow(
        /* [in] */ Int32 position,
        /* [out] */ ICursorWindow** result);

    CARAPI OnMove(
        /* [in] */ Int32 position);

    CARAPI Deactivate();

    CARAPI Close();

    CARAPI Requery(
        /* [in] */ IIContentObserver* observer,
        /* [out] */ Int32* result);

    CARAPI GetExtras(
        /* [out] */ IBundle** result);

    CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** result);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ProxyDied();

    CARAPI constructor(
        /* [in] */ ICursor* cursor,
        /* [in] */ IIContentObserver* observer,
        /* [in] */ const String& providerName);

private:
    CARAPI_(void) CloseFilledWindowLocked();

    CARAPI_(void) DisposeLocked();

    CARAPI ThrowIfCursorIsClosed();

    /**
     * Create a ContentObserver from the observer and register it as an observer on the
     * underlying cursor.
     * @param observer the IContentObserver that wants to monitor the cursor
     * @throws IllegalStateException if an observer is already registered
     */
    CARAPI CreateAndRegisterObserverProxyLocked(
        /* [in] */ IIContentObserver* observer);

    /** Unregister the observer if it is already registered. */
    CARAPI UnregisterObserverProxyLocked();

private:
    static const String TAG;
    String mProviderName;
    AutoPtr<ContentObserverProxy> mObserver;

    /**
     * The cursor that is being adapted.
     * This field is set to null when the cursor is closed.
     */
    AutoPtr<ICrossProcessCursor> mCursor;

    /**
     * The cursor window that was filled by the cross process cursor in the
     * case where the cursor does not support getWindow.
     * This field is only ever non-null when the window has actually be filled.
     */
    AutoPtr<ICursorWindow> mFilledWindow;
};

} //Database
} //Droid
} //Elastos

#endif //__CURSORTOBULKCURSORADAPTOR_H__
