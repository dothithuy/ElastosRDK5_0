#ifndef __ELASTOS_DROID_OS_LOOPER_H__
#define __ELASTOS_DROID_OS_LOOPER_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include <elastos/Mutex.h>
#include <pthread.h>

using Elastos::Core::Threading::IThread;
using Elastos::Core::Threading::Mutex;
using Elastos::Droid::Utility::IPrinter;
using Elastos::Droid::Os::IMessageQueue;

namespace Elastos {
namespace Droid {
namespace Os {

class Looper
    : public ElRefBase
    , public ILooper
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* pObject,
        /* [in] */ InterfaceID* pIID);

    /** Initialize the current thread as a looper.
      * This gives you a chance to create handlers that then reference
      * this looper, before actually starting the loop. Be sure to call
      * {@link #loop()} after calling this method, and end it by calling
      * {@link #quit()}.
      */
    static CARAPI Prepare();

    /**
     * Initialize the current thread as a looper, marking it as an
     * application's main looper. The main looper for your application
     * is created by the Android environment, so you should never need
     * to call this function yourself.  See also: {@link #prepare()}
     */
    static CARAPI PrepareMainLooper();

    /** Returns the application's main looper, which lives in the main thread of the application.
     */
    static CARAPI_(AutoPtr<ILooper>) GetMainLooper();

    /**
     * Run the message queue in this thread. Be sure to call
     * {@link #quit()} to end the loop.
     */
    static CARAPI Loop();

    /**
     * Return the Looper object associated with the current thread.  Returns
     * null if the calling thread is not associated with a Looper.
     */
    static CARAPI_(AutoPtr<ILooper>) MyLooper();

    /**
     * Control logging of messages as they are processed by this Looper.  If
     * enabled, a log message will be written to <var>printer</var>
     * at the beginning and ending of each message dispatch, identifying the
     * target Handler and message contents.
     *
     * @param printer A Printer object that will receive log messages, or
     * null to disable message logging.
     */
    CARAPI SetCallbackLogging(
        /* [in] */ IPrinter* printer);

    /**
     * Return the {@link MessageQueue} object associated with the current
     * thread.  This must be called from a thread running a Looper, or a
     * NullPointerException will be thrown.
     */
    static CARAPI_(AutoPtr<IMessageQueue>) MyQueue();

    /**
     * Quits the looper.
     *
     * Causes the {@link #loop} method to terminate as soon as possible.
     */
    CARAPI Quit();

    /**
     * Posts a synchronization barrier to the Looper's message queue.
     *
     * Message processing occurs as usual until the message queue encounters the
     * synchronization barrier that has been posted.  When the barrier is encountered,
     * later synchronous messages in the queue are stalled (prevented from being executed)
     * until the barrier is released by calling {@link #removeSyncBarrier} and specifying
     * the token that identifies the synchronization barrier.
     *
     * This method is used to immediately postpone execution of all subsequently posted
     * synchronous messages until a condition is met that releases the barrier.
     * Asynchronous messages (see {@link Message#isAsynchronous} are exempt from the barrier
     * and continue to be processed as usual.
     *
     * This call must be always matched by a call to {@link #removeSyncBarrier} with
     * the same token to ensure that the message queue resumes normal operation.
     * Otherwise the application will probably hang!
     *
     * @return A token that uniquely identifies the barrier.  This token must be
     * passed to {@link #removeSyncBarrier} to release the barrier.
     *
     * @hide
     */
    CARAPI PostSyncBarrier(
        /* [out] */ Int32* token);

    /**
     * Removes a synchronization barrier.
     *
     * @param token The synchronization barrier token that was returned by
     * {@link #postSyncBarrier}.
     *
     * @throws IllegalStateException if the barrier was not found.
     *
     * @hide
     */
    CARAPI RemoveSyncBarrier(
        /* [in] */ Int32 token);

    /**
     * Return the Thread associated with this Looper.
     */
    CARAPI GetThread(
        /* [out] */ IThread** thread);

    /** @hide */
    CARAPI_(AutoPtr<IMessageQueue>) GetQueue();

    CARAPI GetQueue(
        /* [out] */ IMessageQueue** queue);

private:
    Looper(
        /* [in] */ Boolean quitAllowed);

    static CARAPI Prepare(
        /* [in] */ Boolean quitAllowed);

    static CARAPI_(void) InitTLS();

public:
    AutoPtr<IMessageQueue> mQueue;
    AutoPtr<IThread> mThread;
    Boolean mRun;

private:
    static AutoPtr<ILooper> sMainLooper;  // guarded by Looper.class

    AutoPtr<IPrinter> mLogging;

    static Mutex sClassLock;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_LOOPER_H__
