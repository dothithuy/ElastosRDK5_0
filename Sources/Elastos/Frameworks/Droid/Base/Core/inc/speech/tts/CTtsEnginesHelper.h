
#ifndef __CTTSENGINESHELPER_H__
#define __CTTSENGINESHELPER_H__

#include "_CTtsEnginesHelper.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

CarClass(CTtsEnginesHelper)
{
public:
    CARAPI ParseLocalePref(
        /* [in] */ const String& pref,
        /* [out, callee] */ ArrayOf<String> ** ppRet);

private:
    // TODO: Add your private member variables here.
};

}//namespace Tts
}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __CTTSENGINESHELPER_H__
