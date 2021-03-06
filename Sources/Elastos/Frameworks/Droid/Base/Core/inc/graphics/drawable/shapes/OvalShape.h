
#ifndef __OVALSHAPE_H__
#define __OVALSHAPE_H__

#include "graphics/drawable/shapes/RectShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

/**
 * Defines an oval shape.
 * The oval can be drawn to a Canvas with its own draw() method,
 * but more graphical control is available if you instead pass
 * the OvalShape to a {@link android.graphics.drawable.ShapeDrawable}.
 */
class OvalShape : public RectShape
{
public:
    /**
     * OvalShape constructor.
     */
    OvalShape();

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IPaint* paint);

protected:
    CARAPI Init();

    CARAPI_(void) Clone(
        /* [in] */ OvalShape* other);
};

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__OVALSHAPE_H__
