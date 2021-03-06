#ifndef __ELASTOS_DROID_OS_HANDLER_CALLER_H__
#define __ELASTOS_DROID_OS_HANDLER_CALLER_H__

#include "ext/frameworkext.h"
#include "os/HandlerBase.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IHandlerCallerCallback;

namespace Elastos {
namespace Droid {
namespace Os {

class HandlerCaller
    : public IWeakReferenceSource
    , public IHandlerCaller
    , public ElRefBase
{
private:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ IWeakReference* host);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        AutoPtr<IWeakReference> mWeakHost;
    };

public:
    CAR_INTERFACE_DECL()

    CARAPI GetWeakReference(
        /* [out] */ IWeakReference** weakReference);

    HandlerCaller(
        /* [in] */ IContext* context,
        /* [in] */ IHandlerCallerCallback* callback,
        /* [in] */ Boolean isStrong);

    HandlerCaller(
        /* [in] */ IContext* context,
        /* [in] */ ILooper* looper,
        /* [in] */ IHandlerCallerCallback* callback,
        /* [in] */ Boolean isStrong);

    CARAPI GetContext(
        /* [out] */ IContext** ctx);

    CARAPI SetContext(
        /* [in] */ IContext* ctx);

    CARAPI ExecuteOrSendMessage(
        /* [in] */ IMessage* msg);

    CARAPI HasMessages(
        /* [in] */ Int32 what,
        /* [out] */ Boolean* result);

    CARAPI RemoveMessages(
        /* [in] */ Int32 what);

    CARAPI RemoveMessagesEx(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    CARAPI SendMessage(
        /* [in] */ IMessage* msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageBO(
        /* [in] */ Int32 what,
        /* [in] */ Boolean arg1,
        /* [in] */ IInterface* arg2,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageBOO(
        /* [in] */ Int32 what,
        /* [in] */ Boolean arg1,
        /* [in] */ IInterface* arg2,
        /* [in] */ IInterface* arg3,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageO(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* arg1,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageI(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageII(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageIO(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ IInterface* arg2,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageIIO(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* arg3,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageIIOO(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* arg3,
        /* [in] */ IInterface* arg4,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageIOO(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ IInterface* arg2,
        /* [in] */ IInterface* arg3,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageOO(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* arg1,
        /* [in] */ IInterface* arg2,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageOOO(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* arg1,
        /* [in] */ IInterface* arg2,
        /* [in] */ IInterface* arg3,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageOOOO(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* arg1,
        /* [in] */ IInterface* arg2,
        /* [in] */ IInterface* arg3,
        /* [in] */ IInterface* arg4,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageIIII(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ Int32 arg3,
        /* [in] */ Int32 arg4,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageIIIII(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ Int32 arg3,
        /* [in] */ Int32 arg4,
        /* [in] */ Int32 arg5,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageIIIIII(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ Int32 arg3,
        /* [in] */ Int32 arg4,
        /* [in] */ Int32 arg5,
        /* [in] */ Int32 arg6,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessageIIII0(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ Int32 arg3,
        /* [in] */ Int32 arg4,
        /* [in] */ IInterface* arg5,
        /* [out] */ IMessage** msg);

private:
    AutoPtr<IHandlerCallerCallback> GetCallback();

private:
    AutoPtr<IContext> mContext;

    AutoPtr<ILooper> mMainLooper;
    AutoPtr<MyHandler> mH;

    AutoPtr<IHandlerCallerCallback> mCallback;
    AutoPtr<IWeakReference> mWeakCallback;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_HANDLER_CALLER_H__
