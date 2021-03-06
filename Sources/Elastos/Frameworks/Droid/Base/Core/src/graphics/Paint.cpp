
#include "graphics/Paint.h"
#include "graphics/CPath.h"
#include "graphics/CPaintFontMetrics.h"
#include "graphics/CPaintFontMetricsInt.h"
#include "graphics/Shader.h"
#include "graphics/ColorFilter.h"
#include "graphics/Xfermode.h"
#include "graphics/PathEffect.h"
#include "graphics/MaskFilter.h"
#include "graphics/Typeface.h"
#include "graphics/Rasterizer.h"
#include "graphics/TemporaryBuffer.h"
#include "graphics/TextLayout.h"
#include "graphics/TextLayoutCache.h"
#include "graphics/GraphicsNative.h"
#include "text/TextUtils.h"
#include <elastos/Math.h>
#include <elastos/Logger.h>
#include <Elastos.Core.h>
#include <skia/effects/SkBlurDrawLooper.h>
#include <skia/core/SkPaint.h>
#include <unicode/uloc.h>
#include <elastos/Character.h>

using Elastos::Core::Character;
using Elastos::Core::IString;
using Elastos::Utility::Logging::Logger;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;

using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::ISpannedString;
using Elastos::Droid::Text::ISpannableString;
using Elastos::Droid::Text::IGraphicsOperations;

