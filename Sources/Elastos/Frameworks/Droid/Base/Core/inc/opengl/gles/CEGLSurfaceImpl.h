
#ifndef __CEGLSURFACEIMPL_H__
#define __CEGLSURFACEIMPL_H__

#include "_CEGLSurfaceImpl.h"

namespace Elastos {
namespace Droid {
namespace Opengl {
namespace Gles {
class CEGLImpl;

CarClass(CEGLSurfaceImpl)
{
    friend class CEGLImpl;
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 surface);

    CARAPI Equals(
        /* [in] */ IInterface *object,
        /* [out] */ Boolean *equals);

    CARAPI GetHashCode(
        /* [out] */ Int32 *hashCode);

    CARAPI_(Int32) GetEGLSurface();

private:
    Int32 mEGLSurface;
    Int32 mNativePixelRef;
};

} // namespace Gles
} // namespace Opengl
} // namespace Droid
} // namespace Elastos

#endif // __CEGLSURFACEIMPL_H__
