
#include "content/AsyncQueryHandler.h"
#include <elastos/Logger.h>
#include <elastos/StringUtils.h>
#include "os/CHandlerThread.h"

using Elastos::Core::StringUtils;
using Elastos::Core::CInteger32;
using Elastos::Core::IInteger32;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Os::CHandlerThread;

namespace Elastos {
namespace Droid {
namespace Content {

const String AsyncQueryHandler::TAG = String("AsyncQuery");
const Boolean AsyncQueryHandler::localLOGV = FALSE;
const Int32 AsyncQueryHandler::EVENT_ARG_QUERY;
const Int32 AsyncQueryHandler::EVENT_ARG_INSERT;
const Int32 AsyncQueryHandler::EVENT_ARG_UPDATE;
const Int32 AsyncQueryHandler::EVENT_ARG_DELETE;
AutoPtr<ILooper> AsyncQueryHandler::sLooper;

AsyncQueryHandler::WorkerArgs::WorkerArgs()
{}

AsyncQueryHandler::WorkerArgs::~WorkerArgs()
{}

CAR_INTERFACE_IMPL(AsyncQueryHandler::WorkerArgs, IInterface)

AsyncQueryHandler::WorkerHandler::WorkerHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ AsyncQueryHandler* context)
    : HandlerBase(looper)
    , mContext(context)
{}

AsyncQueryHandler::WorkerHandler::~WorkerHandler()
{}

ECode AsyncQueryHandler::WorkerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IInterface> resolverObj;
    mContext->mResolver->Resolve(EIID_IInterface, (IInterface**)&resolverObj);
    if (NULL == resolverObj) return NOERROR;
    AutoPtr<IContentResolver> resolver = (IContentResolver*)resolverObj->Probe(EIID_IContentResolver);
    if (NULL == resolver) return NOERROR;

    Int32 what, arg1;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    AutoPtr<WorkerArgs> args = (WorkerArgs*)obj.Get();
    Int32 token = what;
    Int32 event = arg1;

    switch (event) {
        case AsyncQueryHandler::EVENT_ARG_QUERY:
        {
            AutoPtr<ICursor> cursor;
            // try {
            ECode ec = resolver->Query(args->mUri, args->mProjection,
                    args->mSelection, args->mSelectionArgs, args->mOrderBy, (ICursor**)&cursor);
            if (FAILED(ec)) cursor = NULL;
            // Calling getCount() causes the cursor window to be filled,
            // which will make the first access on the main thread a lot faster.
            if (NULL != cursor) {
                Int32 count = 0;
                ec = cursor->GetCount(&count);
                if (FAILED(ec)) {
                    cursor = NULL;
                    Logger::W(TAG, "Exception thrown during handling EVENT_ARG_QUERY");
                }
            }

            args->mResult = (IInterface*)cursor.Get();
            break;
        }
        case AsyncQueryHandler::EVENT_ARG_INSERT:
        {
            FAIL_RETURN(resolver->Insert(args->mUri, args->mValues, (IUri**)&args->mResult))
            break;
        }
        case AsyncQueryHandler::EVENT_ARG_UPDATE:
        {
            args->mResult = NULL;
            Int32 result = 0;
            FAIL_RETURN(resolver->Update(args->mUri, args->mValues, args->mSelection,
                args->mSelectionArgs, &result))
            FAIL_RETURN(CInteger32::New(result, (IInteger32**)&args->mResult))
            break;
        }
        case AsyncQueryHandler::EVENT_ARG_DELETE:
        {
            args->mResult = NULL;
            Int32 result = 0;
            FAIL_RETURN(resolver->Delete(args->mUri, args->mSelection, args->mSelectionArgs, &result))
            FAIL_RETURN(CInteger32::New(result, (IInteger32**)&args->mResult))
            break;
        }
    }

    if (AsyncQueryHandler::localLOGV) {
        Logger::D(TAG, "WorkerHandler.handleMsg: msg.arg1=%d, reply.what=%d", arg1, what);
    }

    // passing the original token value back to the caller
    // on top of the event values in arg1.
    AutoPtr<IHandler> replyHandler = args->mHandler;

