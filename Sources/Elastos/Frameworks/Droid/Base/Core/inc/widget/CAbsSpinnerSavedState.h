#ifndef ___CABSSPINNERSAVEDSTATE_H__
#define ___CABSSPINNERSAVEDSTATE_H__

#include "_CAbsSpinnerSavedState.h"
#include "AbsSpinnerSavedState.h"

namespace Elastos{
namespace Droid {
namespace Widget {

CarClass(CAbsSpinnerSavedState) , AbsSpinnerSavedState
{
public:

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor(
        /* [in] */ IParcelable* superState);

    CARAPI constructor();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    using AbsSpinnerSavedState::mSelectedId;
    using AbsSpinnerSavedState::mPosition;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
