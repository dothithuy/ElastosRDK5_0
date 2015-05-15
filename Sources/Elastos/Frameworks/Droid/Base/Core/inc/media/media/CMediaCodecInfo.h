
#ifndef __CMEDIACODECINFO_H__
#define __CMEDIACODECINFO_H__

#include "_CMediaCodecInfo.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Media::ICodecCapabilities;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaCodecInfo)
{
public:
    /**
     * Retrieve the codec name.
     */
    CARAPI GetName(
        /* [out] */ String* result);

    /**
     * Query if the codec is an encoder.
     */
    CARAPI IsEncoder(
        /* [out] */ Boolean* result);

    /**
     * Query the media types supported by the codec.
     */
    CARAPI GetSupportedTypes(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI GetCapabilitiesForType(
        /* [in] */ const String& type,
        /* [out] */ ICodecCapabilities** result);


   CARAPI constructor(
        /* [in] */ Int32 index);


private:
    Int32 mIndex;

};


} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __CMEDIACODECINFO_H__