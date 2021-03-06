
#ifdef DROID_CORE
#include "ext/frameworkdef.h"
#include "inputmethodservice/AbstractInputMethodService.h"
#include "inputmethodservice/CIInputMethodWrapper.h"
#include "view/CDispatcherState.h"
#else
#include "inputmethodservice/AbstractInputMethodService.h"
#endif

using Elastos::Droid::View::CDispatcherState;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::Internal::View::IIInputMethod;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

AbstractInputMethodService::AbstractInputMethodService()
{
    ASSERT_TRUE(CDispatcherState::New((IDispatcherState**)&mDispatcherState) == NOERROR);
}

PInterface AbstractInputMethodService::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IKeyEventCallback) {
       return (IKeyEventCallback*)this;
    }
    else return Service::Probe(riid);
}

ECode AbstractInputMethodService::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    if (pObject == (IInterface*)(IKeyEventCallback*)this) {
        *pIID = EIID_IKeyEventCallback;
        return NOERROR;
    }
    else return Service::GetInterfaceID(pObject, pIID);
}

ECode AbstractInputMethodService::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** dispatcherState)
{
    assert(dispatcherState != NULL);
    *dispatcherState = mDispatcherState;
    INTERFACE_ADDREF(*dispatcherState);

    return NOERROR;
}

ECode AbstractInputMethodService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    assert(binder != NULL);

    if (mInputMethod == NULL) {
        AutoPtr<IAbstractInputMethodImpl> mtdImpl;
        OnCreateInputMethodInterface((IAbstractInputMethodImpl**)&mtdImpl);
        mInputMethod = IInputMethod::Probe(mtdImpl.Get());
    }

    AutoPtr<IIInputMethod> method;
    CIInputMethodWrapper::New(THIS_PROBE(IAbstractInputMethodService), mInputMethod, (IIInputMethod**)&method);
    *binder = IBinder::Probe(method);
    INTERFACE_ADDREF(*binder)
    return NOERROR;
}

ECode AbstractInputMethodService::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = FALSE;

    return NOERROR;
}

ECode AbstractInputMethodService::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = FALSE;

    return NOERROR;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
