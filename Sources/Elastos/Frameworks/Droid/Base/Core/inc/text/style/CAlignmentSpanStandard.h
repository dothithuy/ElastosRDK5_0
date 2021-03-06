#ifndef __CALIGNMENTSPANSTANDARD_H__
#define __CALIGNMENTSPANSTANDARD_H__

#include "_CAlignmentSpanStandard.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CAlignmentSpanStandard)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ LayoutAlignment align);

    CARAPI constructor(
        /* [in] */ IParcel* src);

    CARAPI DescribeContents(
        /* [out] */ Int32* ret);

    CARAPI ReadFromParcel(
    /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSpanTypeId(
        /* [out] */ Int32* ret);

    CARAPI GetAlignment(
        /* [out] */ LayoutAlignment* alignment);

private:
    LayoutAlignment mAlignment;
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CALIGNMENTSPANSTANDARD_H__
