
#include "inputmethodservice/CIInputMethodWrapper.h"
#include "inputmethodservice/CIInputMethodSessionWrapper.h"
#include "view/LocalInputConnectionWrapper.h"
#include "view/inputmethod/CInputBinding.h"
#include "os/SomeArgs.h"
#include "os/HandlerCaller.h"
#include <elastos/Logger.h>
#include <stdio.h>
#include <unistd.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::InputMethodService::CIInputMethodSessionWrapper;
using Elastos::Droid::Internal::View::LocalInputConnectionWrapper;
using Elastos::Droid::Os::EIID_IHandlerCallerCallback;
using Elastos::Droid::Os::SomeArgs;
using Elastos::Droid::Os::HandlerCaller;
using Elastos::Droid::Internal::View::IIInputMethodSession;
using Elastos::Droid::View::InputMethod::IInputMethod;
using Elastos::Droid::View::InputMethod::IInputConnection;
using Elastos::Droid::View::InputMethod::CInputBinding;
using Elastos::Droid::View::InputMethod::EIID_IInputMethodSessionEventCallback;
using Elastos::Droid::View::InputMethod::EIID_IInputMethod;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

const String CIInputMethodWrapper::TAG("CIInputMethodWrapper");
const Boolean CIInputMethodWrapper::DEBUG = FALSE;
const Int32 CIInputMethodWrapper::DO_DUMP = 1;
const Int32 CIInputMethodWrapper::DO_ATTACH_TOKEN = 10;
const Int32 CIInputMethodWrapper::DO_SET_INPUT_CONTEXT = 20;
const Int32 CIInputMethodWrapper::DO_UNSET_INPUT_CONTEXT = 30;
const Int32 CIInputMethodWrapper::DO_START_INPUT = 32;
const Int32 CIInputMethodWrapper::DO_RESTART_INPUT = 34;
const Int32 CIInputMethodWrapper::DO_CREATE_SESSION = 40;
const Int32 CIInputMethodWrapper::DO_SET_SESSION_ENABLED = 45;
const Int32 CIInputMethodWrapper::DO_REVOKE_SESSION = 50;
const Int32 CIInputMethodWrapper::DO_SHOW_SOFT_INPUT = 60;
const Int32 CIInputMethodWrapper::DO_HIDE_SOFT_INPUT = 70;
const Int32 CIInputMethodWrapper::DO_CHANGE_INPUTMETHOD_SUBTYPE = 80;


CIInputMethodWrapper::InputMethodSessionCallbackWrapper::InputMethodSessionCallbackWrapper(
    /* [in] */ IContext* context,
    /* [in] */ IInputMethodCallback* cb)
    : mContext(context)
    , mCb(cb)
{}

PInterface CIInputMethodWrapper::InputMethodSessionCallbackWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInputMethodSessionEventCallback == riid) {
        return (IInputMethodSessionEventCallback *)this;
    }

    return NULL;
}

UInt32 CIInputMethodWrapper::InputMethodSessionCallbackWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CIInputMethodWrapper::InputMethodSessionCallbackWrapper::Release()
{
    return ElRefBase::Release();
}

