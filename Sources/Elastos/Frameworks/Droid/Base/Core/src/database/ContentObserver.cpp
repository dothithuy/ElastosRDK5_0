
#include "database/ContentObserver.h"
#ifdef DROID_CORE
#include "database/CContentObserverTransport.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include "os/Handler.h"

namespace Elastos {
namespace Droid {
namespace Database {

ContentObserver::NotificationRunnable::NotificationRunnable(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri,
    /* [in] */ ContentObserver* owner)
    : mSelfChange(selfChange)
    , mUri(uri)
    , mOwner(owner)
{
}

ECode ContentObserver::NotificationRunnable::Run()
{
    return mOwner->OnChangeEx(mSelfChange, mUri);
}

ContentObserver::ContentObserver(
    /* [in]  */ IHandler* handler)
    : mHandler(handler)
{}

ECode ContentObserver::Init(
    /* [in]  */ IHandler* handler)
{
    mHandler = handler;
    return NOERROR;
}

CAR_INTERFACE_IMPL(ContentObserver, IContentObserver)

ECode ContentObserver::GetContentObserver(
    /* [out] */ IIContentObserver** contentObserver)
{
    VALIDATE_NOT_NULL(contentObserver)

    Mutex::Autolock lock(mLock);
    if (mTransport == NULL) {
        CContentObserverTransport::New((IContentObserver*)this->Probe(EIID_IContentObserver), (IContentObserverTransport**)&mTransport);
    }
    *contentObserver = mTransport;
    INTERFACE_ADDREF(*contentObserver)
    return NOERROR;
}

ECode ContentObserver::ReleaseContentObserver(
    /* [out] */ IIContentObserver** contentObserver)
{
    VALIDATE_NOT_NULL(contentObserver)

    Mutex::Autolock lock(mLock);

    AutoPtr<IContentObserverTransport> oldTransport = mTransport;
    if (oldTransport != NULL) {
        oldTransport->ReleaseContentObserver();
        mTransport = NULL;
    }
    *contentObserver = oldTransport;
    INTERFACE_ADDREF(*contentObserver)
    return NOERROR;
}

ECode ContentObserver::DeliverSelfNotifications(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode ContentObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    return NOERROR;
}

ECode ContentObserver::OnChangeEx(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    return OnChange(selfChange);
}

ECode ContentObserver::DispatchChange(
    /* [in] */ Boolean selfChange)
{
    return DispatchChangeEx(selfChange, NULL);
}

ECode ContentObserver::DispatchChangeEx(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    if (mHandler == NULL) {
        OnChangeEx(selfChange, uri);
    }
    else {
        Boolean res;
        AutoPtr<IRunnable> run = new NotificationRunnable(selfChange, uri, this);
        mHandler->Post(run, &res);
    }
    return NOERROR;
}

} //Database
} //Droid
} //Elastos
