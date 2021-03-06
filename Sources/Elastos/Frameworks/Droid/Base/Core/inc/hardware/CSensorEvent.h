
#ifndef  __CSENSOREVENT_H__
#define  __CSENSOREVENT_H__

#include "_CSensorEvent.h"


namespace Elastos {
namespace Droid {
namespace Hardware {

CarClass(CSensorEvent)
{
public:
    CSensorEvent();

    CARAPI SetValues(
        /* [in] */ ArrayOf<Float>* values);

    CARAPI GetValues(
        /* [out, callee] */ ArrayOf<Float>** values);

    CARAPI GetSensor(
        /* [out] */ ISensor** sensor);

    CARAPI GetAccuracy(
        /* [out] */ Int32* accuracy);

    CARAPI GetTimestamp(
        /* [out] */ Int64* timestamp);

    CARAPI SetOriginalValue(
        /* [in] */ ArrayOf<Float>* values);

    CARAPI GetOriginalValue(
        /* [out, callee] */ ArrayOf<Float>** values);

    CARAPI constructor(
        /* [in] */ Int32 size);

public:
    AutoPtr<ArrayOf<Float> > mValues;

    AutoPtr<ArrayOf<Float> > mOriginalValues;

    /**
     * The sensor that generated this event. See
     * {@link android.hardware.SensorManager SensorManager} for details.
     */
    AutoPtr<ISensor> mSensor;

    /**
     * The accuracy of this event. See {@link android.hardware.SensorManager
     * SensorManager} for details.
     */
    Int32 mAccuracy;


    /**
     * The time in nanosecond at which the event happened
     */
    Int64 mTimestamp;
};


} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif  //__CSENSOREVENT_H__
