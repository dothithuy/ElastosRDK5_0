
#include "CComposingView.h"
#include "graphics/CPaint.h"

using Elastos::Core::IStringBuffer;
using Elastos::Droid::Graphics::CPaint;

namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

const Int32 ComposingView::LEFT_RIGHT_MARGIN = 5;

ComposingView::ComposingView()
    : mStrColor(0)
    , mStrColorHl(0)
    , mStrColorIdle(0)
    , mFontSize(0)
{

}

void ComposingView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Float width = 0.f;
    Int32 height = 0, bottom = 0, top = 0;
    height = (mFmi->GetBottom(&bottom), bottom) - (mFmi->GetTop(&top), top) + mPaddingTop + mPaddingBottom;

    if (NULL == mDecInfo) {
        width = 0;
    } else {
        width = mPaddingLeft + mPaddingRight + LEFT_RIGHT_MARGIN * 2;

        String str;
        if (ComposingStatus_SHOW_STRING_LOWERCASE == mComposingStatus) {
            AutoPtr<IStringBuffer> strBuf;
            mDecInfo->GetOrigianlSplStr((IStringBuffer**)&strBuf);
            strBuf->ToString(&str);
        } else {
            mDecInfo->GetComposingStrForDisplay(&str);
        }
        Float value = 0.f;
        mPaint->MeasureTextEx(str, 0, str.GetLength(), &value);
        width += value;
    }
    SetMeasuredDimension((Int32) (width + 0.5f), height);
}

void ComposingView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    if (ComposingStatus_EDIT_PINYIN == mComposingStatus
            || ComposingStatus_SHOW_PINYIN == mComposingStatus) {
        DrawForPinyin(canvas);
        return;
    }

    Float x, y;
    Int32 top = 0;
    x = mPaddingLeft + LEFT_RIGHT_MARGIN;
    y = -(mFmi->GetTop(&top), top) + mPaddingTop;

    mPaint->SetColor(mStrColorHl);
    mHlDrawable->SetBounds(mPaddingLeft, mPaddingTop, GetWidth()
            - mPaddingRight, GetHeight() - mPaddingBottom);
    mHlDrawable->Draw(canvas);

    String splStr;
    AutoPtr<IStringBuffer> strBuf;
    mDecInfo->GetOrigianlSplStr((IStringBuffer**)&strBuf);
    strBuf->ToString(&splStr);
    canvas->DrawTextEx2(splStr, 0, splStr.GetLength(), x, y, mPaint);
}

void ComposingView::DrawCursor(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Float x)
{
    Int32 width = 0;
    mCursor->SetBounds((Int32) x, mPaddingTop, (Int32) x
            + (mCursor->GetIntrinsicWidth(&width), width), GetHeight() - mPaddingBottom);
    mCursor->Draw(canvas);
}

void ComposingView::DrawForPinyin(
    /* [in] */ ICanvas* canvas)
{
    Float x, y, value;
    Int32 top = 0;
    x = mPaddingLeft + LEFT_RIGHT_MARGIN;
    y = -(mFmi->GetTop(&top), top) + mPaddingTop;

    mPaint->SetColor(mStrColor);

    Int32 cursorPos = 0;
    mDecInfo->GetCursorPosInCmpsDisplay(&cursorPos);
    Int32 cmpsPos = cursorPos;
    String cmpsStr;
    mDecInfo->GetComposingStrForDisplay(&cmpsStr);
    Int32 activeCmpsLen = 0;
    mDecInfo->GetActiveCmpsDisplayLen(&activeCmpsLen);
    if (cursorPos > activeCmpsLen) cmpsPos = activeCmpsLen;
    canvas->DrawTextEx2(cmpsStr, 0, cmpsPos, x, y, mPaint);
    x += (mPaint->MeasureTextEx(cmpsStr, 0, cmpsPos, &value), value);
    if (cursorPos <= activeCmpsLen) {
        if (ComposingStatus_EDIT_PINYIN == mComposingStatus) {
            DrawCursor(canvas, x);
        }
        canvas->DrawTextEx2(cmpsStr, cmpsPos, activeCmpsLen, x, y, mPaint);
    }

    x += (mPaint->MeasureTextEx(cmpsStr, cmpsPos, activeCmpsLen, &value), value);

    if ((Int32)cmpsStr.GetLength() > activeCmpsLen) {
        mPaint->SetColor(mStrColorIdle);
        Int32 oriPos = activeCmpsLen;
        if (cursorPos > activeCmpsLen) {
            if (cursorPos > (Int32)cmpsStr.GetLength()) cursorPos = cmpsStr.GetLength();
            canvas->DrawTextEx2(cmpsStr, oriPos, cursorPos, x, y, mPaint);
            x += (mPaint->MeasureTextEx(cmpsStr, oriPos, cursorPos, &value), value);

            if (ComposingStatus_EDIT_PINYIN == mComposingStatus) {
                DrawCursor(canvas, x);
            }

            oriPos = cursorPos;
        }
        canvas->DrawTextEx2(cmpsStr, oriPos, cmpsStr.GetLength(), x, y, mPaint);
    }
}


