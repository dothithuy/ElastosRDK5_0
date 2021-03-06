
#ifndef __CPARSEPOSITION_H__
#define __CPARSEPOSITION_H__

#include "_CParsePosition.h"

namespace Elastos {
namespace Text {

CarClass(CParsePosition)
{
public:
    CParsePosition();

    CARAPI constructor(
        /* [in] */ Int32 index);

//    @Override
//    public boolean equals(Object object);

    CARAPI GetErrorIndex(
        /* [out] */ Int32* index);

    CARAPI GetIndex(
        /* [out] */ Int32* index);

//    @Override
//    public int hashCode();

    CARAPI SetErrorIndex(
        /* [in] */ Int32 index);

    CARAPI SetIndex(
        /* [in] */ Int32 index);

//    @Override
//    public String toString();

private:
    Int32 mCurrentPosition;
    Int32 mErrorIndex;
};

} // namespace Text
} // namespace Elastos

#endif // __CPARSEPOSITION_H__