ECode CIInputMethodWrapper::InputMethodSessionCallbackWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IInputMethodSessionEventCallback*)this) {
        *pIID = EIID_IInputMethodSessionEventCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CIInputMethodWrapper::InputMethodSessionCallbackWrapper::SessionCreated(
    /* [in] */ IInputMethodSession* session)
{
    // try {
    if (session != NULL) {
        AutoPtr<IIInputMethodSession> wrap;
        CIInputMethodSessionWrapper::New(mContext, session, (IIInputMethodSession**)&wrap);
        return mCb->SessionCreated(wrap);
    }
    else {
        return mCb->SessionCreated(NULL);
    }
    // } catch (RemoteException e) {
    // }
}

ECode CIInputMethodWrapper::constructor(
    /* [in] */ IAbstractInputMethodService* context,
    /* [in] */ IInputMethod* inputMethod)
{
    AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(context);
    if (wrs) wrs->GetWeakReference((IWeakReference**)&mTarget);
    wrs = IWeakReferenceSource::Probe(inputMethod);
    if (wrs) wrs->GetWeakReference((IWeakReference**)&mInputMethod);

    context->GetApplicationContext((IContext**)&mContext);
    mCaller = new HandlerCaller(mContext, THIS_PROBE(IHandlerCallerCallback), FALSE);
    AutoPtr<IApplicationInfo> appInfo;
    context->GetApplicationInfo((IApplicationInfo**)&appInfo);
    assert(appInfo != NULL);
    appInfo->GetTargetSdkVersion(&mTargetSdkVersion);
    return NOERROR;
}

AutoPtr<IInputMethod> CIInputMethodWrapper::GetInternalInputMethod()
{
    AutoPtr<IInputMethod> im;
    if (mInputMethod) {
        AutoPtr<IInterface> obj;
        mInputMethod->Resolve(EIID_IInputMethod, (IInterface**)&obj);
        if (obj) {
            im = IInputMethod::Probe(obj);
        }
    }

    return im;
}

ECode CIInputMethodWrapper::ExecuteMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    AutoPtr<IInputMethod> inputMethod = GetInternalInputMethod();
    // Need a valid reference to the inputMethod for everything except a dump.
    if (inputMethod == NULL && what != DO_DUMP) {
        Logger::W(TAG, "Input method reference was NULL, ignoring message: %d", what);
        return  NOERROR;
    }

    switch (what) {
        case DO_DUMP: {
            AutoPtr<IAbstractInputMethodService> target;
            AutoPtr<IInterface> obj;
            mTarget->Resolve(EIID_IAbstractInputMethodService, (IInterface**)&obj);
            if (obj) {
                target = IAbstractInputMethodService::Probe(obj);
            }
            if (target == NULL) {
                return NOERROR;
            }
            SomeArgs* args = (SomeArgs*)obj.Get();
            // try {
            //     target.dump((FileDescriptor)args->mArg1,
            //             (PrintWriter)args->mArg2, (String[])args->mArg3);
            // } catch (RuntimeException e) {
            //     ((PrintWriter)args->mArg2).println("Exception: " + e);
            // }
            // synchronized (args->mArg4) {
            //     ((CountDownLatch)args->mArg4).countDown();
            // }
            args->Recycle();
            return NOERROR;
        }

        case DO_ATTACH_TOKEN: {
            inputMethod->AttachToken(IBinder::Probe(obj));
            return NOERROR;
        }
        case DO_SET_INPUT_CONTEXT: {
            inputMethod->BindInput(IInputBinding::Probe(obj));
            return NOERROR;
        }
        case DO_UNSET_INPUT_CONTEXT:
            inputMethod->UnbindInput();
            return NOERROR;
        case DO_START_INPUT: {
            SomeArgs* args = (SomeArgs*)obj.Get();
            IInputContext* inputContext = IInputContext::Probe(args->mArg1);
            AutoPtr<IInputConnection> ic = inputContext != NULL
                    ? new LocalInputConnectionWrapper(inputContext) : NULL;
            IEditorInfo* info = IEditorInfo::Probe(args->mArg2);
            info->MakeCompatible(mTargetSdkVersion);
            inputMethod->StartInput(ic, info);
            args->Recycle();
            return NOERROR;
        }
        case DO_RESTART_INPUT: {
            SomeArgs* args = (SomeArgs*)obj.Get();
            IInputContext* inputContext = IInputContext::Probe(args->mArg1);
            AutoPtr<IInputConnection> ic = inputContext != NULL
                    ? new LocalInputConnectionWrapper(inputContext) : NULL;
            IEditorInfo* info = IEditorInfo::Probe(args->mArg2);
            info->MakeCompatible(mTargetSdkVersion);
            inputMethod->RestartInput(ic, info);
            args->Recycle();
            return NOERROR;
        }
        case DO_CREATE_SESSION: {
            AutoPtr<IContext> context;
            mCaller->GetContext((IContext**)&context);
            AutoPtr<IInputMethodSessionEventCallback> cb = new InputMethodSessionCallbackWrapper(
                    context, IInputMethodCallback::Probe(obj));
            inputMethod->CreateSession(cb);
            return NOERROR;
        }
        case DO_SET_SESSION_ENABLED:
            inputMethod->SetSessionEnabled(IInputMethodSession::Probe(obj), arg1 != 0);
            return NOERROR;
        case DO_REVOKE_SESSION:
            inputMethod->RevokeSession(IInputMethodSession::Probe(obj));
            return NOERROR;
        case DO_SHOW_SOFT_INPUT:
            inputMethod->ShowSoftInput(arg1, IResultReceiver::Probe(obj));
            return NOERROR;
        case DO_HIDE_SOFT_INPUT:
            inputMethod->HideSoftInput(arg1, IResultReceiver::Probe(obj));
            return NOERROR;
        case DO_CHANGE_INPUTMETHOD_SUBTYPE:
            inputMethod->ChangeInputMethodSubtype(IInputMethodSubtype::Probe(obj));
            return NOERROR;
    }

    Logger::W(TAG, "Unhandled message code: %d", what);
    return NOERROR;
}

