#ifndef __CWINDOWINFOHELPER_H__
#define __CWINDOWINFOHELPER_H__

#include "_CWindowInfoHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CWindowInfoHelper)
{
public:
    CARAPI Obtain(
        /* [out] */ IWindowInfo** info);

    CARAPI ObtainEx(
        /* [in] */ IWindowInfo *other,
        /* [out] */ IWindowInfo** info);

};

}// namespace View
}// namespace Droid
}// namespace Elastos
#endif
