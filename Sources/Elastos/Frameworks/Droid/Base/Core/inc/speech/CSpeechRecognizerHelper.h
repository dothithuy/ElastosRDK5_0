
#ifndef __CSPEECHRECOGNIZERHELPER_H__
#define __CSPEECHRECOGNIZERHELPER_H__

#include "_CSpeechRecognizerHelper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Speech::ISpeechRecognizer;
using Elastos::Droid::Content::IComponentName;

namespace Elastos {
namespace Droid {
namespace Speech {

CarClass(CSpeechRecognizerHelper)
{
public:
    CARAPI IsRecognitionAvailable(
        /* [in] */ IContext * pContext,
        /* [out] */ Boolean * pRet);

    CARAPI CreateSpeechRecognizer(
        /* [in] */ IContext * pContext,
        /* [out] */ ISpeechRecognizer ** ppRet);

    CARAPI CreateSpeechRecognizerEx(
        /* [in] */ IContext * pContext,
        /* [in] */ IComponentName * pServiceComponent,
        /* [out] */ ISpeechRecognizer ** ppRet);

private:
    // TODO: Add your private member variables here.
};

}//namespace Speech
}//namespace Droid
}//namespace Elastos

#endif // __CSPEECHRECOGNIZERHELPER_H__
