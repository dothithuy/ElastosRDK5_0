#include "app/CActionKeyInfo.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;

namespace Elastos {
namespace Droid {
namespace App {

CActionKeyInfo::CActionKeyInfo()
    : mKeyCode(0)
{}

ECode CActionKeyInfo::constructor()
{
    return NOERROR;
}

ECode CActionKeyInfo::constructor(
    /* [in] */ IContext* activityContext,
    /* [in] */ IAttributeSet* attr)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::SearchableActionKey),
        ARRAY_SIZE(R::styleable::SearchableActionKey));
    AutoPtr<ITypedArray>  a;
    activityContext->ObtainStyledAttributesEx2(attr, attrIds, (ITypedArray**)&a);

    a->GetInt32(R::styleable::SearchableActionKey_keycode, 0, &mKeyCode);
    a->GetString(R::styleable::SearchableActionKey_queryActionMsg, &mQueryActionMsg);
    a->GetString(R::styleable::SearchableActionKey_suggestActionMsg, &mSuggestActionMsg);
    a->GetString(R::styleable::SearchableActionKey_suggestActionMsgColumn, &mSuggestActionMsgColumn);
    a->Recycle();

    //sanity check
    if(mKeyCode == 0)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    } else if((mQueryActionMsg == NULL) &&
            (mSuggestActionMsg == NULL) &&
            (mSuggestActionMsgColumn == NULL)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CActionKeyInfo::GetKeyCode(
    /* [out] */ Int32* keyCode)
{
    *keyCode = mKeyCode;
    return NOERROR;
}

ECode CActionKeyInfo::GetQueryActionMsg(
    /* [out] */ String* actionMsg)
{
    *actionMsg = mQueryActionMsg;
    return NOERROR;
}

ECode CActionKeyInfo::GetSuggestActionMsg(
    /* [out] */ String* actionMsg)
{
    *actionMsg = mSuggestActionMsg;
    return NOERROR;
}

ECode CActionKeyInfo::GetSuggestActionMsgColumn(
    /* [out] */ String* column)
{
    *column = mSuggestActionMsgColumn;
    return NOERROR;
}

ECode CActionKeyInfo::DescribeContents(
    /* [out] */ Int32* rst)
{
    *rst = 0;
    return NOERROR;
}

ECode CActionKeyInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    source->ReadInt32(&mKeyCode);
    source->ReadString(&mQueryActionMsg);
    source->ReadString(&mSuggestActionMsg);
    source->ReadString(&mSuggestActionMsgColumn);
    return NOERROR;
}

ECode CActionKeyInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteInt32(mKeyCode);
    dest->WriteString(mQueryActionMsg);
    dest->WriteString(mSuggestActionMsg);
    dest->WriteString(mSuggestActionMsgColumn);
    return NOERROR;
}

}
}
}