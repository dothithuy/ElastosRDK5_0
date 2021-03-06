
#ifndef __CBLUETOOTHCLASS_H__
#define __CBLUETOOTHCLASS_H__

#include "_CBluetoothClass.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothClass)
{
public:
    CARAPI GetHashCode(
        /* [out] */ Int32* code);

    CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI HasService(
        /* [in] */ Int32 service,
        /* [out] */ Boolean* hasService);

    CARAPI GetMajorDeviceClass(
        /* [out] */ Int32* deviceClass);

    CARAPI GetDeviceClass(
        /* [out] */ Int32* deviceClass);

    CARAPI DoesClassMatch(
        /* [in] */ Int32 profile,
        /* [out] */ Boolean* isMatch);

    CARAPI GetClass(
        /* [out] */ Int32* value);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 classInt);

private:
    Int32 mClass;
};

}
}
}

#endif // __CBLUETOOTHCLASS_H__
