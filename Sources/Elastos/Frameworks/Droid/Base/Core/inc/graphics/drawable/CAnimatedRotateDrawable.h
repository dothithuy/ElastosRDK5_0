
#ifndef __CANIMATEDROTATEDRAWABLE_H__
#define __CANIMATEDROTATEDRAWABLE_H__

#include "_CAnimatedRotateDrawable.h"
#include "graphics/drawable/AnimatedRotateDrawable.h"
#include "graphics/drawable/DrawableMacro.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CAnimatedRotateDrawable), AnimatedRotateDrawable
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IDrawableConstantState* state,
        /* [in] */ IResources* res);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    CARAPI Start();

    CARAPI Stop();

    CARAPI IsRunning(
        /* [out] */ Boolean* isRunning);

    CARAPI Run();

    CARAPI GetDrawable(
        /* [out] */ IDrawable** drawable);

    CARAPI SetFramesCount(
        /* [in] */ Int32 framesCount);

    CARAPI SetFramesDuration(
        /* [in] */ Int32 framesDuration);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __CANIMATEDROTATEDRAWABLE_H__

