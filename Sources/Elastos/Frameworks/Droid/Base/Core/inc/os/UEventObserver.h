
#ifndef __UEVENTOBSERVER_H__
#define __UEVENTOBSERVER_H__

#include "ext/frameworkext.h"
#include <elastos/ThreadBase.h>
#include <elastos/HashMap.h>
#include <elastos/List.h>
#include <elastos/Mutex.h>

using Elastos::Utility::HashMap;
using Elastos::Utility::List;
using Elastos::Core::Threading::ThreadBase;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * UEventObserver is an abstract class that receives UEvent's from the kernel.<p>
 *
 * Subclass UEventObserver, implementing onUEvent(UEvent event), then call
 * startObserving() with a match string. The UEvent thread will then call your
 * onUEvent() method when a UEvent occurs that contains your match string.<p>
 *
 * Call stopObserving() to stop receiving UEvent's.<p>
 *
 * There is only one UEvent thread per process, even if that process has
 * multiple UEventObserver subclass instances. The UEvent thread starts when
 * the startObserving() is called for the first time in that process. Once
 * started the UEvent thread will not stop (although it can stop notifying
 * UEventObserver's via stopObserving()).<p>
 *
 * @hide
*/
class UEventObserver
    : public IInterface
    , public ElRefBase
{
public:
    /**
     * Representation of a UEvent.
     */
    class UEvent : public ElRefBase
    {
    public:
        UEvent(
            /* [in] */ const String& message);

        CARAPI_(String) Get(
            /* [in] */ const String& key);

        CARAPI_(String) Get(
            /* [in] */ const String& key,
            /* [in] */ const String& defaultValue);

        CARAPI_(String) ToString();

    private:
        // collection of key=value pairs parsed from the uevent message
        HashMap<String, String> mMap;
    };

private:
    class UEventThread
         : public ThreadBase
    {
    public:
        UEventThread();

        //@Override
        virtual CARAPI Run();

        CARAPI_(void) AddObserver(
            /* [in] */ const String& match,
            /* [in] */ UEventObserver* observer);

        /** Removes every key/value pair where value=observer from mObservers */
        CARAPI_(void) RemoveObserver(
            /* [in] */ UEventObserver* observer);

    private:
        CARAPI_(void) SendEvent(
            /* [in] */ const String& message);

    private:
        /** Many to many mapping of string match to observer.
         *  Multimap would be better, but not available in android, so use
         *  an ArrayList where even elements are the String match and odd
         *  elements the corresponding UEventObserver observer */
        List< AutoPtr<IInterface> > mKeysAndObservers;
        Mutex mKeysAndObserversLock;

        List< AutoPtr<UEventObserver> > mTempObserversToSignal;
    };

public:
    virtual ~UEventObserver();

    CAR_INTERFACE_DECL();

    /**
     * Begin observation of UEvent's.<p>
     * This method will cause the UEvent thread to start if this is the first
     * invocation of startObserving in this process.<p>
     * Once called, the UEvent thread will call onUEvent() when an incoming
     * UEvent matches the specified string.<p>
     * This method can be called multiple times to register multiple matches.
     * Only one call to stopObserving is required even with multiple registered
     * matches.
     *
     * @param match A substring of the UEvent to match.  Try to be as specific
     * as possible to aCARAPI_(void) incurring unintended additional cost from processing
     * irrelevant messages.  Netlink messages can be moderately high bandwidth and
     * are expensive to parse.  For example, some devices may send one netlink message
     * for each vsync period.
     */
    CARAPI StartObserving(
        /* [in] */ const String& match);

    /**
     * End observation of UEvent's.<p>
     * This process's UEvent thread will never call onUEvent() on this
     * UEventObserver after this call. Repeated calls have no effect.
     */
    CARAPI_(void) StopObserving();

    /**
     * Subclasses of UEventObserver should override this method to handle
     * UEvents.
     */
    virtual CARAPI_(void) OnUEvent(
        /* [in] */ UEvent* event) = 0;

private:
    static CARAPI NativeSetup();

    static CARAPI_(String) NativeWaitForNextEvent();

    static CARAPI_(void) NativeAddMatch(
        /* [in] */ const String& match);

    static CARAPI_(void) NativeRemoveMatch(
        /* [in] */ const String& match);

    static CARAPI_(AutoPtr<UEventThread>) GetThread();

    static CARAPI_(AutoPtr<UEventThread>) PeekThread();

private:
    static const String TAG;
    static const Boolean DEBUG;

    static AutoPtr<UEventThread> sThread;

    static Mutex sClsLock;

    // static Mutex gMatchesMutex;

    // static Vector<String> gMatches;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__UEVENTOBSERVER_H__