namespace Elastos {
namespace Droid {
namespace Graphics {

static const String TAG("Paint");

// {4407AE9E-A61A-4FE3-8F99-12BFFDD68E56}
extern const InterfaceID EIID_Paint =
    { 0x4407ae9e, 0xa61a, 0x4fe3, { 0x8f, 0x99, 0x12, 0xbf, 0xfd, 0xd6, 0x8e, 0x56 } };

static void DefaultSettingsForElastos(SkPaint* paint)
{
    // GlyphID encoding is required because we are using Harfbuzz shaping
    paint->setTextEncoding(SkPaint::kGlyphID_TextEncoding);
}

const PaintStyle Paint::sStyleArray[] = {
    PaintStyle_FILL, PaintStyle_STROKE, PaintStyle_FILL_AND_STROKE
};
const PaintCap Paint::sCapArray[] = {
    PaintCap_BUTT, PaintCap_ROUND, PaintCap_SQUARE
};
const PaintJoin Paint::sJoinArray[] = {
    PaintJoin_MITER, PaintJoin_ROUND, PaintJoin_BEVEL
};
const PaintAlign Paint::sAlignArray[] = {
    PaintAlign_LEFT, PaintAlign_CENTER, PaintAlign_RIGHT
};

AutoPtr<ILocale> Paint::GetDefaultLocale()
{
    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    AutoPtr<ILocale> locale;
    helper->GetDefault((ILocale**)&locale);
    return locale;
}

const Int32 Paint::DEFAULT_PAINT_FLAGS;
const Int32 Paint::BIDI_MAX_FLAG_VALUE;
const Int32 Paint::BIDI_FLAG_MASK;
const Int32 Paint::CURSOR_OPT_MAX_VALUE;

Paint::Paint()
    : mNativePaint(0)
    , mHasShadow(FALSE)
    , mShadowDx(0)
    , mShadowDy(0)
    , mShadowRadius(0)
    , mShadowColor(0)
    , mBidiFlags(IPaint::BIDI_DEFAULT_LTR)
    , mHasCompatScaling(FALSE)
    , mCompatScaling(0)
    , mInvCompatScaling(0)
{}

Paint::~Paint()
{
    NativeFinalizer(mNativePaint);
}

/**
 * Create a new paint with default settings.
 */
ECode Paint::Init()
{
    return Init(0);
}

/**
 * Create a new paint with the specified flags. Use setFlags() to change
 * these after the paint is created.
 *
 * @param flags initial flag bits, as if they were passed via setFlags().
 */
ECode Paint::Init(
    /* [in] */ Int32 flags)
{
    mNativePaint = NativeInit();
    SetFlags(flags | DEFAULT_PAINT_FLAGS);
    // TODO: Turning off hinting has undesirable side effects, we need to
    //       revisit hinting once we add support for subpixel positioning
    // setHinting(DisplayMetrics.DENSITY_DEVICE >= DisplayMetrics.DENSITY_TV
    //        ? HINTING_OFF : HINTING_ON);
    mCompatScaling = mInvCompatScaling = 1;
    AutoPtr<ILocale> locale = GetDefaultLocale();
    SetTextLocale(locale);
    return NOERROR;
}

ECode Paint::Init(
    /* [in] */ IPaint* paint)
{
    VALIDATE_NOT_NULL(paint);
    Paint* paint_ = (Paint*)paint->Probe(EIID_Paint);
    mNativePaint = NativeInitWithPaint(paint_->mNativePaint);
    SetClassVariablesFrom(paint_);
    return NOERROR;
}

/** Restores the paint to its default settings. */
ECode Paint::Reset()
{
    NativeReset(mNativePaint);
    SetFlags(DEFAULT_PAINT_FLAGS);

    // TODO: Turning off hinting has undesirable side effects, we need to
    //       revisit hinting once we add support for subpixel positioning
    // setHinting(DisplayMetrics.DENSITY_DEVICE >= DisplayMetrics.DENSITY_TV
    //        ? HINTING_OFF : HINTING_ON);

    mColorFilter = NULL;
    mMaskFilter = NULL;
    mPathEffect = NULL;
    mRasterizer = NULL;
    mShader = NULL;
    mTypeface = NULL;
    mXfermode = NULL;

    mHasCompatScaling = FALSE;
    mCompatScaling = 1;

    mInvCompatScaling = 1;

    mHasShadow = FALSE;
    mShadowDx = 0;
    mShadowDy = 0;
    mShadowRadius = 0;
    mShadowColor = 0;

    mBidiFlags = IPaint::BIDI_DEFAULT_LTR;
    AutoPtr<ILocale> locale = GetDefaultLocale();
    SetTextLocale(locale);
    return NOERROR;
}

/**
 * Copy the fields from src into this paint. This is equivalent to calling
 * get() on all of the src fields, and calling the corresponding set()
 * methods on this.
 */
ECode Paint::Set(
    /* [in] */ IPaint* src)
{
    assert(src != NULL);
    Paint* src_ = (Paint*)src->Probe(EIID_Paint);
    if (this != src_) {
        assert(src_ != NULL);
        // copy over the native settings
        NativeSet(mNativePaint, src_->mNativePaint);
        SetClassVariablesFrom(src_);
    }
    return NOERROR;
}

void Paint::SetClassVariablesFrom(
    /* [in] */ Paint* paint)
{
    mColorFilter = paint->mColorFilter;
    mMaskFilter = paint->mMaskFilter;
    mPathEffect = paint->mPathEffect;
    mRasterizer = paint->mRasterizer;
    mShader = paint->mShader;
    mTypeface = paint->mTypeface;
    mXfermode = paint->mXfermode;

    mHasCompatScaling = paint->mHasCompatScaling;
    mCompatScaling = paint->mCompatScaling;
    mInvCompatScaling = paint->mInvCompatScaling;

    mHasShadow = paint->mHasShadow;
    mShadowDx = paint->mShadowDx;
    mShadowDy = paint->mShadowDy;
    mShadowRadius = paint->mShadowRadius;
    mShadowColor = paint->mShadowColor;

    mBidiFlags = paint->mBidiFlags;
    mLocale = paint->mLocale;
}

/** @hide */
ECode Paint::SetCompatibilityScaling(
    /* [in] */ Float factor)
{
    if (factor == 1.0) {
        mHasCompatScaling = FALSE;
        mCompatScaling = mInvCompatScaling = 1.0f;
    }
    else {
        mHasCompatScaling = TRUE;
        mCompatScaling = factor;
        mInvCompatScaling = 1.0f/factor;
    }
    return NOERROR;
}

ECode Paint::GetBidiFlags(
    /* [out] */ Int32* flags)
{
    *flags = mBidiFlags;
    return NOERROR;
}

ECode Paint::SetBidiFlags(
    /* [in] */ Int32 flags)
{
    // only flag value is the 3-bit BIDI control setting
    flags &= BIDI_FLAG_MASK;
    if (flags > BIDI_MAX_FLAG_VALUE) {
        // throw new IllegalArgumentException("unknown bidi flag: " + flags);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBidiFlags = flags;
    return NOERROR;
}


 /**
 * Return the paint's flags. Use the Flag enum to test flag values.
 *
 * @return the paint's flags (see enums ending in _Flag for bit masks)
 */
ECode Paint::GetFlags(
    /* [out] */ Int32* flags)
{
    *flags = ((SkPaint*)mNativePaint)->getFlags();
    return NOERROR;
}

/**
 * Return the paint's flags. Use the Flag enum to test flag values.
 *
 * @return the paint's flags (see enums ending in _Flag for bit masks)
 */
ECode Paint::SetFlags(
    /* [in] */ Int32 flags)
{
    ((SkPaint*)mNativePaint)->setFlags(flags);
    return NOERROR;
}

ECode Paint::GetHinting(
    /* [out] */ Int32* mode)
{
    *mode = ((SkPaint*)mNativePaint)->getHinting() == SkPaint::kNo_Hinting ? 0 : 1;
    return NOERROR;
}

ECode Paint::SetHinting(
    /* [in] */ Int32 mode)
{
    ((SkPaint*)mNativePaint)->setFlags(mode == 0 ? SkPaint::kNo_Hinting : SkPaint::kSlight_Hinting);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if ANTI_ALIAS_FLAG bit is set
 * AntiAliasing smooths out the edges of what is being drawn, but is has
 * no impact on the interior of the shape. See setDither() and
 * setFilterBitmap() to affect how colors are treated.
 *
 * @return true if the antialias bit is set in the paint's flags.
 */
ECode Paint::IsAntiAlias(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::ANTI_ALIAS_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the ANTI_ALIAS_FLAG bit
 * AntiAliasing smooths out the edges of what is being drawn, but is has
 * no impact on the interior of the shape. See setDither() and
 * setFilterBitmap() to affect how colors are treated.
 *
 * @param aa true to set the antialias bit in the flags, false to clear it
 */
ECode Paint::SetAntiAlias(
    /* [in] */ Boolean aa)
{
    ((SkPaint*)mNativePaint)->setAntiAlias(aa);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if DITHER_FLAG bit is set
 * Dithering affects how colors that are higher precision than the device
 * are down-sampled. No dithering is generally faster, but higher precision
 * colors are just truncated down (e.g. 8888 -> 565). Dithering tries to
 * distribute the error inherent in this process, to reduce the visual
 * artifacts.
 *
 * @return true if the dithering bit is set in the paint's flags.
 */
ECode Paint::IsDither(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::DITHER_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the DITHER_FLAG bit
 * Dithering affects how colors that are higher precision than the device
 * are down-sampled. No dithering is generally faster, but higher precision
 * colors are just truncated down (e.g. 8888 -> 565). Dithering tries to
 * distribute the error inherent in this process, to reduce the visual
 * artifacts.
 *
 * @param dither true to set the dithering bit in flags, false to clear it
 */
ECode Paint::SetDither(
    /* [in] */ Boolean dither)
{
    ((SkPaint*)mNativePaint)->setDither(dither);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if LINEAR_TEXT_FLAG bit is set
 *
 * @return true if the lineartext bit is set in the paint's flags
 */
ECode Paint::IsLinearText(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::LINEAR_TEXT_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the LINEAR_TEXT_FLAG bit
 *
 * @param linearText true to set the linearText bit in the paint's flags,
 *                   false to clear it.
 */
ECode Paint::SetLinearText(
    /* [in] */ Boolean linearText)
{
    ((SkPaint*)mNativePaint)->setLinearText(linearText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if SUBPIXEL_TEXT_FLAG bit is set
 *
 * @return true if the subpixel bit is set in the paint's flags
 */
ECode Paint::IsSubpixelText(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::SUBPIXEL_TEXT_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the SUBPIXEL_TEXT_FLAG bit
 *
 * @param subpixelText true to set the subpixelText bit in the paint's
 *                     flags, false to clear it.
 */
ECode Paint::SetSubpixelText(
    /* [in] */ Boolean subpixelText)
{
    ((SkPaint*)mNativePaint)->setSubpixelText(subpixelText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if UNDERLINE_TEXT_FLAG bit is set
 *
 * @return true if the underlineText bit is set in the paint's flags.
 */
ECode Paint::IsUnderlineText(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::UNDERLINE_TEXT_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the UNDERLINE_TEXT_FLAG bit
 *
 * @param underlineText true to set the underlineText bit in the paint's
 *                      flags, false to clear it.
 */
ECode Paint::SetUnderlineText(
    /* [in] */ Boolean underlineText)
{
    ((SkPaint*)mNativePaint)->setUnderlineText(underlineText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if STRIKE_THRU_TEXT_FLAG bit is set
 *
 * @return true if the strikeThruText bit is set in the paint's flags.
 */
ECode Paint::IsStrikeThruText(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::STRIKE_THRU_TEXT_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the STRIKE_THRU_TEXT_FLAG bit
 *
 * @param strikeThruText true to set the strikeThruText bit in the paint's
 *                       flags, false to clear it.
 */
ECode Paint::SetStrikeThruText(
    /* [in] */ Boolean strikeThruText)
{
    ((SkPaint*)mNativePaint)->setStrikeThruText(strikeThruText);
    return NOERROR;
}

/**
 * Helper for getFlags(), returning true if FAKE_BOLD_TEXT_FLAG bit is set
 *
 * @return true if the fakeBoldText bit is set in the paint's flags.
 */
ECode Paint::IsFakeBoldText(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::FAKE_BOLD_TEXT_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the STRIKE_THRU_TEXT_FLAG bit
 *
 * @param fakeBoldText true to set the fakeBoldText bit in the paint's
 *                     flags, false to clear it.
 */
ECode Paint::SetFakeBoldText(
    /* [in] */ Boolean fakeBoldText)
{
    ((SkPaint*)mNativePaint)->setFakeBoldText(fakeBoldText);
    return NOERROR;
}

/**
 * Whether or not the bitmap filter is activated.
 * Filtering affects the sampling of bitmaps when they are transformed.
 * Filtering does not affect how the colors in the bitmap are converted into
 * device pixels. That is dependent on dithering and xfermodes.
 *
 * @see #setFilterBitmap(boolean) setFilterBitmap()
 */
ECode Paint::IsFilterBitmap(
    /* [out] */ Boolean* result)
{
    Int32 flags;
    GetFlags(&flags);
    *result = (flags & IPaint::FILTER_BITMAP_FLAG) != 0;
    return NOERROR;
}

/**
 * Helper for setFlags(), setting or clearing the FILTER_BITMAP_FLAG bit.
 * Filtering affects the sampling of bitmaps when they are transformed.
 * Filtering does not affect how the colors in the bitmap are converted into
 * device pixels. That is dependent on dithering and xfermodes.
 *
 * @param filter true to set the FILTER_BITMAP_FLAG bit in the paint's
 *               flags, false to clear it.
 */
ECode Paint::SetFilterBitmap(
    /* [in] */ Boolean filterBitmap)
{
    ((SkPaint*)mNativePaint)->setFilterBitmap(filterBitmap);
    return NOERROR;
}

/**
 * Return the paint's style, used for controlling how primitives'
 * geometries are interpreted (except for drawBitmap, which always assumes
 * FILL_STYLE).
 *
 * @return the paint's style setting (Fill, Stroke, StrokeAndFill)
 */
ECode Paint::GetStyle(
    /* [out] */ PaintStyle* style)
{
    *style = sStyleArray[NativeGetStyle(mNativePaint)];
    return NOERROR;
}

/**
 * Set the paint's style, used for controlling how primitives'
 * geometries are interpreted (except for drawBitmap, which always assumes
 * Fill).
 *
 * @param style The new style to set in the paint
 */
ECode Paint::SetStyle(
    /* [in] */ PaintStyle style)
{
    NativeSetStyle(mNativePaint, style);
    return NOERROR;
}

/**
 * Return the paint's color. Note that the color is a 32bit value
 * containing alpha as well as r,g,b. This 32bit value is not premultiplied,
 * meaning that its alpha can be any value, regardless of the values of
 * r,g,b. See the Color class for more details.
 *
 * @return the paint's color (and alpha).
 */
ECode Paint::GetColor(
    /* [out] */ Int32* color)
{
    *color = ((SkPaint*)mNativePaint)->getColor();
    return NOERROR;
}

/**
 * Set the paint's color. Note that the color is an int containing alpha
 * as well as r,g,b. This 32bit value is not premultiplied, meaning that
 * its alpha can be any value, regardless of the values of r,g,b.
 * See the Color class for more details.
 *
 * @param color The new color (including alpha) to set in the paint.
 */
ECode Paint::SetColor(
    /* [in] */ Int32 color)
{
    ((SkPaint*)mNativePaint)->setColor(color);
    return NOERROR;
}

/**
 * Helper to getColor() that just returns the color's alpha value. This is
 * the same as calling getColor() >>> 24. It always returns a value between
 * 0 (completely transparent) and 255 (completely opaque).
 *
 * @return the alpha component of the paint's color.
 */
ECode Paint::GetAlpha(
    /* [out] */ Int32* alpha)
{
    *alpha = ((SkPaint*)mNativePaint)->getAlpha();
    return NOERROR;
}

/**
 * Helper to setColor(), that only assigns the color's alpha value,
 * leaving its r,g,b values unchanged. Results are undefined if the alpha
 * value is outside of the range [0..255]
 *
 * @param a set the alpha component [0..255] of the paint's color.
 */
ECode Paint::SetAlpha(
    /* [in] */ Int32 a)
{
    ((SkPaint*)mNativePaint)->setAlpha(a);
    return NOERROR;
}

/**
 * Helper to setColor(), that takes a,r,g,b and constructs the color int
 *
 * @param a The new alpha component (0..255) of the paint's color.
 * @param r The new red component (0..255) of the paint's color.
 * @param g The new green component (0..255) of the paint's color.
 * @param b The new blue component (0..255) of the paint's color.
 */
ECode Paint::SetARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return SetColor((a << 24) | (r << 16) | (g << 8) | b);
}

/**
 * Return the width for stroking.
 * <p />
 * A value of 0 strokes in hairline mode.
 * Hairlines always draws a single pixel independent of the canva's matrix.
 *
 * @return the paint's stroke width, used whenever the paint's style is
 *         Stroke or StrokeAndFill.
 */
ECode Paint::GetStrokeWidth(
    /* [out] */ Float* width)
{
    *width = SkScalarToFloat(((SkPaint*)mNativePaint)->getStrokeWidth());
    return NOERROR;
}

/**
 * Set the width for stroking.
 * Pass 0 to stroke in hairline mode.
 * Hairlines always draws a single pixel independent of the canva's matrix.
 *
 * @param width set the paint's stroke width, used whenever the paint's
 *              style is Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeWidth(
    /* [in] */ Float width)
{
    ((SkPaint*)mNativePaint)->setStrokeWidth(SkFloatToScalar(width));
    return NOERROR;
}

/**
 * Return the paint's stroke miter value. Used to control the behavior
 * of miter joins when the joins angle is sharp.
 *
 * @return the paint's miter limit, used whenever the paint's style is
 *         Stroke or StrokeAndFill.
 */
ECode Paint::GetStrokeMiter(
    /* [out] */ Float* miter)
{
    *miter = SkScalarToFloat(((SkPaint*)mNativePaint)->getStrokeMiter());
    return NOERROR;
}

/**
 * Set the paint's stroke miter value. This is used to control the behavior
 * of miter joins when the joins angle is sharp. This value must be >= 0.
 *
 * @param miter set the miter limit on the paint, used whenever the paint's
 *              style is Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeMiter(
    /* [in] */ Float miter)
{
    ((SkPaint*)mNativePaint)->setStrokeMiter(SkFloatToScalar(miter));
    return NOERROR;
}

/**
 * Return the paint's Cap, controlling how the start and end of stroked
 * lines and paths are treated.
 *
 * @return the line cap style for the paint, used whenever the paint's
 *         style is Stroke or StrokeAndFill.
 */
ECode Paint::GetStrokeCap(
    /* [out] */ PaintCap* cap)
{
    *cap = sCapArray[NativeGetStrokeCap(mNativePaint)];
    return NOERROR;
}

/**
 * Set the paint's Cap.
 *
 * @param cap set the paint's line cap style, used whenever the paint's
 *            style is Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeCap(
    /* [in] */ PaintCap cap)
{
    NativeSetStrokeCap(mNativePaint, cap);
    return NOERROR;
}

/**
 * Return the paint's stroke join type.
 *
 * @return the paint's Join.
 */
ECode Paint::GetStrokeJoin(
    /* [out] */ PaintJoin* join)
{
    *join = sJoinArray[NativeGetStrokeJoin(mNativePaint)];
    return NOERROR;
}

/**
 * Set the paint's Join.
 *
 * @param join set the paint's Join, used whenever the paint's style is
 *             Stroke or StrokeAndFill.
 */
ECode Paint::SetStrokeJoin(
    /* [in] */ PaintJoin join)
{
    NativeSetStrokeJoin(mNativePaint, join);
    return NOERROR;
}

/**
 * Applies any/all effects (patheffect, stroking) to src, returning the
 * result in dst. The result is that drawing src with this paint will be
 * the same as drawing dst with a default paint (at least from the
 * geometric perspective).
 *
 * @param src input path
 * @param dst output path (may be the same as src)
 * @return    true if the path should be filled, or false if it should be
 *                 drawn with a hairline (width == 0)
 */
ECode Paint::GetFillPath(
    /* [in] */ IPath* src,
    /* [in] */ IPath* dst,
    /* [out] */ Boolean* result)
{
    *result = NativeGetFillPath(mNativePaint, ((CPath*)src)->Ni(), ((CPath*)dst)->Ni());
    return NOERROR;
}

/**
 * Get the paint's shader object.
 *
 * @return the paint's shader (or null)
 */
ECode Paint::GetShader(
    /* [out] */ IShader** shader)
{
    *shader = mShader;
    INTERFACE_ADDREF(*shader);
    return NOERROR;
}

/**
 * Set or clear the shader object.
 * <p />
 * Pass null to clear any previous shader.
 * As a convenience, the parameter passed is also returned.
 *
 * @param shader May be null. the new shader to be installed in the paint
 * @return       shader
 */
ECode Paint::SetShader(
    /* [in] */ IShader* shader)
{
    Int32 shaderNative = 0;
    if (shader != NULL) {
        Shader* s = (Shader*)shader->Probe(EIID_Shader);
        assert(s != NULL);
        shaderNative = s->mNativeInstance;
    }
    NativeSetShader(mNativePaint, shaderNative);
    mShader = shader;
    return NOERROR;
}

 /**
 * Get the paint's colorfilter (maybe be null).
 *
 * @return the paint's colorfilter (maybe be null)
 */
ECode Paint::GetColorFilter(
    /* [out] */ IColorFilter** filter)
{
    *filter = mColorFilter;
    INTERFACE_ADDREF(*filter);
    return NOERROR;
}

/**
 * Set or clear the paint's colorfilter, returning the parameter.
 *
 * @param filter May be null. The new filter to be installed in the paint
 * @return       filter
 */
ECode Paint::SetColorFilter(
    /* [in] */ IColorFilter* filter)
{
    Int32 filterNative = 0;
    if (filter != NULL) {
        ColorFilter* cf = (ColorFilter*)filter->Probe(EIID_ColorFilter);
        assert(cf != NULL);
        filterNative = cf->mNativeInstance;
    }
    NativeSetColorFilter(mNativePaint, filterNative);
    mColorFilter = filter;
    return NOERROR;
}

/**
 * Get the paint's xfermode object.
 *
 * @return the paint's xfermode (or null)
 */
ECode Paint::GetXfermode(
    /* [out] */ IXfermode** xfermode)
{
    *xfermode = mXfermode;
    INTERFACE_ADDREF(*xfermode);
    return NOERROR;
}

/**
 * Set or clear the xfermode object.
 * <p />
 * Pass null to clear any previous xfermode.
 * As a convenience, the parameter passed is also returned.
 *
 * @param xfermode May be null. The xfermode to be installed in the paint
 * @return         xfermode
 */
ECode Paint::SetXfermode(
    /* [in] */ IXfermode* xfermode)
{
    Int32 xfermodeNative = 0;
    if (xfermode != NULL) {
        Xfermode* x = (Xfermode*)xfermode->Probe(EIID_Xfermode);
        assert(x != NULL);
        xfermodeNative = x->mNativeInstance;
    }
    NativeSetXfermode(mNativePaint, xfermodeNative);
    mXfermode = xfermode;
    return NOERROR;
}

/**
 * Get the paint's patheffect object.
 *
 * @return the paint's patheffect (or null)
 */
ECode Paint::GetPathEffect(
    /* [out] */ IPathEffect** effect)
{
    *effect = mPathEffect;
    INTERFACE_ADDREF(*effect);
    return NOERROR;
}

/**
 * Set or clear the patheffect object.
 * <p />
 * Pass null to clear any previous patheffect.
 * As a convenience, the parameter passed is also returned.
 *
 * @param effect May be null. The patheffect to be installed in the paint
 * @return       effect
 */
ECode Paint::SetPathEffect(
    /* [in] */ IPathEffect* effect)
{
    Int32 effectNative = 0;
    if (effect != NULL) {
        PathEffect* pe = (PathEffect*)effect->Probe(EIID_PathEffect);
        assert(pe != NULL);
        effectNative = pe->mNativeInstance;
    }
    NativeSetPathEffect(mNativePaint, effectNative);
    mPathEffect = effect;
    return NOERROR;
}

/**
 * Get the paint's maskfilter object.
 *
 * @return the paint's maskfilter (or null)
 */
ECode Paint::GetMaskFilter(
    /* [out] */ IMaskFilter** maskfilter)
{
    *maskfilter = mMaskFilter;
    INTERFACE_ADDREF(*maskfilter)
    return NOERROR;
}

/**
 * Set or clear the maskfilter object.
 * <p />
 * Pass null to clear any previous maskfilter.
 * As a convenience, the parameter passed is also returned.
 *
 * @param maskfilter May be null. The maskfilter to be installed in the
 *                   paint
 * @return           maskfilter
 */
ECode Paint::SetMaskFilter(
    /* [in] */ IMaskFilter* maskfilter)
{
    Int32 maskfilterNative = 0;
    if (maskfilter != NULL) {
        MaskFilter* mf = (MaskFilter*)maskfilter->Probe(EIID_MaskFilter);
        assert(mf != NULL);
        maskfilterNative = mf->mNativeInstance;
    }
    NativeSetMaskFilter(mNativePaint, maskfilterNative);
    mMaskFilter = maskfilter;
    return NOERROR;
}

/**
 * Get the paint's typeface object.
 * <p />
 * The typeface object identifies which font to use when drawing or
 * measuring text.
 *
 * @return the paint's typeface (or null)
 */
ECode Paint::GetTypeface(
    /* [out] */ ITypeface** typeface)
{
    *typeface = mTypeface;
    INTERFACE_ADDREF(*typeface);
    return NOERROR;
}

/**
 * Set or clear the typeface object.
 * <p />
 * Pass null to clear any previous typeface.
 * As a convenience, the parameter passed is also returned.
 *
 * @param typeface May be null. The typeface to be installed in the paint
 * @return         typeface
 */
ECode Paint::SetTypeface(
    /* [in] */ ITypeface* typeface)
{
    Int32 typefaceNative = 0;
    if (typeface != NULL) {
        Typeface* t = (Typeface*)typeface->Probe(EIID_Typeface);
        typefaceNative = t->mNativeInstance;
    }
    NativeSetTypeface(mNativePaint, typefaceNative);
    mTypeface = typeface;
    return NOERROR;
}

/**
 * Get the paint's rasterizer (or null).
 * <p />
 * The raster controls/modifies how paths/text are turned into alpha masks.
 *
 * @return         the paint's rasterizer (or null)
 */
ECode Paint::GetRasterizer(
    /* [out] */ IRasterizer** rasterizer)
{
    *rasterizer = mRasterizer;
    INTERFACE_ADDREF(*rasterizer);
    return NOERROR;
}

/**
 * Set or clear the rasterizer object.
 * <p />
 * Pass null to clear any previous rasterizer.
 * As a convenience, the parameter passed is also returned.
 *
 * @param rasterizer May be null. The new rasterizer to be installed in
 *                   the paint.
 * @return           rasterizer
 */
ECode Paint::SetRasterizer(
    /* [in] */ IRasterizer* rasterizer)
{
    Int32 rasterizerNative = 0;
    if (rasterizer != NULL) {
        Rasterizer* r = (Rasterizer*)rasterizer->Probe(EIID_Rasterizer);
        assert(r != NULL);
        rasterizerNative = r->mNativeInstance;
    }
    NativeSetRasterizer(mNativePaint, rasterizerNative);
    mRasterizer = rasterizer;
    return NOERROR;
}

/**
 * This draws a shadow layer below the main layer, with the specified
 * offset and color, and blur radius. If radius is 0, then the shadow
 * layer is removed.
 */
ECode Paint::SetShadowLayer(
    /* [in] */ Float radius,
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Int32 color)
{
    mHasShadow = radius > 0.0f;
    mShadowRadius = radius;
    mShadowDx = dx;
    mShadowDy = dy;
    mShadowColor = color;
    NativeSetShadowLayer(radius, dx, dy, color);
    return NOERROR;
}

void Paint::NativeSetShadowLayer(
    /* [in] */ Float radius,
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Int32 color)
{
    SkPaint* paint = (SkPaint*)mNativePaint;
    if (radius <= 0) {
        paint->setLooper(NULL);
    }
    else {
        paint->setLooper(new SkBlurDrawLooper(SkFloatToScalar(radius),
                                                     SkFloatToScalar(dx),
                                                     SkFloatToScalar(dy),
                                                     (SkColor)color))->unref();
    }
}

/**
 * Temporary API to clear the shadow layer.
 */
ECode Paint::ClearShadowLayer()
{
    mHasShadow = FALSE;
    NativeSetShadowLayer(0, 0, 0, 0);
    return NOERROR;
}

/**
 * Return the paint's Align value for drawing text. This controls how the
 * text is positioned relative to its origin. LEFT align means that all of
 * the text will be drawn to the right of its origin (i.e. the origin
 * specifieds the LEFT edge of the text) and so on.
 *
 * @return the paint's Align value for drawing text.
 */
ECode Paint::GetTextAlign(
    /* [out] */ PaintAlign* align)
{
    *align = sAlignArray[NativeGetTextAlign(mNativePaint)];
    return NOERROR;
}

/**
 * Set the paint's text alignment. This controls how the
 * text is positioned relative to its origin. LEFT align means that all of
 * the text will be drawn to the right of its origin (i.e. the origin
 * specifieds the LEFT edge of the text) and so on.
 *
 * @param align set the paint's Align value for drawing text.
 */
ECode Paint::SetTextAlign(
    /* [in] */ PaintAlign align)
{
    NativeSetTextAlign(mNativePaint, align);
    return NOERROR;
}

ECode Paint::GetTextLocale(
    /* [out] */ ILocale** locale)
{
    *locale = mLocale;
    INTERFACE_ADDREF(*locale);
    return NOERROR;
}

ECode Paint::SetTextLocale(
    /* [in] */ ILocale* locale)
{
    if (locale == NULL) {
        Logger::E(TAG, "locale cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean isEqual;
    if (locale->Equals(mLocale, &isEqual), isEqual) return NOERROR;
    mLocale = locale;
    String str;
    locale->ToString(&str);
    NativeSetTextLocale(mNativePaint, str);
    return NOERROR;
}

/**
 * Return the paint's text size.
 *
 * @return the paint's text size.
 */
ECode Paint::GetTextSize(
    /* [out] */ Float* size)
{
    *size = SkScalarToFloat(((SkPaint*)mNativePaint)->getTextSize());
    return NOERROR;
}

/**
 * Set the paint's text size. This value must be > 0
 *
 * @param textSize set the paint's text size.
 */
ECode Paint::SetTextSize(
    /* [in] */ Float textSize)
{
    ((SkPaint*)mNativePaint)->setTextSize(SkFloatToScalar(textSize));
    return NOERROR;
}

/**
 * Return the paint's horizontal scale factor for text. The default value
 * is 1.0.
 *
 * @return the paint's scale factor in X for drawing/measuring text
 */
ECode Paint::GetTextScaleX(
    /* [out] */ Float* scaleX)
{
    *scaleX = SkScalarToFloat(((SkPaint*)mNativePaint)->getTextScaleX());
    return NOERROR;
}

/**
 * Set the paint's horizontal scale factor for text. The default value
 * is 1.0. Values > 1.0 will stretch the text wider. Values < 1.0 will
 * stretch the text narrower.
 *
 * @param scaleX set the paint's scale in X for drawing/measuring text.
 */
ECode Paint::SetTextScaleX(
    /* [in] */ Float scaleX)
{
     ((SkPaint*)mNativePaint)->setTextScaleX(SkFloatToScalar(scaleX));
     return NOERROR;
}

/**
 * Return the paint's horizonstal skew factor for text. The default value
 * is 0.
 *
 * @return         the paint's skew factor in X for drawing text.
 */
ECode Paint::GetTextSkewX(
    /* [out] */ Float* skewX)
{
    *skewX = SkScalarToFloat(((SkPaint*)mNativePaint)->getTextSkewX());
    return NOERROR;
}

/**
 * Set the paint's horizontal skew factor for text. The default value
 * is 0. For approximating oblique text, use values around -0.25.
 *
 * @param skewX set the paint's skew factor in X for drawing text.
 */
ECode Paint::SetTextSkewX(
    /* [in] */ Float skewX)
{
    ((SkPaint*)mNativePaint)->setTextSkewX(SkFloatToScalar(skewX));
    return NOERROR;
}

/**
 * Return the distance above (negative) the baseline (ascent) based on the
 * current typeface and text size.
 *
 * @return the distance above (negative) the baseline (ascent) based on the
 *         current typeface and text size.
 */
ECode Paint::Ascent(
    /* [out] */ Float* distance)
{
    SkPaint::FontMetrics metrics;
    ((SkPaint*)mNativePaint)->getFontMetrics(&metrics);
    *distance = SkScalarToFloat(metrics.fAscent);
    return NOERROR;
}

/**
 * Return the distance below (positive) the baseline (descent) based on the
 * current typeface and text size.
 *
 * @return the distance below (positive) the baseline (descent) based on
 *         the current typeface and text size.
 */
ECode Paint::Descent(
    /* [out] */ Float* distance)
{
    SkPaint::FontMetrics metrics;
    ((SkPaint*)mNativePaint)->getFontMetrics(&metrics);
    *distance = SkScalarToFloat(metrics.fDescent);
    return NOERROR;
}

/**
 * Return the font's recommended interline spacing, given the Paint's
 * settings for typeface, textSize, etc. If metrics is not null, return the
 * fontmetric values in it.
 *
 * @param metrics If this object is not null, its fields are filled with
 *                the appropriate values given the paint's text attributes.
 * @return the font's recommended interline spacing.
 */
ECode Paint::GetFontMetrics(
    /* [in] */ IPaintFontMetrics* metrics,
    /* [out] */ Float* spacing)
{
    SkPaint::FontMetrics metrics_;
    SkScalar spacing_ = ((SkPaint*)mNativePaint)->getFontMetrics(&metrics_);

    if (metrics) {
        metrics->SetTop(SkScalarToFloat(metrics_.fTop));
        metrics->SetAscent(SkScalarToFloat(metrics_.fAscent));
        metrics->SetDescent(SkScalarToFloat(metrics_.fDescent));
        metrics->SetBottom(SkScalarToFloat(metrics_.fBottom));
        metrics->SetLeading(SkScalarToFloat(metrics_.fLeading));
    }
    *spacing = SkScalarToFloat(spacing_);
    return NOERROR;
}

/**
 * Allocates a new FontMetrics object, and then calls getFontMetrics(fm)
 * with it, returning the object.
 */
ECode Paint::GetFontMetricsEx(
    /* [out] */ IPaintFontMetrics** metrics)
{
    CPaintFontMetrics::New(metrics);
    Float spacing;
    return GetFontMetrics(*metrics, &spacing);
}

/**
 * Return the font's interline spacing, given the Paint's settings for
 * typeface, textSize, etc. If metrics is not null, return the fontmetric
 * values in it. Note: all values have been converted to integers from
 * floats, in such a way has to make the answers useful for both spacing
 * and clipping. If you want more control over the rounding, call
 * getFontMetrics().
 *
 * @return the font's interline spacing.
 */
ECode Paint::GetFontMetricsInt(
    /* [in] */ IPaintFontMetricsInt* fmi,
    /* [out] */ Int32* spacing)
{
    SkPaint::FontMetrics metrics;

    ((SkPaint*)mNativePaint)->getFontMetrics(&metrics);
    Int32 ascent = SkScalarRound(metrics.fAscent);
    Int32 descent = SkScalarRound(metrics.fDescent);
    Int32 leading = SkScalarRound(metrics.fLeading);

    if (fmi) {
        fmi->SetTop(SkScalarFloor(metrics.fTop));
        fmi->SetAscent(ascent);
        fmi->SetDescent(descent);
        fmi->SetBottom(SkScalarCeil(metrics.fBottom));
        fmi->SetLeading(leading);
    }
    *spacing = descent - ascent + leading;
    return NOERROR;
}

ECode Paint::GetFontMetricsIntEx(
    /* [out] */ IPaintFontMetricsInt** fmi)
{
    CPaintFontMetricsInt::New(fmi);
    Int32 spacing;
    return GetFontMetricsInt(*fmi, &spacing);
}

/**
 * Return the recommend line spacing based on the current typeface and
 * text size.
 *
 * @return  recommend line spacing based on the current typeface and
 *          text size.
 */
ECode Paint::GetFontSpacing(
    /* [out] */ Float* spacing)
{
    return GetFontMetrics(NULL, spacing);
}

/**
 * Return the width of the text.
 *
 * @param text  The text to measure. Cannot be null.
 * @param index The index of the first character to start measuring
 * @param count THe number of characters to measure, beginning with start
 * @return      The width of the text
 */
ECode Paint::MeasureText(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [out] */ Float* width)
{
    if ((index | count) < 0 || index + count > text.GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text.GetLength() == 0 || count == 0) {
        *width = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *width = NativeMeasureText(text, index, count);
        return NOERROR;
    }
    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Float w = NativeMeasureText(text, index, count);
    SetTextSize(oldSize);
    *width = w * mInvCompatScaling;
    return NOERROR;
}

 /**
 * Return the width of the text.
 *
 * @param text  The text to measure. Cannot be null.
 * @param start The index of the first character to start measuring
 * @param end   1 beyond the index of the last character to measure
 * @return      The width of the text
 */
ECode Paint::MeasureTextEx(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Float* width)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text.GetLength() == 0 || start == end) {
        *width = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *width = NativeMeasureText(text, start, end);
        return NOERROR;
    }

    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Float w = NativeMeasureText(text, start, end);
    SetTextSize(oldSize);
    *width = w * mInvCompatScaling;
    return NOERROR;
}

/**
 * Return the width of the text. Cannot be null.
 *
 * @param text  The text to measure
 * @return      The width of the text
 */
ECode Paint::MeasureTextEx2(
    /* [in] */ const String& text,
    /* [out] */ Float* width)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (text.IsEmpty()) {
        *width = 0;
        return NOERROR;
    }

    if (!mHasCompatScaling) {
        *width = NativeMeasureText(text);
        return NOERROR;
    }
    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Float w = NativeMeasureText(text);
    SetTextSize(oldSize);
    *width = w * mInvCompatScaling;
    return NOERROR;
}

/**
 * Return the width of the text.
 *
 * @param text  The text to measure
 * @param start The index of the first character to start measuring
 * @param end   1 beyond the index of the last character to measure
 * @return      The width of the text
 */
ECode Paint::MeasureTextEx3(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Float* width)
{
    if (text == NULL) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 length;
    text->GetLength(&length);
    if ((start | end | (end - start) | (length - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (length == 0 || start == end) {
        *width = 0;
        return NOERROR;
    }
    // if (text instanceof String) {
    //     return measureText((String)text, start, end);
    // }
    // if (text instanceof SpannedString ||
    //     text instanceof SpannableString) {
    //     return measureText(text.toString(), start, end);
    // }
    // if (text instanceof GraphicsOperations) {
    //     return ((GraphicsOperations)text).measureText(start, end, this);
    // }

    AutoPtr< ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(end - start);
    TextUtils::GetChars(text, start, end, buf, 0);
    ECode ec = MeasureText(*buf, 0, end - start, width);
    TemporaryBuffer::Recycle(buf);
    return ec;
}

/**
 * Measure the text, stopping early if the measured width exceeds maxWidth.
 * Return the number of chars that were measured, and if measuredWidth is
 * not null, return in it the actual width measured.
 *
 * @param text  The text to measure. Cannot be null.
 * @param index The offset into text to begin measuring at
 * @param count The number of maximum number of entries to measure. If count
 *              is negative, then the characters are measured in reverse order.
 * @param maxWidth The maximum width to accumulate.
 * @param measuredWidth Optional. If not null, returns the actual width
 *                     measured.
 * @return The number of chars that were measured. Will always be <=
 *         abs(count).
 */
ECode Paint::BreakText(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    if (index < 0 || text.GetLength() - index < Elastos::Core::Math::Abs(count)) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text.GetLength() == 0 || count == 0) {
        *number = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *number = NativeBreakText(text, index, count, maxWidth, measuredWidth);
        return NOERROR;
    }

    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeBreakText(text, index, count, maxWidth * mCompatScaling,
            measuredWidth);
    SetTextSize(oldSize);
    if (measuredWidth != NULL) (*measuredWidth)[0] *= mInvCompatScaling;
    *number = res;
    return NOERROR;
}

/**
 * Measure the text, stopping early if the measured width exceeds maxWidth.
 * Return the number of chars that were measured, and if measuredWidth is
 * not null, return in it the actual width measured.
 *
 * @param text  The text to measure. Cannot be null.
 * @param start The offset into text to begin measuring at
 * @param end   The end of the text slice to measure.
 * @param measureForwards If true, measure forwards, starting at start.
 *                        Otherwise, measure backwards, starting with end.
 * @param maxWidth The maximum width to accumulate.
 * @param measuredWidth Optional. If not null, returns the actual width
 *                     measured.
 * @return The number of chars that were measured. Will always be <=
 *         abs(end - start).
 */
ECode Paint::BreakTextEx(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    if (text == NULL) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 length;
    text->GetLength(&length);
    if ((start | end | (end - start) | (length - end)) < 0) {
        return E_RUNTIME_EXCEPTION;
    }

    if (length == 0 || start == end) {
        *number = 0;
        return NOERROR;
    }
    // if (start == 0 && text instanceof String && end == text.length()) {
    //     return breakText((String) text, measureForwards, maxWidth,
    //                      measuredWidth);
    // }

    AutoPtr< ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(end - start);

    TextUtils::GetChars(text, start, end, buf, 0);

    ECode ec;
    if (measureForwards) {
        ec = BreakText(*buf, 0, end - start, maxWidth, measuredWidth, number);
    }
    else {
        ec = BreakText(*buf, 0, -(end - start), maxWidth, measuredWidth, number);
    }

    TemporaryBuffer::Recycle(buf);
    return ec;
}

/**
 * Measure the text, stopping early if the measured width exceeds maxWidth.
 * Return the number of chars that were measured, and if measuredWidth is
 * not null, return in it the actual width measured.
 *
 * @param text  The text to measure. Cannot be null.
 * @param measureForwards If true, measure forwards, starting with the
 *                        first character in the string. Otherwise,
 *                        measure backwards, starting with the
 *                        last character in the string.
 * @param maxWidth The maximum width to accumulate.
 * @param measuredWidth Optional. If not null, returns the actual width
 *                     measured.
 * @return The number of chars that were measured. Will always be <=
 *         abs(count).
 */
ECode Paint::BreakTextEx2(
    /* [in] */ const String& text,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [out] */ Int32* number)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (text.IsEmpty()) {
        *number = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *number = NativeBreakText(text, measureForwards, maxWidth, measuredWidth);
        return NOERROR;
    }

    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeBreakText(text, measureForwards, maxWidth * mCompatScaling,
            measuredWidth);
    SetTextSize(oldSize);
    if (measuredWidth != NULL) (*measuredWidth)[0] *= mInvCompatScaling;
    *number = res;
    return NOERROR;
}

/**
 * Return the advance widths for the characters in the string.
 *
 * @param text     The text to measure. Cannot be null.
 * @param index    The index of the first char to to measure
 * @param count    The number of chars starting with index to measure
 * @param widths   array to receive the advance widths of the characters.
 *                 Must be at least a large as count.
 * @return         the actual number of widths returned.
 */
ECode Paint::GetTextWidths(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* number)
{
    if ((index | count) < 0 || index + count > text.GetLength()
            || count > widths->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text.GetLength() == 0 || count == 0) {
        *number = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *number = NativeGetTextWidths(mNativePaint, text, index, count, widths);
        return NOERROR;
    }

    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeGetTextWidths(mNativePaint, text, index, count, widths);
    SetTextSize(oldSize);
    for (Int32 i = 0; i < res; i++) {
        (*widths)[i] *= mInvCompatScaling;
    }
    *number = res;
    return NOERROR;
}

/**
 * Return the advance widths for the characters in the string.
 *
 * @param text     The text to measure. Cannot be null.
 * @param start    The index of the first char to to measure
 * @param end      The end of the text slice to measure
 * @param widths   array to receive the advance widths of the characters.
 *                 Must be at least a large as (end - start).
 * @return         the actual number of widths returned.
 */
ECode Paint::GetTextWidthsEx(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* number)
{
    if (text == NULL) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 length;
    text->GetLength(&length);
    if ((start | end | (end - start) | (length - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (end - start > widths->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (length == 0 || start == end) {
        *number = 0;
        return NOERROR;
    }
    // if (text instanceof String) {
    //     return getTextWidths((String) text, start, end, widths);
    // }
    // if (text instanceof SpannedString ||
    //     text instanceof SpannableString) {
    //     return getTextWidths(text.toString(), start, end, widths);
    // }
    // if (text instanceof GraphicsOperations) {
    //     return ((GraphicsOperations) text).getTextWidths(start, end,
    //                                                          widths, this);
    // }

    AutoPtr< ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(end - start);
    TextUtils::GetChars(text, start, end, buf, 0);
    ECode ec = GetTextWidths(*buf, 0, end - start, widths, number);
    TemporaryBuffer::Recycle(buf);
    return ec;
}

/**
 * Return the advance widths for the characters in the string.
 *
 * @param text   The text to measure. Cannot be null.
 * @param start  The index of the first char to to measure
 * @param end    The end of the text slice to measure
 * @param widths array to receive the advance widths of the characters.
 *               Must be at least a large as the text.
 * @return       the number of unichars in the specified text.
 */
ECode Paint::GetTextWidthsEx2(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* number)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (end - start > widths->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text.GetLength() == 0 || start == end) {
        *number = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *number = NativeGetTextWidths(mNativePaint, text, start, end, widths);
        return NOERROR;
    }
    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Int32 res = NativeGetTextWidths(mNativePaint, text, start, end, widths);
    SetTextSize(oldSize);
    for (Int32 i = 0; i < res; i++) {
        (*widths)[i] *= mInvCompatScaling;
    }
    *number = res;
    return NOERROR;
}

/**
 * Return the advance widths for the characters in the string.
 *
 * @param text   The text to measure
 * @param widths array to receive the advance widths of the characters.
 *               Must be at least a large as the text.
 * @return       the number of unichars in the specified text.
 */
ECode Paint::GetTextWidthsEx3(
    /* [in] */ const String& text,
    /* [in] */ ArrayOf<Float>* widths,
    /* [out] */ Int32* number)
{
    return GetTextWidthsEx2(text, 0, text.GetLength(), widths, number);
}

ECode Paint::GetTextGlyphs(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [out] */ ArrayOf<Char32>* glyphs,
    /* [out] */ Int32* number)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (flags != IPaint::DIRECTION_LTR && flags != IPaint::DIRECTION_RTL) {
        Logger::E(TAG, "unknown flags value:%d\n", flags);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if ((start | end | contextStart | contextEnd | (end - start)
            | (start - contextStart) | (contextEnd - end) | (text.GetLength() - end)
            | (text.GetLength() - contextEnd)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (!glyphs && end - start > glyphs->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    *number = NativeGetTextGlyphs(mNativePaint, text, start, end, contextStart, contextEnd,
                flags, glyphs);
    return NOERROR;
}

ECode Paint::GetTextRunAdvances(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [out] */ Float* advance)
{
    return GetTextRunAdvancesEx(chars, index, count, contextIndex, contextCount, flags,
                advances, advancesIndex, 0 /* use Harfbuzz*/, advance);
}

ECode Paint::GetTextRunAdvancesEx(
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [in] */ Int32 reserved,
    /* [out] */ Float* advance)
{
    if (flags != IPaint::DIRECTION_LTR && flags != IPaint::DIRECTION_RTL) {
        Logger::E(TAG, "unknown flags value:%d\n", flags);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if ((index | count | contextIndex | contextCount | advancesIndex
            | (index - contextIndex) | (contextCount - count)
            | ((contextIndex + contextCount) - (index + count))
            | (chars.GetLength() - (contextIndex + contextCount))
            | (advances == NULL ? 0 :
                (advances->GetLength() - (advancesIndex + count)))) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (chars.GetLength() == 0 || count == 0){
        *advance = 0;
        return NOERROR;
    }
    if (!mHasCompatScaling) {
        *advance = NativeGetTextRunAdvances(mNativePaint, chars, index, count,
                contextIndex, contextCount, flags, advances, advancesIndex, reserved);
        return NOERROR;
    }

    Float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Float res = NativeGetTextRunAdvances(mNativePaint, chars, index, count,
                contextIndex, contextCount, flags, advances, advancesIndex, reserved);
    SetTextSize(oldSize);

    if (advances != NULL) {
        for (Int32 i = advancesIndex, e = i + count; i < e; i++) {
            (*advances)[i] *= mInvCompatScaling;
        }
    }
    *advance = res * mInvCompatScaling; // assume errors are not significant
    return NOERROR;
}

ECode Paint::GetTextRunAdvancesEx2(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [out] */ Float* advance)
{
     return GetTextRunAdvancesEx3(text, start, end, contextStart, contextEnd, flags,
            advances, advancesIndex, 0 /* use Harfbuzz */, advance);
}

ECode Paint::GetTextRunAdvancesEx3(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [in] */ Int32 reserved,
    /* [out] */ Float* advance)
{
    if (text == NULL) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 length;
    text->GetLength(&length);
    if ((start | end | contextStart | contextEnd | advancesIndex | (end - start)
            | (start - contextStart) | (contextEnd - end)
            | (length - contextEnd)
            | (advances == NULL ? 0 :
                (advances->GetLength() - advancesIndex - (end - start)))) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

//  if (text instanceof String) {
//        return getTextRunAdvances((String) text, start, end,
//                contextStart, contextEnd, flags, advances, advancesIndex, reserved);
//    }
//    if (text instanceof SpannedString ||
//        text instanceof SpannableString) {
//        return getTextRunAdvances(text.toString(), start, end,
//                contextStart, contextEnd, flags, advances, advancesIndex, reserved);
//  }
//    if (text instanceof GraphicsOperations) {
//        return ((GraphicsOperations) text).getTextRunAdvances(start, end,
//                contextStart, contextEnd, flags, advances, advancesIndex, this);
//    }
    if (length == 0 || end == start) {
        *advance = 0;
        return NOERROR;
    }

    Int32 contextLen = contextEnd - contextStart;
    Int32 len = end - start;
    AutoPtr< ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(contextLen);
    TextUtils::GetChars(text, contextStart, contextEnd, buf, 0);
    ECode ec = GetTextRunAdvancesEx(*buf, start - contextStart, len,
            0, contextLen, flags, advances, advancesIndex, reserved, advance);
    TemporaryBuffer::Recycle(buf);
    return ec;
}

ECode Paint::GetTextRunAdvancesEx4(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [out] */ Float* advance)
{
    return GetTextRunAdvancesEx5(text, start, end, contextStart, contextEnd, flags,
            advances, advancesIndex, 0 /* use Harfbuzz*/, advance);
}

ECode Paint::GetTextRunAdvancesEx5(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [in] */ Int32 reserved,
    /* [out] */ Float* advance)
{
    if (text.IsNull()) {
        Logger::E(TAG, "text cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (flags != IPaint::DIRECTION_LTR && flags != IPaint::DIRECTION_RTL) {
        Logger::E(TAG, "unknown flags value:%d\n", flags);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if ((start | end | contextStart | contextEnd | advancesIndex | (end - start)
            | (start - contextStart) | (contextEnd - end)
            | (text.GetLength() - contextEnd)
            | (advances == NULL ? 0 :
                (advances->GetLength() - advancesIndex - (end - start)))) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (text.GetLength() == 0 || start == end) {
        *advance = 0;
        return NOERROR;
    }

    if (!mHasCompatScaling) {
        *advance = NativeGetTextRunAdvances(mNativePaint, text, start, end,
                contextStart, contextEnd, flags, advances, advancesIndex, reserved);
        return NOERROR;
    }

    float oldSize;
    GetTextSize(&oldSize);
    SetTextSize(oldSize * mCompatScaling);
    Float totalAdvance = NativeGetTextRunAdvances(mNativePaint, text, start, end,
                contextStart, contextEnd, flags, advances, advancesIndex, reserved);
    SetTextSize(oldSize);

   if (advances != NULL) {
        for (Int32 i = advancesIndex, e = i + (end - start); i < e; i++) {
            (*advances)[i] *= mInvCompatScaling;
        }
    }

    *advance = totalAdvance * mInvCompatScaling; // assume errors are insignificant
    return NOERROR;
}

ECode Paint::GetTextRunCursor(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextLength,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
    /* [out] */ Int32* position)
{
    Int32 contextEnd = contextStart + contextLength;
    if (((contextStart | contextEnd | offset | (contextEnd - contextStart)
        | (offset - contextStart) | (contextEnd - offset)
        | (text.GetLength() - contextEnd) | cursorOpt) < 0)
        || cursorOpt > CURSOR_OPT_MAX_VALUE) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *position = NativeGetTextRunCursor(mNativePaint, text,
        contextStart, contextLength, flags, offset, cursorOpt);
    return NOERROR;
}

ECode Paint::GetTextRunCursorEx(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
    /* [out] */ Int32* position)
{
   if (IString::Probe(text) || ISpannedString::Probe(text) || ISpannableString::Probe(text)) {
        String str;
        text->ToString(&str);
       return GetTextRunCursorEx2(str, contextStart, contextEnd,
               flags, offset, cursorOpt, position);
   }
   if (IGraphicsOperations::Probe(text)) {
       return IGraphicsOperations::Probe(text)->GetTextRunCursor(
               contextStart, contextEnd, flags, offset, cursorOpt, (IPaint*)this->Probe(EIID_Paint), position);
   }

    Int32 contextLen = contextEnd - contextStart;
    AutoPtr< ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(contextLen);
    TextUtils::GetChars(text, contextStart, contextEnd, buf, 0);
    ECode ec = GetTextRunCursor(*buf, 0, contextLen, flags, offset - contextStart, cursorOpt, position);
    TemporaryBuffer::Recycle(buf);
    return ec;
}

ECode Paint::GetTextRunCursorEx2(
    /* [in] */ const String& text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt,
    /* [out] */ Int32* position)
{
    if (((contextStart | contextEnd | offset | (contextEnd - contextStart)
            | (offset - contextStart) | (contextEnd - offset)
            | (text.GetLength() - contextEnd) | cursorOpt) < 0)
            || cursorOpt > CURSOR_OPT_MAX_VALUE) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *position = NativeGetTextRunCursor(mNativePaint, text,
            contextStart, contextEnd, flags, offset, cursorOpt);
    return NOERROR;
}

/**
 * Return the path (outline) for the specified text.
 * Note: just like Canvas.drawText, this will respect the Align setting in
 * the paint.
 *
 * @param text     The text to retrieve the path from
 * @param index    The index of the first character in text
 * @param count    The number of characterss starting with index
 * @param x        The x coordinate of the text's origin
 * @param y        The y coordinate of the text's origin
 * @param path     The path to receive the data describing the text. Must
 *                 be allocated by the caller.
 */
ECode Paint::GetTextPath(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    if ((index | count) < 0 || index + count > text.GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    NativeGetTextPath(mNativePaint, mBidiFlags, text, index, count, x, y, ((CPath*)path)->Ni());
    return NOERROR;
}

/**
 * Return the path (outline) for the specified text.
 * Note: just like Canvas.drawText, this will respect the Align setting
 * in the paint.
 *
 * @param text  The text to retrieve the path from
 * @param start The first character in the text
 * @param end   1 past the last charcter in the text
 * @param x     The x coordinate of the text's origin
 * @param y     The y coordinate of the text's origin
 * @param path  The path to receive the data describing the text. Must
 *              be allocated by the caller.
 */
ECode Paint::GetTextPathEx(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPath* path)
{
    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    NativeGetTextPath(mNativePaint, mBidiFlags, text, start, end, x, y, ((CPath*)path)->Ni());
    return NOERROR;
}

/**
 * Return in bounds (allocated by the caller) the smallest rectangle that
 * encloses all of the characters, with an implied origin at (0,0).
 *
 * @param text  String to measure and return its bounds
 * @param start Index of the first char in the string to measure
 * @param end   1 past the last char in the string measure
 * @param bounds Returns the unioned bounds of all the text. Must be
 *               allocated by the caller.
 */
ECode Paint::GetTextBounds(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IRect* bounds)
{
    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (bounds == NULL) {
//        throw new NullPointerException("need bounds Rect");
        return E_NULL_POINTER_EXCEPTION;
    }
    NativeGetStringBounds(mNativePaint, text, start, end, bounds);
    return NOERROR;
}

/**
 * Return in bounds (allocated by the caller) the smallest rectangle that
 * encloses all of the characters, with an implied origin at (0,0).
 *
 * @param text  Array of chars to measure and return their unioned bounds
 * @param index Index of the first char in the array to measure
 * @param count The number of chars, beginning at index, to measure
 * @param bounds Returns the unioned bounds of all the text. Must be
 *               allocated by the caller.
 */
ECode Paint::GetTextBoundsEx(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IRect* bounds)
{
    if ((index | count) < 0 || index + count > text.GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (bounds == NULL) {
//        throw new NullPointerException("need bounds Rect");
        return E_NULL_POINTER_EXCEPTION;
    }
    NativeGetCharArrayBounds(mNativePaint, text, index, count, bounds);
    return NOERROR;
}

Int32 Paint::NativeInit()
{
    SkPaint* obj = new SkPaint();
    DefaultSettingsForElastos(obj);
    return (Int32)obj;
}

Int32 Paint::NativeInitWithPaint(
    /* [in] */ Int32 nObj)
{
    SkPaint* obj = new SkPaint(*(SkPaint*)nObj);
    return (Int32)obj;
}

void Paint::NativeReset(
    /* [in] */ Int32 nObj)
{
    ((SkPaint*)nObj)->reset();
    DefaultSettingsForElastos((SkPaint*)nObj);
}

void Paint::NativeSet(
    /* [in] */ Int32 nDst,
    /* [in] */ Int32 nSrc)
{
    *(SkPaint*)nDst = *(SkPaint*)nSrc;
}

Int32 Paint::NativeGetStyle(
    /* [in] */ Int32 nObj)
{
    return ((SkPaint*)nObj)->getStyle();
}

void Paint::NativeSetStyle(
    /* [in] */ Int32 nObj,
    /* [in] */ PaintStyle style)
{
    ((SkPaint*)nObj)->setStyle((SkPaint::Style)style);
}

Int32 Paint::NativeGetStrokeCap(
    /* [in] */ Int32 nObj)
{
    return ((SkPaint*)nObj)->getStrokeCap();
}

void Paint::NativeSetStrokeCap(
    /* [in] */ Int32 nObj,
    /* [in] */ PaintCap cap)
{
    ((SkPaint*)nObj)->setStrokeCap((SkPaint::Cap)cap);
}

Int32 Paint::NativeGetStrokeJoin(
    /* [in] */ Int32 nObj)
{
    return ((SkPaint*)nObj)->getStrokeJoin();
}

void Paint::NativeSetStrokeJoin(
    /* [in] */ Int32 nObj,
    /* [in] */ PaintJoin join)
{
    ((SkPaint*)nObj)->setStrokeJoin((SkPaint::Join)join);
}

Boolean Paint::NativeGetFillPath(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 src,
    /* [in] */ Int32 dst)
{
    return ((SkPaint*)nObj)->getFillPath(*(SkPath*)src, (SkPath*)dst);
}

Int32 Paint::NativeSetShader(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 shader)
{
    return (Int32)((SkPaint*)nObj)->setShader((SkShader*)shader);
}

Int32 Paint::NativeSetColorFilter(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 filter)
{
    return (Int32)((SkPaint*)nObj)->setColorFilter((SkColorFilter*)filter);
}

Int32 Paint::NativeSetXfermode(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 xfermode)
{
    return (Int32)((SkPaint*)nObj)->setXfermode((SkXfermode*)xfermode);
}

Int32 Paint::NativeSetPathEffect(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 effect)
{
    return (Int32)((SkPaint*)nObj)->setPathEffect((SkPathEffect*)effect);
}

Int32 Paint::NativeSetMaskFilter(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 maskfilter)
{
    return (Int32)((SkPaint*)nObj)->setMaskFilter((SkMaskFilter*)maskfilter);
}

Int32 Paint::NativeSetTypeface(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 typeface)
{
    return (Int32)((SkPaint*)nObj)->setTypeface((SkTypeface*)typeface);
}

Int32 Paint::NativeSetRasterizer(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 rasterizer)
{
    return (Int32)((SkPaint*)nObj)->setRasterizer((SkRasterizer*)rasterizer);
}

Int32 Paint::NativeGetTextAlign(
    /* [in] */ Int32 nObj)
{
    return ((SkPaint*)nObj)->getTextAlign();
}

void Paint::NativeSetTextAlign(
    /* [in] */ Int32 nObj,
    /* [in] */ PaintAlign align)
{
    ((SkPaint*)nObj)->setTextAlign((SkPaint::Align)align);
}

// generate bcp47 identifier for the supplied locale
static void ToLanguageTag(
    /* [in] */ char* output,
    /* [in] */ size_t outSize,
    /* [in] */ const char* locale)
{
    if (output == NULL || outSize <= 0) {
        return;
    }
    if (locale == NULL) {
        output[0] = '\0';
        return;
    }
    char canonicalChars[ULOC_FULLNAME_CAPACITY];
    UErrorCode uErr = U_ZERO_ERROR;
    uloc_canonicalize(locale, canonicalChars, ULOC_FULLNAME_CAPACITY,
            &uErr);
    if (U_SUCCESS(uErr)) {
        char likelyChars[ULOC_FULLNAME_CAPACITY];
        uErr = U_ZERO_ERROR;
        uloc_addLikelySubtags(canonicalChars, likelyChars,
                ULOC_FULLNAME_CAPACITY, &uErr);
        if (U_SUCCESS(uErr)) {
            uErr = U_ZERO_ERROR;
            uloc_toLanguageTag(likelyChars, output, outSize, FALSE, &uErr);
            if (U_SUCCESS(uErr)) {
                return;
            }
            else {
                // ALOGD("uloc_toLanguageTag(\"%s\") failed: %s", likelyChars,
                //         u_errorName(uErr));
            }
        }
        else {
            // ALOGD("uloc_addLikelySubtags(\"%s\") failed: %s",
            //         canonicalChars, u_errorName(uErr));
        }
    }
    else {
        // ALOGD("uloc_canonicalize(\"%s\") failed: %s", locale,
        //         u_errorName(uErr));
    }
    // unable to build a proper language identifier
    output[0] = '\0';
}

//anthony native, can't implement
void Paint::NativeSetTextLocale(
    /* [in] */ Int32 nObj,
    /* [in] */ const String& locale)
{
    char langTag[ULOC_FULLNAME_CAPACITY];
    ToLanguageTag(langTag, ULOC_FULLNAME_CAPACITY, locale.string());
    ((SkPaint*)nObj)->setLanguage(SkLanguage(langTag));
}

Float Paint::NativeMeasureText(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count)
{
    if ((index | count) < 0 || (size_t)(index + count) > text.GetLength()) {
//        doThrow(env, "java/lang/ArrayIndexOutOfBoundsException");
        return 0;
    }
    if (count == 0) {
        return 0;
    }

    SkPaint* paint = (SkPaint*)mNativePaint;
    Float result = 0;

    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength());
    for (Int32 i = 0; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }
    android::TextLayout::getTextRunAdvances(paint, char16Array->GetPayload(), index, count,
            char16Array->GetLength(), paint->getFlags(), NULL /* dont need all advances */, &result);

    return result;
}

Float Paint::NativeMeasureText(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    size_t textLength = text.GetLength();
    int count = end - start;
    if ((start | count) < 0 || (size_t)end > textLength) {
        // doThrowAIOOBE(env);
        return 0;
    }
    if (count == 0) {
        return 0;
    }

    AutoPtr< ArrayOf<Char32> > textArray = text.GetChars();
    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(textArray->GetLength());
    for (Int32 i = 0; i < textArray->GetLength(); ++i) {
        (*char16Array)[i] = (Char16)(*textArray)[i];
    }
    SkPaint* paint = (SkPaint*)mNativePaint;
    Float width = 0;

    android::TextLayout::getTextRunAdvances(paint, char16Array->GetPayload(), start, count, textLength,
            paint->getFlags(), NULL /* dont need all advances */, &width);

    return width;
}

Float Paint::NativeMeasureText(
    /* [in] */ const String& text)
{
    size_t textLength = text.GetLength();
    if (textLength == 0) {
        return 0;
    }

    AutoPtr< ArrayOf<Char32> > textArray = text.GetChars();
    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(textArray->GetLength());
    for (Int32 i = 0; i < textArray->GetLength(); ++i) {
        (*char16Array)[i] = (Char16)(*textArray)[i];
    }
    SkPaint* paint = (SkPaint*)mNativePaint;
    Float width = 0;

    android::TextLayout::getTextRunAdvances(paint, char16Array->GetPayload(), 0, textLength, textLength,
            paint->getFlags(), NULL /* dont need all advances */, &width);

    return width;
}

static Int32 DoBreakText(
    /* [in] */ SkPaint& paint,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth,
    /* [in] */ SkPaint::TextBufferDirection tbd)
{
    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength() - index);
    for (Int32 i = index; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }
    android::sp<android::TextLayoutValue> value =
            android::TextLayoutEngine::getInstance().getValue(&paint,
                char16Array->GetPayload(), 0, count, count, paint.getFlags());
    if (value == NULL) {
        return 0;
    }
    SkScalar     measured;
    size_t       bytes = paint.breakText(value->getGlyphs(), value->getGlyphsCount() << 1,
                               SkFloatToScalar(maxWidth), &measured, tbd);
    SkASSERT((bytes & 1) == 0);

    if (measuredWidth && measuredWidth->GetLength() > 0) {
        (*measuredWidth)[0] = SkScalarToFloat(measured);
    }
    return bytes >> 1;
}

Int32 Paint::NativeBreakText(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth)
{
    SkPaint::TextBufferDirection tbd;
    if (count < 0) {
        tbd = SkPaint::kBackward_TextBufferDirection;
        count = -count;
    }
    else {
        tbd = SkPaint::kForward_TextBufferDirection;
    }

    if ((index < 0) || (index + count > text.GetLength())) {
//        doThrow(env, "java/lang/ArrayIndexOutOfBoundsException");
        return 0;
    }

    SkPaint* paint = (SkPaint*)mNativePaint;
    count = DoBreakText(*paint, text, index, count, maxWidth,
            measuredWidth, tbd);
    return count;
}

Int32 Paint::NativeBreakText(
    /* [in] */ const String& text,
    /* [in] */ Boolean measureForwards,
    /* [in] */ Float maxWidth,
    /* [in] */ ArrayOf<Float>* measuredWidth)
{
    SkPaint::TextBufferDirection tbd = measureForwards ?
            SkPaint::kForward_TextBufferDirection :
            SkPaint::kBackward_TextBufferDirection;

    SkPaint* paint = (SkPaint*)mNativePaint;
    AutoPtr< ArrayOf<Char32> > textArray = text.GetChars();
    Int32 count = DoBreakText(*paint, *textArray, 0, textArray->GetLength(), maxWidth,
            measuredWidth, tbd);
    return count;
}

static Int32 DoTextWidths(
    /* [in] */ SkPaint* paint,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* widths)
{
    if (count < 0 || !widths) {
        // doThrowAIOOBE(env);
        return 0;
    }
    if (count == 0) {
        return 0;
    }
    if (count > widths->GetLength()) {
        // doThrowAIOOBE(env);
        return 0;
    }

    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength() - index);
    for (Int32 i = index; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }
    android::TextLayout::getTextRunAdvances(paint, char16Array->GetPayload(), 0, count, count,
            paint->getFlags(), widths->GetPayload(), NULL /* dont need totalAdvance */);

    return count;
}

Int32 Paint::NativeGetTextWidths(
    /* [in] */ Int32 nObj,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Float>* widths)
{
    count = DoTextWidths((SkPaint*)nObj, text, index, count, widths);
    return count;
}

Int32 Paint::NativeGetTextWidths(
    /* [in] */ Int32 nObj,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Float>* widths)
{
    AutoPtr< ArrayOf<Char32> > textArray = text.GetChars();
    Int32 count = DoTextWidths((SkPaint*)nObj, *textArray, start, end - start, widths);
    return count;
}

static Int32 DoTextGlyphs(
    /* [in] */ SkPaint* paint,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextCount,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Char32>* glyphs)
{
    if ((start | count | contextCount) < 0 || contextCount < count || !glyphs) {
        // doThrowAIOOBE(env);
        return 0;
    }
    if (count == 0) {
        return 0;
    }
    if (count > glyphs->GetLength()) {
        // doThrowAIOOBE(env);
        return 0;
    }

    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength());
    for (Int32 i = 0; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }

    android::sp<android::TextLayoutValue> value = android::TextLayoutEngine::getInstance().getValue(
            paint, char16Array->GetPayload(), start, count, contextCount, flags);
    const Char16* shapedGlyphs = value->getGlyphs();
    size_t glyphsCount = value->getGlyphsCount();
    for (Int32 i = 0; i < glyphsCount; ++i) {
        (*glyphs)[i] = shapedGlyphs[i];
    }

    return glyphsCount;
}

Int32 Paint::NativeGetTextGlyphs(
    /* [in] */ Int32 nObj,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [out] */ ArrayOf<Char32>* glyphs)
{
    AutoPtr< ArrayOf<Char32> > textArray = text.GetChars();
    Int32 count = DoTextGlyphs((SkPaint*)nObj, *textArray, start,
            end - start, contextEnd - contextStart, flags, glyphs);
    return count;
}

static Float DoTextRunAdvances(
    /* [in] */ SkPaint *paint,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextCount,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex)
{
    if ((start | count | contextCount | advancesIndex) < 0 || contextCount < count) {
        // doThrowAIOOBE(env);
        return 0;
    }
    if (count == 0) {
        return 0;
    }
    if (advances) {
        if (count > advances->GetLength()) {
            // doThrowAIOOBE(env);
            return 0;
        }
    }
    Float advancesArray[count];
    Float totalAdvance = 0;

    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength());
    for (Int32 i = 0; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }

    android::TextLayout::getTextRunAdvances(paint, char16Array->GetPayload(), start, count,
            contextCount, flags, advancesArray, &totalAdvance);

    if (advances != NULL) {
        memcpy(advances->GetPayload() + advancesIndex, advancesArray, count * sizeof(Float));
    }
    return totalAdvance;
}

static Float DoTextRunAdvancesICU(
    /* [in] */ SkPaint *paint,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextCount,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex)
{
    if ((start | count | contextCount | advancesIndex) < 0 || contextCount < count) {
        // doThrowAIOOBE(env);
        return 0;
    }
    if (count == 0) {
        return 0;
    }
    if (advances) {
        if (count > advances->GetLength()) {
            // doThrowAIOOBE(env);
            return 0;
        }
    }

    Float advancesArray[count];
    Float totalAdvance = 0;

    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength());
    for (Int32 i = 0; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }

    android::TextLayout::getTextRunAdvancesICU(paint, char16Array->GetPayload(),
            start, count, contextCount, flags, advancesArray, totalAdvance);

    if (advances != NULL) {
        memcpy(advances->GetPayload() + advancesIndex, advancesArray, count * sizeof(Float));
    }
    return totalAdvance;
}

Float Paint::NativeGetTextRunAdvances(
    /* [in] */ Int32 paint,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [in] */ Int32 reserved)
{
    Float result = (reserved == 0) ?
            DoTextRunAdvances((SkPaint*)paint, text, index,
                    count, contextCount, flags, advances, advancesIndex) :
            DoTextRunAdvancesICU((SkPaint*)paint, text, index,
                    count, contextCount, flags, advances, advancesIndex);
    return result;
}

Float Paint::NativeGetTextRunAdvances(
    /* [in] */ Int32 paint,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ ArrayOf<Float>* advances,
    /* [in] */ Int32 advancesIndex,
    /* [in] */ Int32 reserved)
{
    AutoPtr< ArrayOf<Char32> > textArray = text.GetChars();
    Float result = (reserved == 0) ?
            DoTextRunAdvances((SkPaint*)paint, *textArray, start,
                    end - start, contextEnd - contextStart, flags, advances, advancesIndex) :
            DoTextRunAdvancesICU((SkPaint*)paint, *textArray, start,
                    end - start, contextEnd - contextStart, flags, advances, advancesIndex);
    return result;
}

enum MoveOpt {
    AFTER, AT_OR_AFTER, BEFORE, AT_OR_BEFORE, AT
};

static Int32 DoTextRunCursor(
    /* [in] */ SkPaint* paint,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 opt)
{
    Float scalarArray[count];

    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength());
    for (Int32 i = 0; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }
    android::TextLayout::getTextRunAdvances(paint, char16Array->GetPayload(), start,
            count, start + count, flags, scalarArray, NULL /* dont need totalAdvance */);

    Int32 pos = offset - start;
    switch (opt) {
    case AFTER:
      if (pos < count) {
        pos += 1;
      }
      // fall through
    case AT_OR_AFTER:
      while (pos < count && scalarArray[pos] == 0) {
        ++pos;
      }
      break;
    case BEFORE:
      if (pos > 0) {
        --pos;
      }
      // fall through
    case AT_OR_BEFORE:
      while (pos > 0 && scalarArray[pos] == 0) {
        --pos;
      }
      break;
    case AT:
    default:
      if (scalarArray[pos] == 0) {
        pos = -1;
      }
      break;
    }

    if (pos != -1) {
      pos += start;
    }

    return pos;
}

Int32 Paint::NativeGetTextRunCursor(
    /* [in] */ Int32 paint,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextCount,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt)
{
    Int32 result = DoTextRunCursor((SkPaint*)paint, text, contextStart, contextCount, flags,
            offset, cursorOpt);
    return result;
}

Int32 Paint::NativeGetTextRunCursor(
    /* [in] */ Int32 paint,
    /* [in] */ const String& text,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 cursorOpt)
{
    AutoPtr< ArrayOf<Char32> > textArray = text.GetChars();
    Int32 result = DoTextRunCursor((SkPaint*)paint, *textArray, contextStart,
            contextEnd - contextStart, flags, offset, cursorOpt);
    return result;
}

static void DoGetTextPath(
    /* [in] */ SkPaint* paint,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ SkPath *path)
{
    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength() - index);
    for (Int32 i = index; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }
    android::TextLayout::getTextPath(paint, char16Array->GetPayload(), count, bidiFlags, x, y, path);
}

void Paint::NativeGetTextPath(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 path)
{
    DoGetTextPath((SkPaint*)nObj, text, index, count, bidiFlags, x, y, (SkPath*)path);
}

void Paint::NativeGetTextPath(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 path)
{
    AutoPtr< ArrayOf<Char32> > textArray = text.GetChars();
    DoGetTextPath((SkPaint*)nObj, *textArray, start, end - start, bidiFlags, x, y, (SkPath*)path);
}

static void DoTextBounds(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IRect* bounds,
    /* [in] */ const SkPaint& paint)
{
    SkRect r = SkRect::MakeXYWH(0,0,0,0);
    SkIRect ir;

    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength());
    for (Int32 i = index; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }
    android::sp<android::TextLayoutValue> value = android::TextLayoutEngine::getInstance().getValue(&paint,
            char16Array->GetPayload(), 0, count, count, paint.getFlags());
    if (value == NULL) {
        return;
    }
    paint.measureText(value->getGlyphs(), value->getGlyphsCount() << 1, &r);
    r.roundOut(&ir);
    GraphicsNative::SkIRect2IRect(ir, bounds);
}

void Paint::NativeGetStringBounds(
    /* [in] */ Int32 nObj,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IRect* bounds)
{
    AutoPtr< ArrayOf<Char32> > textArray = text.GetChars();
    DoTextBounds(*textArray, start, end - start, bounds, *(SkPaint*)nObj);
}

void Paint::NativeGetCharArrayBounds(
    /* [in] */ Int32 nObj,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IRect* bounds)
{
    DoTextBounds(text, index, count, bounds, *(SkPaint*)nObj);
}

void Paint::NativeFinalizer(
    /* [in] */ Int32 nObj)
{
    delete (SkPaint*)nObj;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
