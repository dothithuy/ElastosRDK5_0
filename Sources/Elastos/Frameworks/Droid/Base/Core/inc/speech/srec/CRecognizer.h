
#ifndef __CRECOGNIZER_H__
#define __CRECOGNIZER_H__

#include "_CRecognizer.h"
#include "speech/srec/Recognizer.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

CarClass(CRecognizer) , public Recognizer
{
public:
    CARAPI Start();

    CARAPI Advance(
        /* [out] */ Int32* ret);

    CARAPI PutAudio(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isLast,
        /* [out] */ Int32* ret);

    CARAPI PutAudioEx(
        /* [in] */ IInputStream* audio);

    CARAPI GetResultCount(
        /* [out] */ Int32* ret);

    CARAPI GetResultKeys(
        /* [in] */ Int32 index,
        /* [out, callee] */ ArrayOf<String>** ret);

    CARAPI GetResult(
        /* [in] */ Int32 index,
        /* [in] */ const String& key,
        /* [out] */ String* ret);

    CARAPI Stop();

    CARAPI ResetAcousticState();

    CARAPI SetAcousticState(
        /* [in] */ const String& state);

    CARAPI GetAcousticState(
        /* [out] */ String* ret);

    CARAPI Destroy();

    CARAPI constructor(
        /* [in] */ const String& configFile);

};

}//namespace Srec
}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __CRECOGNIZER_H__
