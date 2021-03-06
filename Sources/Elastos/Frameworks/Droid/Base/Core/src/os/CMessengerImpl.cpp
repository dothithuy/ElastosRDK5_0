
#include "os/CMessengerImpl.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CMessengerImpl::constructor(
    /* [in] */ IHandler* handler)
{
    mOwner = handler;
    return NOERROR;
}

ECode CMessengerImpl::Send(
    /* [in] */ IMessage* message)
{
    Boolean result;
    return mOwner->SendMessage(message, &result);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