IVIEW_METHODS_IMPL(CComposingView, ComposingView);
IDRAWABLECALLBACK_METHODS_IMPL(CComposingView, ComposingView);
IKEYEVENTCALLBACK_METHODS_IMPL(CComposingView, ComposingView);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CComposingView, ComposingView);

PInterface CComposingView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == Elastos::Droid::View::EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CComposingView::Probe(riid);
}

ECode CComposingView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);

    AutoPtr<IResources> r;
    context->GetResources((IResources**)&r);
    r->GetDrawable(0x7f02000d/*R.drawable.composing_hl_bg*/, (IDrawable**)&mHlDrawable);
    r->GetDrawable(0x7f02000c/*R.drawable.composing_area_cursor*/, (IDrawable**)&mCursor);

    r->GetColor(0x7f070007/*R.color.composing_color*/, &mStrColor);
    r->GetColor(0x7f070008/*R.color.composing_color_hl*/, &mStrColorHl);
    r->GetColor(0x7f070009/*R.color.composing_color_idle*/, &mStrColorIdle);

    r->GetDimensionPixelSize(0x7f080002/*R.dimen.composing_height*/, &mFontSize);

    CPaint::New((IPaint**)&mPaint);
    mPaint->SetColor(mStrColor);
    mPaint->SetAntiAlias(TRUE);
    mPaint->SetTextSize(mFontSize);

    return mPaint->GetFontMetricsIntEx((IPaintFontMetricsInt**)&mFmi);
}

ECode CComposingView::Reset()
{
    mComposingStatus = ComposingStatus_SHOW_PINYIN;
    return NOERROR;
}

ECode CComposingView::SetDecodingInfo(
    /* [in] */ IDecodingInfo* decInfo,
    /* [in] */ ImeState imeStatus)
{
    mDecInfo = decInfo;

    if (ImeState_STATE_INPUT == imeStatus) {
        mComposingStatus = ComposingStatus_SHOW_PINYIN;
        mDecInfo->MoveCursorToEdge(FALSE);
    } else {
        Int32 value = 0;
        if ((decInfo->GetFixedLen(&value), value) != 0
                || ComposingStatus_EDIT_PINYIN == mComposingStatus) {
            mComposingStatus = ComposingStatus_EDIT_PINYIN;
        } else {
            mComposingStatus = ComposingStatus_SHOW_STRING_LOWERCASE;
        }
        mDecInfo->MoveCursor(0);
    }

    Measure(IViewGroupLayoutParams::WRAP_CONTENT, IViewGroupLayoutParams::WRAP_CONTENT);
    RequestLayout();
    return InvalidateEx2();
}

ECode CComposingView::MoveCursor(
    /* [in] */ Int32 keyCode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (keyCode != IKeyEvent::KEYCODE_DPAD_LEFT
            && keyCode != IKeyEvent::KEYCODE_DPAD_RIGHT) {
        *result = FALSE;
        return NOERROR;
    }

    if (ComposingStatus_EDIT_PINYIN == mComposingStatus) {
        Int32 offset = 0;
        if (keyCode == IKeyEvent::KEYCODE_DPAD_LEFT)
            offset = -1;
        else if (keyCode == IKeyEvent::KEYCODE_DPAD_RIGHT) offset = 1;
        mDecInfo->MoveCursor(offset);
    } else if (ComposingStatus_SHOW_STRING_LOWERCASE == mComposingStatus) {
        if (keyCode == IKeyEvent::KEYCODE_DPAD_LEFT
                || keyCode == IKeyEvent::KEYCODE_DPAD_RIGHT) {
            mComposingStatus = ComposingStatus_EDIT_PINYIN;

            Measure(IViewGroupLayoutParams::WRAP_CONTENT, IViewGroupLayoutParams::WRAP_CONTENT);
            RequestLayout();
        }

    }

    InvalidateEx2();
    *result = TRUE;
    return NOERROR;
}

ECode CComposingView::GetComposingStatus(
    /* [out] */ ComposingStatus* status)
{
    VALIDATE_NOT_NULL(status);
    *status = mComposingStatus;
    return NOERROR;
}

} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos
