
#ifndef __CACTIVITYRECORDTOKEN_H__
#define __CACTIVITYRECORDTOKEN_H__

#include "_CActivityRecordToken.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class ActivityRecord;

CarClass(CActivityRecordToken)
{
public:
    CActivityRecordToken();

    ~CActivityRecordToken();

    CARAPI constructor(
        /* [in] */ Handle32 activity);

    CARAPI WindowsDrawn();

    CARAPI WindowsVisible();

    CARAPI WindowsGone();

    CARAPI KeyDispatchingTimedOut(
        /* [out] */ Boolean* result);

    CARAPI GetKeyDispatchingTimeout(
        /* [out] */ Int64* timeout);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI_(AutoPtr<ActivityRecord>) GetActivityRecord();

public:
    AutoPtr<IWeakReference> mWeakActivity; // WeakReference<ActivityRecord>
    String mStringName;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__CACTIVITYRECORDTOKEN_H__