    AutoPtr<IMessage> reply;
    replyHandler->ObtainMessageEx3(token, 1, 0, args, (IMessage**)&reply);
    return reply->SendToTarget();
}

AsyncQueryHandler::AsyncQueryHandler()
    : Handler(FALSE)
{}

AsyncQueryHandler::AsyncQueryHandler(
    /* [in] */ IContentResolver* cr)
    : Handler(FALSE)
    , mWorkerThreadHandler(NULL)
{
    AutoPtr<IWeakReferenceSource> wrs = (IWeakReferenceSource*)cr->Probe(EIID_IWeakReferenceSource);
    assert(wrs != NULL);
    wrs->GetWeakReference((IWeakReference**)&mResolver);

    {
        Mutex::Autolock lock(mAsyncQueryHandlerLock);
        if (NULL == sLooper) {
            AutoPtr<IHandlerThread> thread;
            CHandlerThread::New(String("AsyncQueryWorker"), (IHandlerThread**)&thread);
            thread->Start();
            thread->GetLooper((ILooper**)&sLooper);
        }
    }

    ASSERT_SUCCEEDED(CreateHandler(sLooper, (IHandler**)&mWorkerThreadHandler))
}

AsyncQueryHandler::~AsyncQueryHandler()
{}

ECode AsyncQueryHandler::Init(
    /* [in] */ IContentResolver* cr)
{
    AutoPtr<IWeakReferenceSource> wrs = (IWeakReferenceSource*)cr->Probe(EIID_IWeakReferenceSource);
    assert(wrs != NULL);
    wrs->GetWeakReference((IWeakReference**)&mResolver);

    mWorkerThreadHandler = NULL;
    {
        Mutex::Autolock lock(mAsyncQueryHandlerLock);
        if (NULL == sLooper) {
            AutoPtr<IHandlerThread> thread;
            CHandlerThread::New(String("AsyncQueryWorker"), (IHandlerThread**)&thread);
            thread->Start();
            thread->GetLooper((ILooper**)&sLooper);
        }
    }
    ASSERT_SUCCEEDED(CreateHandler(sLooper, (IHandler**)&mWorkerThreadHandler))
    return NOERROR;
}

CAR_INTERFACE_IMPL_2(AsyncQueryHandler, IAsyncQueryHandler, IHandler)

IHANDLER_METHODS_IMPL(AsyncQueryHandler, Handler)

ECode AsyncQueryHandler::StartQuery(
    /* [in] */ Int32 token,
    /* [in] */ IInterface* cookie,
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& orderBy)
{
    // Use the token as what so cancelOperations works properly

    AutoPtr<WorkerArgs> args = new WorkerArgs();
    args->mHandler = (IHandler*) this;
    args->mUri = uri;
    args->mProjection = projection;
    args->mSelection = selection;
    args->mSelectionArgs = selectionArgs;
    args->mOrderBy = orderBy;
    args->mCookie = cookie;

    AutoPtr<IMessage> msg;
    mWorkerThreadHandler->ObtainMessageEx3(token, EVENT_ARG_QUERY, 0, args, (IMessage**)&msg);
    Boolean result;
    return mWorkerThreadHandler->SendMessage(msg, &result);
}

ECode AsyncQueryHandler::CancelOperation(
    /* [in] */ Int32 token)
{
    return mWorkerThreadHandler->RemoveMessages(token);
}

ECode AsyncQueryHandler::StartInsert(
    /* [in] */ Int32 token,
    /* [in] */ IInterface* cookie,
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* initialValues)
{
    // Use the token as what so cancelOperations works properly
    AutoPtr<WorkerArgs> args = new WorkerArgs();
    args->mHandler = (IHandler*) this;
    args->mUri = uri;
    args->mCookie = cookie;
    args->mValues = initialValues;

    AutoPtr<IMessage> msg;
    mWorkerThreadHandler->ObtainMessageEx3(token, EVENT_ARG_INSERT, 0, args, (IMessage**)&msg);
    Boolean result;
    return mWorkerThreadHandler->SendMessage(msg, &result);
}

ECode AsyncQueryHandler::StartUpdate(
    /* [in] */ Int32 token,
    /* [in] */ IInterface* cookie,
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs)
{
    // Use the token as what so cancelOperations works properly
    AutoPtr<WorkerArgs> args = new WorkerArgs();
    args->mHandler = (IHandler*) this;
    args->mUri = uri;
    args->mCookie = cookie;
    args->mValues = values;
    args->mSelection = selection;
    args->mSelectionArgs = selectionArgs;

    AutoPtr<IMessage> msg;
    mWorkerThreadHandler->ObtainMessageEx3(token, EVENT_ARG_UPDATE, 0, args, (IMessage**)&msg);
    Boolean result;
    return mWorkerThreadHandler->SendMessage(msg, &result);
}

ECode AsyncQueryHandler::StartDelete(
    /* [in] */ Int32 token,
    /* [in] */ IInterface* cookie,
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs)
{
    // Use the token as what so cancelOperations works properly
    AutoPtr<WorkerArgs> args = new WorkerArgs();
    args->mHandler = (IHandler*) this;
    args->mUri = uri;
    args->mCookie = cookie;
    args->mSelection = selection;
    args->mSelectionArgs = selectionArgs;

    AutoPtr<IMessage> msg;
    mWorkerThreadHandler->ObtainMessageEx3(token, EVENT_ARG_DELETE, 0, args, (IMessage**)&msg);
    Boolean result;
    return mWorkerThreadHandler->SendMessage(msg, &result);
}

ECode AsyncQueryHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    AutoPtr<WorkerArgs> args = (WorkerArgs*) obj.Get();
    if (localLOGV) {
        Logger::D(TAG, "AsyncQueryHandler.handleMessage: msg.what=%d, msg.arg1=%d", what, arg1);
    }

