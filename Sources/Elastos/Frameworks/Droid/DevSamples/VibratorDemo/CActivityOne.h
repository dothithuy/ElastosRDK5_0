
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "app/Activity.h"
#include "_CActivityOne.h"
#include <elrefbase.h>

using Elastos::Droid::App::Activity;
using Elastos::Droid::App::IAlarmManager;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Os::IVibrator;

namespace Elastos {
namespace Droid {
namespace DevSamples {
namespace VibratorDemo {

class CActivityOne : public Activity
{
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

public:
    AutoPtr<IVibrator> mVb;
};

} // namespace VibratorDemo
} // namespace DevSamples
} // namespace Droid
} // namespace Elastos

#endif // __CACTIVITYONE_H__