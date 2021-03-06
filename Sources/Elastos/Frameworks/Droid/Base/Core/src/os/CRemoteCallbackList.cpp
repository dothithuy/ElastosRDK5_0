
#include "os/CRemoteCallbackList.h"
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Os {

CAR_INTERFACE_IMPL(CRemoteCallbackList::Callback, IProxyDeathRecipient)

CRemoteCallbackList::Callback::Callback(
    /* [in] */ IInterface* callback,
    /* [in] */ IInterface* cookie,
    /* [in] */ CRemoteCallbackList* owner)
    : mCallback(callback)
    , mCookie(cookie)
    , mOwner(owner)
{}

ECode CRemoteCallbackList::Callback::ProxyDied()
{
    Slogger::W("CRemoteCallbackList::Callback", " >> %p ProxyDied()", (IProxyDeathRecipient*)this);
    {
        Mutex::Autolock lock(mOwner->mCallbacksLock);

        mOwner->mCallbacks.Erase(IBinder::Probe(mCallback));
    }
    return mOwner->OnCallbackDiedEx(mCallback, mCookie);
}


CRemoteCallbackList::CRemoteCallbackList()
    : mCallbacks(20)
    , mBroadcastCount(-1)
    , mKilled(FALSE)
{}

ECode CRemoteCallbackList::Register(
    /* [in] */ IInterface* callback,
    /* [out] */ Boolean* result)
{
    return RegisterEx(callback, NULL, result);
}

ECode CRemoteCallbackList::RegisterEx(
    /* [in] */ IInterface* callback,
    /* [in] */ IInterface* cookie,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    {
        Mutex::Autolock lock(mCallbacksLock);

        if (mKilled) {
            *result = FALSE;
            return NOERROR;
        }
        AutoPtr<IBinder> binder = IBinder::Probe(callback);
        // try {
        Callback* cb = new Callback(callback, cookie, this);
        IProxy* proxy = (IProxy*)callback->Probe(EIID_IProxy);
        if (proxy != NULL) {
            proxy->LinkToDeath(cb, 0);
        }
        mCallbacks[binder] = cb;
        *result = TRUE;
        return NOERROR;
        // } catch (RemoteException e) {
        //     return false;
        // }
    }
}

ECode CRemoteCallbackList::Unregister(
    /* [in] */ IInterface* callback,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    {
        Mutex::Autolock lock(mCallbacksLock);

        HashMap< AutoPtr<IBinder>, AutoPtr<Callback> >::Iterator it =
                mCallbacks.Find(IBinder::Probe(callback));
        if (it != mCallbacks.End()) {
            AutoPtr<Callback> cb = it->mSecond;
            mCallbacks.Erase(it);
            IProxy* proxy = (IProxy*)cb->mCallback->Probe(EIID_IProxy);
            assert(proxy != NULL);
            proxy->UnlinkToDeath(cb, 0, result);
            *result = TRUE;
            return NOERROR;
        }
        *result = FALSE;
        return NOERROR;
    }
}

ECode CRemoteCallbackList::Kill()
{
    {
        Mutex::Autolock lock(mCallbacksLock);

        HashMap< AutoPtr<IBinder>, AutoPtr<Callback> >::Iterator it;
        for (it = mCallbacks.Begin(); it != mCallbacks.End(); ++it) {
            AutoPtr<Callback> cb = it->mSecond;
            IProxy* proxy = (IProxy*)cb->mCallback->Probe(EIID_IProxy);
            assert(proxy != NULL);
            Boolean result;
            proxy->UnlinkToDeath(cb.Get(), 0, &result);
        }
        mCallbacks.Clear();
        mKilled = TRUE;
    }
    return NOERROR;
}

ECode CRemoteCallbackList::OnCallbackDied(
    /* [in] */ IInterface* callback)
{
    return NOERROR;
}

ECode CRemoteCallbackList::OnCallbackDiedEx(
    /* [in] */ IInterface* callback,
    /* [in] */ IInterface* cookie)
{
    return OnCallbackDied(callback);
}

ECode CRemoteCallbackList::BeginBroadcast(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    {
        Mutex::Autolock lock(mCallbacksLock);

        if (mBroadcastCount > 0) {
            // throw new IllegalStateException(
            //         "beginBroadcast() called while already in a broadcast");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        const Int32 N = mBroadcastCount = mCallbacks.GetSize();
        if (N <= 0) {
            *number = 0;
            return NOERROR;
        }
        AutoPtr< ArrayOf<IInterface*> > active = mActiveBroadcast;
        if (active == NULL || active->GetLength() < N) {
            mActiveBroadcast = active = ArrayOf<IInterface*>::Alloc(N);
        }
        Int32 i = 0;
        HashMap< AutoPtr<IBinder>, AutoPtr<Callback> >::Iterator it;
        for (it = mCallbacks.Begin(); it != mCallbacks.End(); ++it) {
            active->Set(i++, it->mSecond.Get());
        }
        *number = i;
        return NOERROR;
    }
}

ECode CRemoteCallbackList::GetBroadcastItem(
    /* [in] */ Int32 index,
    /* [out] */ IInterface** callback)
{
    VALIDATE_NOT_NULL(callback);

    *callback = ((Callback*)(*mActiveBroadcast)[index])->mCallback;
    INTERFACE_ADDREF(*callback);
    return NOERROR;
}

ECode CRemoteCallbackList::GetBroadcastCookie(
    /* [in] */ Int32 index,
    /* [out] */ IInterface** cookie)
{
    VALIDATE_NOT_NULL(cookie);

    *cookie = ((Callback*)(*mActiveBroadcast)[index])->mCookie;
    INTERFACE_ADDREF(*cookie);
    return NOERROR;
}

ECode CRemoteCallbackList::FinishBroadcast()
{
    if (mBroadcastCount < 0) {
        // throw new IllegalStateException(
        //         "finishBroadcast() called outside of a broadcast");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr< ArrayOf<IInterface*> > active = mActiveBroadcast;
    if (active != NULL) {
        const Int32 N = mBroadcastCount;
        for (Int32 i = 0; i < N; i++) {
            active->Set(i, NULL);
        }
    }

    mBroadcastCount = -1;
    return NOERROR;
}

ECode CRemoteCallbackList::GetRegisteredCallbackCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    {
        Mutex::Autolock lock(mCallbacksLock);

        if (mKilled) {
            *count = 0;
            return NOERROR;
        }
        *count = mCallbacks.GetSize();
        return NOERROR;
    }
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