    Int32 token = what;
    Int32 event = arg1;

    // pass token back to caller on each callback.
    switch (event) {
        case EVENT_ARG_QUERY:
            return OnQueryComplete(token, args->mCookie, ICursor::Probe(args->mResult.Get()));

        case EVENT_ARG_INSERT:
            return OnInsertComplete(token, args->mCookie, IUri::Probe(args->mResult));

        case EVENT_ARG_UPDATE:
        {
            Int32 result = 0;
            FAIL_RETURN(IInteger32::Probe(args->mResult)->GetValue(&result))
            return OnUpdateComplete(token, args->mCookie, result);
        }
        case EVENT_ARG_DELETE:
        {
            Int32 result = 0;
            FAIL_RETURN(IInteger32::Probe(args->mResult)->GetValue(&result))
            return OnDeleteComplete(token, args->mCookie, result);
        }
    }

    return NOERROR;
}

ECode AsyncQueryHandler::CreateHandler(
    /* [in] */ ILooper* looper,
    /* [out] */ IHandler** handler)
{
    VALIDATE_NOT_NULL(handler)
    AutoPtr<WorkerHandler> worker = new WorkerHandler(looper, this);
    *handler = (IHandler*)worker.Get();
    INTERFACE_ADDREF(*handler);
    return NOERROR;
}

ECode AsyncQueryHandler::OnQueryComplete(
    /* [in] */ Int32 token,
    /* [in] */ IInterface* cookie,
    /* [in] */ ICursor* cursor)
{
    return NOERROR;
}

ECode AsyncQueryHandler::OnInsertComplete(
    /* [in] */ Int32 token,
    /* [in] */ IInterface* cookie,
    /* [in] */ IUri* uri)
{
    return NOERROR;
}

ECode AsyncQueryHandler::OnUpdateComplete(
    /* [in] */ Int32 token,
    /* [in] */ IInterface* cookie,
    /* [in] */ Int32 result)
{
    return NOERROR;
}

ECode AsyncQueryHandler::OnDeleteComplete(
    /* [in] */ Int32 token,
    /* [in] */ IInterface* cookie,
    /* [in] */ Int32 result)
{
    return NOERROR;
}

}
}
}

