
#ifndef __CINPUTDEVICEHELPER_H__
#define __CINPUTDEVICEHELPER_H__

#include "_CInputDeviceHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CInputDeviceHelper)
{
public:
    /**
     * Gets information about the input device with the specified id.
     * @param id The device id.
     * @return The input device or NULL if not found.
     */
    CARAPI GetDevice(
        /* [in] */ Int32 id,
        /* [out] */ IInputDevice** device);

    /**
     * Gets the ids of all input devices in the system.
     * @return The input device ids.
     */
    CARAPI GetDeviceIds(
        /* [out, callee] */ ArrayOf<Int32>** deviceIds);
};

}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__CINPUTDEVICEHELPER_H__