ECode CIInputMethodWrapper::AttachToken(
    /* [in] */ IBinder* token)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageO(DO_ATTACH_TOKEN, token, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::BindInput(
    /* [in] */ IInputBinding* binding)
{
    AutoPtr<IBinder> binder;
    FAIL_RETURN(binding->GetConnectionToken((IBinder**)&binder));
    AutoPtr<IInputConnection> ic = new LocalInputConnectionWrapper(IInputContext::Probe(binder));
    AutoPtr<IInputBinding> nu;
    FAIL_RETURN(CInputBinding::New(ic, binding, (IInputBinding**)&nu));

    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageO(DO_SET_INPUT_CONTEXT, nu, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::UnbindInput()
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_UNSET_INPUT_CONTEXT, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::StartInput(
    /* [in] */ IInputContext* inputContext,
    /* [in] */ IEditorInfo* attribute)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageOO(DO_START_INPUT, inputContext, attribute, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::RestartInput(
    /* [in] */ IInputContext* inputContext,
    /* [in] */ IEditorInfo* attribute)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageOO(DO_RESTART_INPUT, inputContext, attribute, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::CreateSession(
    /* [in] */ IInputMethodCallback* callback)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageO(DO_CREATE_SESSION, callback, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::SetSessionEnabled(
    /* [in] */ IIInputMethodSession* session,
    /* [in] */ Boolean enabled)
{
    // try {
    AutoPtr<IInputMethodSession> ls = ((CIInputMethodSessionWrapper*)
            session)->GetInternalInputMethodSession();

    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageIO(DO_SET_SESSION_ENABLED, enabled ? 1 : 0, ls, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::RevokeSession(
    /* [in] */ IIInputMethodSession* session)
{
    // try {
    AutoPtr<IInputMethodSession> ls = ((CIInputMethodSessionWrapper*)
            session)->GetInternalInputMethodSession();

    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageO(DO_REVOKE_SESSION, ls, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::ShowSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageIO(DO_SHOW_SOFT_INPUT, flags, resultReceiver, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::HideSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageIO(DO_HIDE_SOFT_INPUT, flags, resultReceiver, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::ChangeInputMethodSubtype(
    /* [in] */ IInputMethodSubtype* subtype)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageO(DO_CHANGE_INPUTMETHOD_SUBTYPE, subtype, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodWrapper::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("CIInputMethodWrapper");
    return NOERROR;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
