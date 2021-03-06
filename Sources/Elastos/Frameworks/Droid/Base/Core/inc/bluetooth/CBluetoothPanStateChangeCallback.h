
#ifndef __CBLUETOOTHPANSTATECHANGECALLBACK_H__
#define __CBLUETOOTHPANSTATECHANGECALLBACK_H__

#include "_CBluetoothPanStateChangeCallback.h"
#include "BluetoothPan.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothPanStateChangeCallback)
{
public:
    CARAPI OnBluetoothStateChange(
        /* [in] */ Boolean on);

    CARAPI constructor(
        /* [in] */ IInterface* host);

private:
	BluetoothPan* mHost;
};

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos

#endif //__CBLUETOOTHPANSTATECHANGECALLBACK_H__
