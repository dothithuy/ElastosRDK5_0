
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "app/Activity.h"
#include "_CActivityOne.h"
#include <elrefbase.h>

using Elastos::Droid::App::Activity;
using Elastos::Droid::View::IViewOnClickListener;

namespace Elastos {
namespace Droid {
namespace DevSamples {
namespace ActivityTest {

class CActivityOne : public Activity
{
public:
    class MyListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        MyListener(
            /* [in] */ CActivityOne* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        AutoPtr<CActivityOne> mHost;
        Int32 mRef;
    };

public:
    CActivityOne();

    virtual CARAPI OnRestoreInstanceState(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI OnSaveInstanceState(
        /* [in] */ IBundle* outState);

protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

private:
    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);

private:
    AutoPtr<IView> mStartNewActivity;
    AutoPtr<IView> mTextValue;
    Int32 mState;
};

} // namespace ActivityTest
} // namespace DevSamples
} // namespace Droid
} // namespace Elastos

#endif // __CACTIVITYONE_H__