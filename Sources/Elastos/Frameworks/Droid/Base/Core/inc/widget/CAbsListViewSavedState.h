#ifndef __CABSLISTVIEWSAVEDSTATE_H__
#define __CABSLISTVIEWSAVEDSTATE_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

#include "_CAbsListViewSavedState.h"
#include "widget/AbsListViewSavedState.h"
#include "widget/AbsListView.h"

using Elastos::Droid::Widget::AbsListViewSavedState;
using Elastos::Droid::Widget::AbsListView;

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CAbsListViewSavedState) , public AbsListViewSavedState
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor(
        /* [in] */ IParcelable* superState);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
    friend class AbsListView;
};

}// namespace Elastos
}// namespace Droid
}// namespace Widget

#endif
