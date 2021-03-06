
#ifndef __CABSOLUTESIZESPAN_H__
#define __CABSOLUTESIZESPAN_H__

#include "_CAbsoluteSizeSpan.h"
#include "text/style/AbsoluteSizeSpan.h"
#include "text/style/CharacterStyleMacro.h"
#include "text/style/MetricAffectingSpanMacro.h"
#include "text/ParcelableSpanMacro.h"

using Elastos::Droid::Text::Style::AbsoluteSizeSpan;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CAbsoluteSizeSpan)
    , public AbsoluteSizeSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetSize(
        /* [out] */ Int32* ret);

    CARAPI GetDip(
        /* [out] */ Boolean* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 size);

    CARAPI constructor(
        /* [in] */ Int32 size,
        /* [in] */ Boolean dip);

    CARAPI constructor(
        /* [in] */ IParcel* src);

};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CABSOLUTESIZESPAN_H__
