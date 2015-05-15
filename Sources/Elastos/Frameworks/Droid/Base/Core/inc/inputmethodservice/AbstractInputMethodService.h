
#ifndef  __ABSTRACTINPUTMETHODSERVICE_H__
#define  __ABSTRACTINPUTMETHODSERVICE_H__

#include "app/Service.h"

using Elastos::Droid::InputMethodService::IAbstractInputMethodImpl;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::IDispatcherState;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::InputMethod::IInputMethod;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

class AbstractInputMethodService
    : public Elastos::Droid::App::Service
    , public IKeyEventCallback
{
public:
    AbstractInputMethodService();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Return the global {@link KeyEvent.DispatcherState KeyEvent.DispatcherState}
     * for used for processing events from the target application.
     * Normally you will not need to use this directly, but
     * just use the standard high-level event callbacks like {@link #onKeyDown}.
     */
    CARAPI GetKeyDispatcherState(
        /* [out] */ IDispatcherState** dispatcherState);

    /**
     * Called by the framework during initialization, when the InputMethod
     * interface for this service needs to be created.
     */
    virtual CARAPI OnCreateInputMethodInterface(
        /* [out] */ IAbstractInputMethodImpl** impl) = 0;

    /**
     * Called by the framework when a new InputMethodSession interface is
     * needed for a new client of the input method.
     */
    virtual CARAPI OnCreateInputMethodSessionInterface(
        /* [out] */ IAbstractInputMethodSessionImpl** impl) = 0;

    /**
     * Implement this to handle {@link android.os.Binder#dump Binder.dump()}
     * calls on your input method.
     */
    // protected void dump(FileDescriptor fd, PrintWriter fout, String[] args) {
    // }

    CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

    CARAPI OnTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* state);

    CARAPI OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* state);

public:
    AutoPtr<IDispatcherState> mDispatcherState;

private:
    AutoPtr<IInputMethod> mInputMethod;
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  //__ABSTRACTINPUTMETHODSERVICE_H__