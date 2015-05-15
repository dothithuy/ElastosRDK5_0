#ifndef __CONTENTOBSERVER_H__
#define __CONTENTOBSERVER_H__

#include "ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Database {
/**
 * Receives call backs for changes to content. Must be implemented by objects which are added
 * to a {@link ContentObservable}.
 */
class ContentObserver
    : public ElRefBase
    , public IContentObserver
{
private:
    class NotificationRunnable : public Runnable
    {
    public:
        NotificationRunnable(
            /* [in] */ Boolean selfChange,
            /* [in] */ IUri* uri,
            /* [in] */ ContentObserver* owner);

        CARAPI Run();

    private:
        Boolean mSelfChange;
        AutoPtr<IUri> mUri;
        ContentObserver* mOwner;
    };

public:
    /**
     * Creates a content observer.
     *
     * @param handler The handler to run {@link #onChange} on, or null if none.
     */
    ContentObserver(
        /* [in]  */ IHandler* handler);

    CAR_INTERFACE_DECL()

    /**
     * Gets access to the binder transport object. Not for public consumption.
     *
     * {@hide}
     */
    virtual CARAPI GetContentObserver(
        /* [out] */ IIContentObserver** contentObserver);

    /**
     * Gets access to the binder transport object, and unlinks the transport object
     * from the ContentObserver. Not for public consumption.
     *
     * {@hide}
     */
    virtual CARAPI ReleaseContentObserver(
        /* [out] */ IIContentObserver** contentObserver);

    /**
     * Returns true if this observer is interested receiving self-change notifications.
     *
     * Subclasses should override this method to indicate whether the observer
     * is interested in receiving notifications for changes that it made to the
     * content itself.
     *
     * @return True if self-change notifications should be delivered to the observer.
     */
    virtual CARAPI DeliverSelfNotifications(
        /* [out] */ Boolean* res);

    /**
     * This method is called when a content change occurs.
     * <p>
     * Subclasses should override this method to handle content changes.
     * </p>
     *
     * @param selfChange True if this is a self-change notification.
     */
    virtual CARAPI OnChange(
        /* [in] */ Boolean selfChange);

    /**
     * This method is called when a content change occurs.
     * Includes the changed content Uri when available.
     * <p>
     * Subclasses should override this method to handle content changes.
     * To ensure correct operation on older versions of the framework that
     * did not provide a Uri argument, applications should also implement
     * the {@link #onChange(boolean)} overload of this method whenever they
     * implement the {@link #onChange(boolean, Uri)} overload.
     * </p><p>
     * Example implementation:
     * <pre><code>
     * // Implement the onChange(boolean) method to delegate the change notification to
     * // the onChange(boolean, Uri) method to ensure correct operation on older versions
     * // of the framework that did not have the onChange(boolean, Uri) method.
     * {@literal @Override}
     * public void onChange(boolean selfChange) {
     *     onChange(selfChange, null);
     * }
     *
     * // Implement the onChange(boolean, Uri) method to take advantage of the new Uri argument.
     * {@literal @Override}
     * public void onChange(boolean selfChange, Uri uri) {
     *     // Handle change.
     * }
     * </code></pre>
     * </p>
     *
     * @param selfChange True if this is a self-change notification.
     * @param uri The Uri of the changed content, or null if unknown.
     */
    virtual CARAPI OnChangeEx(
        /* [in] */ Boolean selfChange,
        /* [in] */ IUri* uri);

    /**
     * Dispatches a change notification to the observer.
     * <p>
     * If a {@link Handler} was supplied to the {@link ContentObserver} constructor,
     * then a call to the {@link #onChange} method is posted to the handler's message queue.
     * Otherwise, the {@link #onChange} method is invoked immediately on this thread.
     * </p>
     *
     * @param selfChange True if this is a self-change notification.
     *
     * @deprecated Use {@link #dispatchChange(boolean, Uri)} instead.
     */
    //@Deprecated
    virtual CARAPI DispatchChange(
        /* [in] */ Boolean selfChange);

    /**
     * Dispatches a change notification to the observer.
     * Includes the changed content Uri when available.
     * <p>
     * If a {@link Handler} was supplied to the {@link ContentObserver} constructor,
     * then a call to the {@link #onChange} method is posted to the handler's message queue.
     * Otherwise, the {@link #onChange} method is invoked immediately on this thread.
     * </p>
     *
     * @param selfChange True if this is a self-change notification.
     * @param uri The Uri of the changed content, or null if unknown.
     */
    virtual CARAPI DispatchChangeEx(
        /* [in] */ Boolean selfChange,
        /* [in] */ IUri* uri);

protected:
    ContentObserver() {}

    CARAPI Init(
        /* [in]  */ IHandler* handler);

protected:
    Mutex mLock;
    AutoPtr<IContentObserverTransport> mTransport;
    AutoPtr<IHandler> mHandler;
};

} //Database
} //Droid
} //Elastos

#endif //__CONTENTOBSERVER_H__