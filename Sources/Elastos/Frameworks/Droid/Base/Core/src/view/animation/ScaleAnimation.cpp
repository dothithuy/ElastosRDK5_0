
#include "view/animation/CScaleAnimation.h"
#include "util/CTypedValue.h"

using Elastos::Droid::Utility::CTypedValue;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

ScaleAnimation::ScaleAnimation()
    : mFromX(0.f)
    , mToX(0.f)
    , mFromY(0.f)
    , mToY(0.f)
    , mFromXType(ITypedValue::TYPE_NULL)
    , mToXType(ITypedValue::TYPE_NULL)
    , mFromYType(ITypedValue::TYPE_NULL)
    , mToYType(ITypedValue::TYPE_NULL)
    , mFromXData(0)
    , mToXData(0)
    , mFromYData(0)
    , mToYData(0)
    , mPivotXType(IAnimation::ABSOLUTE)
    , mPivotYType(IAnimation::ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
    , mPivotX(0.f)
    , mPivotY(0.f)
{}

ScaleAnimation::ScaleAnimation(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mFromX(0.f)
    , mToX(0.f)
    , mFromY(0.f)
    , mToY(0.f)
    , mFromXType(ITypedValue::TYPE_NULL)
    , mToXType(ITypedValue::TYPE_NULL)
    , mFromYType(ITypedValue::TYPE_NULL)
    , mToYType(ITypedValue::TYPE_NULL)
    , mFromXData(0)
    , mToXData(0)
    , mFromYData(0)
    , mToYData(0)
    , mPivotXType(IAnimation::ABSOLUTE)
    , mPivotYType(IAnimation::ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
    , mPivotX(0.f)
    , mPivotY(0.f)
{
    Init(context, attrs);
}

ScaleAnimation::ScaleAnimation(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY)
    : mFromX(0.f)
    , mToX(0.f)
    , mFromY(0.f)
    , mToY(0.f)
    , mFromXType(ITypedValue::TYPE_NULL)
    , mToXType(ITypedValue::TYPE_NULL)
    , mFromYType(ITypedValue::TYPE_NULL)
    , mToYType(ITypedValue::TYPE_NULL)
    , mFromXData(0)
    , mToXData(0)
    , mFromYData(0)
    , mToYData(0)
    , mPivotXType(IAnimation::ABSOLUTE)
    , mPivotYType(IAnimation::ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
    , mPivotX(0.f)
    , mPivotY(0.f)
{
    Init(fromX, toX, fromY, toY);
}

ScaleAnimation::ScaleAnimation(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
    : mFromX(0.f)
    , mToX(0.f)
    , mFromY(0.f)
    , mToY(0.f)
    , mFromXType(ITypedValue::TYPE_NULL)
    , mToXType(ITypedValue::TYPE_NULL)
    , mFromYType(ITypedValue::TYPE_NULL)
    , mToYType(ITypedValue::TYPE_NULL)
    , mFromXData(0)
    , mToXData(0)
    , mFromYData(0)
    , mToYData(0)
    , mPivotXType(IAnimation::ABSOLUTE)
    , mPivotYType(IAnimation::ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
    , mPivotX(0.f)
    , mPivotY(0.f)
{
    Init(fromX, toX, fromY, toY, pivotX, pivotY);

    InitializePivotPoint();
}

ScaleAnimation::ScaleAnimation(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Float pivotYType,
    /* [in] */ Float pivotYValue)
    : mFromX(0.f)
    , mToX(0.f)
    , mFromY(0.f)
    , mToY(0.f)
    , mFromXType(ITypedValue::TYPE_NULL)
    , mToXType(ITypedValue::TYPE_NULL)
    , mFromYType(ITypedValue::TYPE_NULL)
    , mToYType(ITypedValue::TYPE_NULL)
    , mFromXData(0)
    , mToXData(0)
    , mFromYData(0)
    , mToYData(0)
    , mPivotXType(IAnimation::ABSOLUTE)
    , mPivotYType(IAnimation::ABSOLUTE)
    , mPivotXValue(0.0f)
    , mPivotYValue(0.0f)
    , mPivotX(0.f)
    , mPivotY(0.f)
{
    Init(fromX, toX, fromY, toY, pivotXType,
        pivotXValue, pivotYType, pivotYValue);

    InitializePivotPoint();
}

AutoPtr<IAnimation> ScaleAnimation::GetCloneInstance()
{
    AutoPtr<IScaleAnimation> result;
    ASSERT_SUCCEEDED(CScaleAnimation::New(0.f, 0.f, 0.f, 0.f, (IScaleAnimation**)&result));
    return result.Get();
}

AutoPtr<IAnimation> ScaleAnimation::Clone()
{
    AutoPtr<IAnimation> result = Animation::Clone();
    if(NULL == result->Probe(EIID_Animation) || NULL ==result->Probe(EIID_IScaleAnimation))
    {
        return NULL;
    }
    Animation* temp = (Animation*)result->Probe(EIID_Animation);
    ScaleAnimation* animation = (ScaleAnimation*)temp;
    animation->mResources = mResources;
    animation->mFromX = mFromX;
    animation->mToX = mToX;
    animation->mFromY = mFromY;
    animation->mToY = mToY;

    animation->mFromXType = mFromXType;
    animation->mToXType = mToXType;
    animation->mFromYType = mFromYType;
    animation->mToYType = mToYType;

    animation->mFromXData = mFromXData;
    animation->mToXData = mToXData;
    animation->mFromYData = mFromYData;
    animation->mToYData = mToYData;

    animation->mPivotXType = mPivotXType;
    animation->mPivotYType = mPivotYType;
    animation->mPivotXValue = mPivotXValue;
    animation->mPivotYValue = mPivotYValue;

    animation->mPivotX = mPivotX;
    animation->mPivotY = mPivotX;
    return result;
}

void ScaleAnimation::ApplyTransformation(
    /* [in] */ Float interpolatedTime,
    /* [in] */ ITransformation* t)
{
    Float sx = 1.0f;
    Float sy = 1.0f;
    Float scale = GetScaleFactor();

    if (mFromX != 1.0f || mToX != 1.0f) {
        sx = mFromX + ((mToX - mFromX) * interpolatedTime);
    }
    if (mFromY != 1.0f || mToY != 1.0f) {
        sy = mFromY + ((mToY - mFromY) * interpolatedTime);
    }

    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    if (mPivotX == 0.0f && mPivotY == 0.0f) {
        matrix->SetScaleEx(sx, sy);
    }
    else {
        matrix->SetScale(sx, sy, scale * mPivotX, scale * mPivotY);
    }
}

ECode ScaleAnimation::Initialize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 parentWidth,
    /* [in] */ Int32 parentHeight)
{
    Animation::Initialize(width, height, parentWidth, parentHeight);

    mFromX = ResolveScale(mFromX, mFromXType, mFromXData, width, parentWidth);
    mToX = ResolveScale(mToX, mToXType, mToXData, width, parentWidth);
    mFromY = ResolveScale(mFromY, mFromYType, mFromYData, height, parentHeight);
    mToY = ResolveScale(mToY, mToYType, mToYData, height, parentHeight);

    mPivotX = ResolveSize(mPivotXType, mPivotXValue, width, parentWidth);
    mPivotY = ResolveSize(mPivotYType, mPivotYValue, height, parentHeight);

    return NOERROR;
}

ECode ScaleAnimation::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(Animation::Init(context, attrs));

    context->GetResources((IResources**)&mResources);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ScaleAnimation),
            ARRAY_SIZE(R::styleable::ScaleAnimation));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(attrs, attrIds, (ITypedArray**)&a);

    AutoPtr<ITypedValue> tv;
    a->PeekValue(
            R::styleable::ScaleAnimation_fromXScale, (ITypedValue**)&tv);
    mFromX = 0.0f;
    if (tv != NULL) {
        Int32 type = 0;
        tv->GetType(&type);
        if (type == ITypedValue::TYPE_FLOAT) {
            // This is a scaling factor.
            tv->GetFloat(&mFromX);
        } else {
            mFromXType = type;
            tv->GetData(&mFromXData);
        }
    }

    tv = NULL;
    a->PeekValue(
            R::styleable::ScaleAnimation_toXScale, (ITypedValue**)&tv);
    mToX = 0.0f;
    if (tv != NULL) {
        Int32 type = 0;
        tv->GetType(&type);
        if (type == ITypedValue::TYPE_FLOAT) {
            // This is a scaling factor.
            tv->GetFloat(&mToX);
        } else {
            mToXType = type;
            tv->GetData(&mToXData);
        }
    }

    tv = NULL;
    a->PeekValue(
            R::styleable::ScaleAnimation_fromYScale, (ITypedValue**)&tv);
    mFromY = 0.0f;
    if (tv != NULL) {
        Int32 type = 0;
        tv->GetType(&type);
        if (type == ITypedValue::TYPE_FLOAT) {
            // This is a scaling factor.
            tv->GetFloat(&mFromY);
        } else {
            mFromYType = type;
            tv->GetData(&mFromYData);
        }
    }

    tv = NULL;
    a->PeekValue(
            R::styleable::ScaleAnimation_toYScale, (ITypedValue**)&tv);
    mToY = 0.0f;
    if (tv != NULL) {
        Int32 type = 0;
        tv->GetType(&type);
        if (type == ITypedValue::TYPE_FLOAT) {
            // This is a scaling factor.
            tv->GetFloat(&mToY);
        } else {
            mToYType = type;
            tv->GetData(&mToYData);
        }
    }

    AutoPtr<ITypedValue> value;
    a->PeekValue(R::styleable::ScaleAnimation_pivotX, (ITypedValue**)&value);
    AutoPtr<Animation::Description> d = Animation::Description::ParseValue(value);
    mPivotXType = d->mType;
    mPivotXValue = d->mValue;

    value = NULL;
    a->PeekValue(R::styleable::ScaleAnimation_pivotY, (ITypedValue**)&value);
    d = Description::ParseValue(value);
    mPivotYType = d->mType;
    mPivotYValue = d->mValue;

    a->Recycle();

    InitializePivotPoint();

    return NOERROR;
}

ECode ScaleAnimation::Init(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY)
{
    mFromX = fromX;
    mToX = toX;
    mFromY = fromY;
    mToY = toY;
    mPivotX = 0.0f;
    mPivotY = 0.0f;

    return NOERROR;
}

ECode ScaleAnimation::Init(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
{
    mFromX = fromX;
    mToX = toX;
    mFromY = fromY;
    mToY = toY;

    mPivotXType = IAnimation::ABSOLUTE;
    mPivotYType = IAnimation::ABSOLUTE;
    mPivotXValue = pivotX;
    mPivotYValue = pivotY;
    InitializePivotPoint();

    return NOERROR;
}

ECode ScaleAnimation::Init(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Float pivotYType,
    /* [in] */ Float pivotYValue)
{
    mFromX = fromX;
    mToX = toX;
    mFromY = fromY;
    mToY = toY;

    mPivotXValue = pivotXValue;
    mPivotXType = pivotXType;
    mPivotYValue = pivotYValue;
    mPivotYType = pivotYType;
    InitializePivotPoint();

    return NOERROR;
}

Float ScaleAnimation::ResolveScale(
    /* [in] */ Float scale,
    /* [in] */ Int32 type,
    /* [in] */ Int32 data,
    /* [in] */ Int32 size,
    /* [in] */ Int32 psize)
{
    Float targetSize;
    if (type == ITypedValue::TYPE_FRACTION) {
        targetSize = CTypedValue::ComplexToFraction(data, size, psize);
    } else if (type == ITypedValue::TYPE_DIMENSION) {
        AutoPtr<IDisplayMetrics> dis;
        mResources->GetDisplayMetrics((IDisplayMetrics**)&dis);
        targetSize = CTypedValue::ComplexToDimension(data, dis);
    } else {
        return scale;
    }

    if (size == 0) {
        return 1;
    }

    return targetSize/(Float)size;
}

void ScaleAnimation::InitializePivotPoint()
{
    if (mPivotXType == IAnimation::ABSOLUTE) {
        mPivotX = mPivotXValue;
    }
    if (mPivotYType == IAnimation::ABSOLUTE) {
        mPivotY = mPivotYValue;
    }
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
