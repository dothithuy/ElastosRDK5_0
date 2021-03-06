
#ifndef __CBORINGLAYOUTMETRICS_H__
#define __CBORINGLAYOUTMETRICS_H__

#include "_CBoringLayoutMetrics.h"

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CBoringLayoutMetrics)
{
public:
    CBoringLayoutMetrics();

    CARAPI GetTop(
        /* [out] */ Int32* top);

    CARAPI SetTop(
        /* [in] */ Int32 top);

    CARAPI GetAscent(
        /* [out] */ Int32* ascent);

    CARAPI SetAscent(
        /* [in] */ Int32 ascent);

    CARAPI GetDescent(
        /* [out] */ Int32* descent);

    CARAPI SetDescent(
        /* [in] */ Int32 descent);

    CARAPI GetBottom(
        /* [out] */ Int32* bottom);

    CARAPI SetBottom(
        /* [in] */ Int32 bottom);

    CARAPI GetLeading(
        /* [out] */ Int32* leading);

    CARAPI SetLeading(
        /* [in] */ Int32 leading);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

private:
    Int32 mTop;
    Int32 mAscent;
    Int32 mDescent;
    Int32 mBottom;
    Int32 mLeading;
    Int32 mWidth;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__CBORINGLAYOUTMETRICS_H__
