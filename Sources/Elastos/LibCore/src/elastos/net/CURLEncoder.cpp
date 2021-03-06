
#include "CURLEncoder.h"
#include "CCharsetHelper.h"
#include <elastos/Character.h>

using Elastos::IO::Charset::ICharsetHelper;
using Elastos::IO::Charset::CCharsetHelper;

namespace Elastos {
namespace Net {

ECode CURLEncoder::Encode(
    /* [in] */ const String& s,
    /* [out] */ String* result)
{
    AutoPtr<ICharset> charset = UriCodec::GetDefaultCharset();
    ENCODER.Encode(s, charset, result);
    return NOERROR;
}

ECode CURLEncoder::EncodeEx(
    /* [in] */ const String& s,
    /* [in] */ const String& charsetName,
    /* [out] */ String* result)
{
    AutoPtr<ICharsetHelper> helper;
    AutoPtr<ICharset> charset;
    CCharsetHelper::AcquireSingleton((ICharsetHelper**)&helper);
    helper->ForName(charsetName, (ICharset**)&charset);
    return ENCODER.Encode(s, charset, result);
}

} // namespace Net
} // namespace Elastos
