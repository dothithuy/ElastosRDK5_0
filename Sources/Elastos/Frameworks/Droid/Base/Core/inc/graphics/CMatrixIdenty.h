
#ifndef __CMATRIXIDENTY_H__
#define __CMATRIXIDENTY_H__

#include "_CMatrixIdenty.h"
#include "graphics/CMatrix.h"
#include <SkMatrix.h>
#include "graphics/Matrix.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

class Canvas;

class CMatrixIdenty : public Matrix {

    friend class Canvas;
    friend class Shader;
    friend class CPath;

public:
    void Oops();
    virtual CARAPI Set(
        /* [in] */ IMatrix* src);
    virtual CARAPI Reset();
    virtual CARAPI SetTranslate(
        /* [in] */ Float dx,
        /* [in] */ Float dy);
    virtual CARAPI SetScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py);
    virtual CARAPI SetScaleEx(
        /* [in] */ Float sx,
        /* [in] */ Float sy);
    virtual CARAPI SetRotate(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py);
    virtual CARAPI SetRotateEx(
        /* [in] */ Float degrees);
    virtual CARAPI SetSinCos(
        /* [in] */ Float sinValue,
        /* [in] */ Float cosValue,
        /* [in] */ Float px,
        /* [in] */ Float py);
    virtual CARAPI SetSinCosEx(
        /* [in] */ Float sinValue,
        /* [in] */ Float cosValue);
    virtual CARAPI SetSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py);
    virtual CARAPI SetSkewEx(
        /* [in] */ Float kx,
        /* [in] */ Float ky);
    virtual CARAPI SetConcat(
        /* [in] */ IMatrix* a,
        /* [in] */ IMatrix* b,
        /* [out] */ Boolean* result);
    virtual CARAPI PreTranslate(
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [out] */ Boolean* result);
    virtual CARAPI PreScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);
    virtual CARAPI PreScaleEx(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [out] */ Boolean* result);
    virtual CARAPI PreRotate(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);
    virtual CARAPI PreRotateEx(
        /* [in] */ Float degrees,
        /* [out] */ Boolean* result);
    virtual CARAPI PreSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);
    virtual CARAPI PreSkewEx(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [out] */ Boolean* result);
    virtual CARAPI PreConcat(
        /* [in] */ IMatrix* other,
        /* [out] */ Boolean* result);
    virtual CARAPI PostTranslate(
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [out] */ Boolean* result);
    virtual CARAPI PostScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);
    virtual CARAPI PostScaleEx(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [out] */ Boolean* result);
    virtual CARAPI PostRotate(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);
    virtual CARAPI PostRotateEx(
        /* [in] */ Float degrees,
        /* [out] */ Boolean* result);
    virtual CARAPI PostSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);
    virtual CARAPI PostSkewEx(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [out] */ Boolean* result);
    virtual CARAPI PostConcat(
        /* [in] */ IMatrix* other,
        /* [out] */ Boolean* result);
    virtual CARAPI SetRectToRect(
        /* [in] */ IRectF* src,
        /* [in] */ IRectF* dst,
        /* [in] */ MatrixScaleToFit stf,
        /* [out] */ Boolean* result);
    virtual CARAPI SetPolyToPoly(
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcIndex,
        /* [in] */ const ArrayOf<Float>& dst,
        /* [in] */ Int32 dstIndex,
        /* [in] */ Int32 pointCount,
        /* [out] */ Boolean* result);
    virtual CARAPI SetValues(
        /* [in] */ const ArrayOf<Float>& values);
};

#endif //__CMATRIXIDENTY_H__
