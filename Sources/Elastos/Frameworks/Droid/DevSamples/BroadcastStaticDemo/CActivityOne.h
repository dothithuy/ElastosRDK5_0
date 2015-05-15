
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "app/Activity.h"
#include "_CActivityOne.h"
#include <elrefbase.h>

using Elastos::Droid::App::Activity;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::IViewOnClickListener;

namespace Elastos {
namespace Droid {
namespace DevSamples {
namespace BroadcastStaticDemo {

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

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        CActivityOne* mHost;
    };

protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnRestart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

private:
    // TODO: Add your private member variables here.
};

} // namespace BroadcastStaticDemo
} // namespace DevSamples
} // namespace Droid
} // namespace Elastos

#endif // __CACTIVITYONE_H__