
#ifndef  __KEYFRAMESET_H__
#define  __KEYFRAMESET_H__

#include "animation/Keyframe.h"
#include "animation/KeyframeSetBase.h"

namespace Elastos {
namespace Droid {
namespace Animation {


/**
 * This class holds a collection of Keyframe objects and is called by ValueAnimator to calculate
 * values between those keyframes for a given animation. The class internal to the animation
 * package because it is an implementation detail of how Keyframes are stored and used.
 */
class KeyframeSet
        : public IKeyframeSet
        , public KeyframeSetBase
        , public ElRefBase
{
public:
    CAR_INTERFACE_DECL()

    KeyframeSet(
        /* [in] */ ArrayOf<IKeyframe*>* keyframes);

    static CARAPI_(AutoPtr<IKeyframeSet>) OfInt32(
        /* [in] */ ArrayOf<Int32>* values);

    static CARAPI_(AutoPtr<IKeyframeSet>) OfFloat(
        /* [in] */ ArrayOf<Float>* values);

    static CARAPI_(AutoPtr<IKeyframeSet>) OfKeyframe(
        /* [in] */ ArrayOf<IKeyframe*>* keyframes);

    static CARAPI_(AutoPtr<IKeyframeSet>) OfObject(
        /* [in] */ ArrayOf<IInterface*>* values);
    /**
     * Sets the TypeEvaluator to be used when calculating animated values. This object
     * is required only for KeyframeSets that are not either IntKeyframeSet or FloatKeyframeSet,
     * both of which assume their own evaluator to speed up calculations with those primitive
     * types.
     *
     * @param evaluator The TypeEvaluator to be used to calculate animated values.
     */
    CARAPI SetEvaluator(
        /* [in] */ ITypeEvaluator* evaluator);

    /**
     * Gets the animated value, given the elapsed fraction of the animation (interpolated by the
     * animation's interpolator) and the evaluator used to calculate in-between values. This
     * function maps the input fraction to the appropriate keyframe interval and a fraction
     * between them and returns the interpolated value. Note that the input fraction may fall
     * outside the [0-1] bounds, if the animation's interpolator made that happen (e.g., a
     * spring interpolation that might send the fraction past 1.0). We handle this situation by
     * just using the two keyframes at the appropriate end when the value is outside those bounds.
     *
     * @param fraction The elapsed fraction of the animation
     * @return The animated value.
     */
    CARAPI GetValue(
        /* [in] */ Float fraction,
        /* [out] */ IInterface** value);

    CARAPI Clone(
        /* [out] */ IKeyframeSet** obj);

    CARAPI GetKeyframes(
        /* [out, callee] */ ArrayOf<IKeyframe*>** frames);
};


}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
#endif  //__KEYFRAMESET_H__
