
#ifndef __CLOADER_H__
#define __CLOADER_H__

#include "_CLoader.h"
#include <ext/frameworkext.h>
#include "Loader.h"

using namespace Elastos;
using namespace Elastos::IO;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CLoader), public Loader
{
public:
    /**
     * Sends the result of the load to the registered listener. Should only be called by subclasses.
     *
     * Must be called from the process's main thread.
     *
     * @param data the result of the load
     */
    CARAPI DeliverResult(
        /* [in] */ IInterface* data);

    /**
     * Informs the registered {@link OnLoadCanceledListener} that the load has been canceled.
     * Should only be called by subclasses.
     *
     * Must be called from the process's main thread.
     */
    CARAPI DeliverCancellation();

    /**
     * @return an application context retrieved from the Context passed to the constructor.
     */
    CARAPI GetContext(
        /* [out] */ IContext** context);

    /**
     * @return the ID of this loader
     */
    CARAPI GetId(
        /* [out] */ Int32* id);

    /**
     * Registers a class that will receive callbacks when a load is complete.
     * The callback will be called on the process's main thread so it's safe to
     * pass the results to widgets.
     *
     * <p>Must be called from the process's main thread.
     */
    CARAPI RegisterListener(
        /* [in] */ Int32 id,
        /* [in] */ ILoaderOnLoadCompleteListener* listener);

    /**
     * Remove a listener that was previously added with {@link #registerListener}.
     *
     * Must be called from the process's main thread.
     */
    CARAPI UnregisterListener(
        /* [in] */ ILoaderOnLoadCompleteListener* listener);

    /**
     * Registers a listener that will receive callbacks when a load is canceled.
     * The callback will be called on the process's main thread so it's safe to
     * pass the results to widgets.
     *
     * Must be called from the process's main thread.
     *
     * @param listener The listener to register.
     */
    CARAPI RegisterOnLoadCanceledListener(
        /* [in] */ ILoaderOnLoadCanceledListener* listener);

    /**
     * Unregisters a listener that was previously added with
     * {@link #registerOnLoadCanceledListener}.
     *
     * Must be called from the process's main thread.
     *
     * @param listener The listener to unregister.
     */
    CARAPI UnregisterOnLoadCanceledListener(
        /* [in] */ ILoaderOnLoadCanceledListener* listener);

    /**
     * Return whether this load has been started.  That is, its {@link #startLoading()}
     * has been called and no calls to {@link #stopLoading()} or
     * {@link #reset()} have yet been made.
     */
    CARAPI IsStarted(
        /* [out] */ Boolean* isStarted);

    /**
     * Return whether this loader has been abandoned.  In this state, the
     * loader <em>must not</em> report any new data, and <em>must</em> keep
     * its last reported data valid until it is finally reset.
     */
    CARAPI IsAbandoned(
        /* [out] */ Boolean* isAbandoned);

    /**
     * Return whether this load has been reset.  That is, either the loader
     * has not yet been started for the first time, or its {@link #reset()}
     * has been called.
     */
    CARAPI IsReset(
        /* [out] */ Boolean* isReset);

    /**
     * This function will normally be called for you automatically by
     * {@link android.app.LoaderManager} when the associated fragment/activity
     * is being started.  When using a Loader with {@link android.app.LoaderManager},
     * you <em>must not</em> call this method yourself, or you will conflict
     * with its management of the Loader.
     *
     * Starts an asynchronous load of the Loader's data. When the result
     * is ready the callbacks will be called on the process's main thread.
     * If a previous load has been completed and is still valid
     * the result may be passed to the callbacks immediately.
     * The loader will monitor the source of
     * the data set and may deliver future callbacks if the source changes.
     * Calling {@link #stopLoading} will stop the delivery of callbacks.
     *
     * <p>This updates the Loader's internal state so that
     * {@link #isStarted()} and {@link #isReset()} will return the correct
     * values, and then calls the implementation's {@link #onStartLoading()}.
     *
     * <p>Must be called from the process's main thread.
     */
    CARAPI StartLoading();

    /**
     * Attempt to cancel the current load task.
     * Must be called on the main thread of the process.
     *
     * <p>Cancellation is not an immediate operation, since the load is performed
     * in a background thread.  If there is currently a load in progress, this
     * method requests that the load be canceled, and notes this is the case;
     * once the background thread has completed its work its remaining state
     * will be cleared.  If another load request comes in during this time,
     * it will be held until the canceled load is complete.
     *
     * @return Returns <tt>false</tt> if the task could not be canceled,
     * typically because it has already completed normally, or
     * because {@link #startLoading()} hasn't been called; returns
     * <tt>true</tt> otherwise.  When <tt>true</tt> is returned, the task
     * is still running and the {@link OnLoadCanceledListener} will be called
     * when the task completes.
     */
    CARAPI CancelLoad(
        /* [out] */ Boolean* succeeded);

    /**
     * Force an asynchronous load. Unlike {@link #startLoading()} this will ignore a previously
     * loaded data set and load a new one.  This simply calls through to the
     * implementation's {@link #onForceLoad()}.  You generally should only call this
     * when the loader is started -- that is, {@link #isStarted()} returns true.
     *
     * <p>Must be called from the process's main thread.
     */
    CARAPI ForceLoad();

    /**
     * This function will normally be called for you automatically by
     * {@link android.app.LoaderManager} when the associated fragment/activity
     * is being stopped.  When using a Loader with {@link android.app.LoaderManager},
     * you <em>must not</em> call this method yourself, or you will conflict
     * with its management of the Loader.
     *
     * <p>Stops delivery of updates until the next time {@link #startLoading()} is called.
     * Implementations should <em>not</em> invalidate their data at this point --
     * clients are still free to use the last data the loader reported.  They will,
     * however, typically stop reporting new data if the data changes; they can
     * still monitor for changes, but must not report them to the client until and
     * if {@link #startLoading()} is later called.
     *
     * <p>This updates the Loader's internal state so that
     * {@link #isStarted()} will return the correct
     * value, and then calls the implementation's {@link #onStopLoading()}.
     *
     * <p>Must be called from the process's main thread.
     */
    CARAPI StopLoading();

    /**
     * This function will normally be called for you automatically by
     * {@link android.app.LoaderManager} when restarting a Loader.  When using
     * a Loader with {@link android.app.LoaderManager},
     * you <em>must not</em> call this method yourself, or you will conflict
     * with its management of the Loader.
     *
     * Tell the Loader that it is being abandoned.  This is called prior
     * to {@link #reset} to have it retain its current data but not report
     * any new data.
     */
    CARAPI Abandon();

    /**
     * This function will normally be called for you automatically by
     * {@link android.app.LoaderManager} when destroying a Loader.  When using
     * a Loader with {@link android.app.LoaderManager},
     * you <em>must not</em> call this method yourself, or you will conflict
     * with its management of the Loader.
     *
     * Resets the state of the Loader.  The Loader should at this point free
     * all of its resources, since it may never be called again; however, its
     * {@link #startLoading()} may later be called at which point it must be
     * able to start running again.
     *
     * <p>This updates the Loader's internal state so that
     * {@link #isStarted()} and {@link #isReset()} will return the correct
     * values, and then calls the implementation's {@link #onReset()}.
     *
     * <p>Must be called from the process's main thread.
     */
    CARAPI Reset();

    /**
     * Take the current flag indicating whether the loader's content had
     * changed while it was stopped.  If it had, true is returned and the
     * flag is cleared.
     */
    CARAPI TakeContentChanged(
        /* [out] */ Boolean* succeeded);

    /**
     * Called when {@link ForceLoadContentObserver} detects a change.  The
     * default implementation checks to see if the loader is currently started;
     * if so, it simply calls {@link #forceLoad()}; otherwise, it sets a flag
     * so that {@link #takeContentChanged()} returns true.
     *
     * <p>Must be called from the process's main thread.
     */
    CARAPI OnContentChanged();

    /**
     * For debugging, converts an instance of the Loader's data class to
     * a string that can be printed.  Must handle a null data.
     */
    CARAPI DataToString(
        /* [in] */ IInterface* data,
        /* [out] */ String* str);

    CARAPI ToString(
        /* [out] */ String* str);

    /**
     * Print the Loader's state into the given stream.
     *
     * @param prefix Text to print at the front of each line.
     * @param fd The raw file descriptor that the dump is being sent to.
     * @param writer A PrintWriter to which the dump is to be set.
     * @param args Additional arguments to the dump request.
     */
    CARAPI Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ ArrayOf<String>* args);

    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __CLOADER_H__