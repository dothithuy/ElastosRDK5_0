
#ifndef __CDECODERCAPABILITIESHELPER_H__
#define __CDECODERCAPABILITIESHELPER_H__

#include "_CDecoderCapabilitiesHelper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CDecoderCapabilitiesHelper)
{
public:
    /**
     * Returns the list of video decoder types
     * @see android.media.DecoderCapabilities.VideoDecoder
     */
    CARAPI GetVideoDecoders(
        /* [out, callee] */ ArrayOf<VideoDecoder>** result);

    /**
     * Returns the list of audio decoder types
     * @see android.media.DecoderCapabilities.AudioDecoder
     */
    CARAPI GetAudioDecoders(
        /* [out, callee] */ ArrayOf<AudioDecoder>** result);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CDECODERCAPABILITIESHELPER_H__
