
#ifndef __CHIDERETURNSTRANSFORMATIONMETHODHELPER_H__
#define __CHIDERETURNSTRANSFORMATIONMETHODHELPER_H__

#include "_CHideReturnsTransformationMethodHelper.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CHideReturnsTransformationMethodHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IHideReturnsTransformationMethod** ret);
};

} //namespace Method
} //namespace Text
} //namespace Droid
} //namespace Elastos

#endif // __CHIDERETURNSTRANSFORMATIONMETHODHELPER_H__
