
#ifndef __CDATETIMEKEYLISTENERHELPER_H__
#define __CDATETIMEKEYLISTENERHELPER_H__

#include "_CDateTimeKeyListenerHelper.h"
#include "text/method/BaseKeyListenerMacro.h"
#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CDateTimeKeyListenerHelper)
{
public:
    IBaseKeyListenerHelper_METHODS_DECL()

    CARAPI GetCHARACTERS(
        /* [out, callee] */ ArrayOf<Char32>** ret);

    CARAPI GetInstance(
        /* [out] */ IDateTimeKeyListener** ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CDATETIMEKEYLISTENERHELPER_H__
