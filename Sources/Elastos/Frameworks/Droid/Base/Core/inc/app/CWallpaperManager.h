#ifndef __CWALLPAPERMANAGER_H__
#define __CWALLPAPERMANAGER_H__

#include "_CWallpaperManager.h"
#include "ext/frameworkdef.h"
#include "app/CGlobalsWallpaperManagerCallback.h"
#include "graphics/drawable/Drawable.h"
#include "graphics/drawable/DrawableMacro.h"
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;
using Elastos::IO::IInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::IColorFilter;
using Elastos::Droid::Graphics::PorterDuffMode;
using Elastos::Droid::Graphics::IRegion;
using Elastos::Droid::Graphics::IInsets;
using Elastos::Droid::Graphics::Drawable::IDrawableConstantState;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::Drawable;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::ILooper;

namespace Elastos{
namespace Droid{
namespace App{

CarClass(CWallpaperManager)
{
public:
    CWallpaperManager();

    static CARAPI_(void) InitGlobals(
        /* [in] */ ILooper* looper);

    /**
     * Retrieve a WallpaperManager associated with the given Context.
     */
    static CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IWallpaperManager** manager);

    /** @hide */
    CARAPI GetIWallpaperManager(
        /* [out] */ IIWallpaperManager** manager);

    /**
     * Retrieve the current system wallpaper; if
     * no wallpaper is set, the system default wallpaper is returned.
     * This is returned as an
     * abstract Drawable that you can install in a View to display whatever
     * wallpaper the user has currently set.
     *
     * @return Returns a Drawable object that will draw the wallpaper.
     */
    CARAPI GetDrawable(
        /* [out] */ IDrawable** drawable);

    /**
     * Retrieve the current system wallpaper; if there is no wallpaper set,
     * a null pointer is returned. This is returned as an
     * abstract Drawable that you can install in a View to display whatever
     * wallpaper the user has currently set.
     *
     * @return Returns a Drawable object that will draw the wallpaper or a
     * null pointer if these is none.
     */
    CARAPI PeekDrawable(
        /* [out] */ IDrawable** drawable);

    /**
     * Like {@link #getDrawable()}, but the returned Drawable has a number
     * of limitations to reduce its overhead as much as possible. It will
     * never scale the wallpaper (only centering it if the requested bounds
     * do match the bitmap bounds, which should not be typical), doesn't
     * allow setting an alpha, color filter, or other attributes, etc.  The
     * bounds of the returned drawable will be initialized to the same bounds
     * as the wallpaper, so normally you will not need to touch it.  The
     * drawable also assumes that it will be used in a context running in
     * the same density as the screen (not in density compatibility mode).
     *
     * @return Returns a Drawable object that will draw the wallpaper.
     */
    CARAPI GetFastDrawable(
        /* [out] */ IDrawable** drawable);

    /**
     * Like {@link #getFastDrawable()}, but if there is no wallpaper set,
     * a null pointer is returned.
     *
     * @return Returns an optimized Drawable object that will draw the
     * wallpaper or a null pointer if these is none.
     */
    CARAPI PeekFastDrawable(
        /* [out] */ IDrawable** drawable);

    /**
     * Like {@link #getDrawable()} but returns a Bitmap.
     *
     * @hide
     */
    CARAPI GetBitmap(
        /* [out] */ IBitmap** bitmap);

    /**
     * Remove all internal references to the last loaded wallpaper.  Useful
     * for apps that want to reduce memory usage when they only temporarily
     * need to have the wallpaper.  After calling, the next request for the
     * wallpaper will require reloading it again from disk.
     */
    CARAPI ForgetLoadedWallpaper();

    /**
     * If the current wallpaper is a live wallpaper component, return the
     * information about that wallpaper.  Otherwise, if it is a static image,
     * simply return null.
     */
    CARAPI GetWallpaperInfo(
        /* [out] */ IWallpaperInfo** info);

    /**
     * Change the current system wallpaper to the bitmap in the given resource.
     * The resource is opened as a raw data stream and copied into the
     * wallpaper; it must be a valid PNG or JPEG image.  On success, the intent
     * {@link Intent#ACTION_WALLPAPER_CHANGED} is broadcast.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#SET_WALLPAPER}.
     *
     * @param resid The bitmap to save.
     *
     * @throws IOException If an error occurs reverting to the default
     * wallpaper.
     */
    CARAPI SetResource(
        /* [in] */ Int32 resid);

    /**
     * Change the current system wallpaper to a bitmap.  The given bitmap is
     * converted to a PNG and stored as the wallpaper.  On success, the intent
     * {@link Intent#ACTION_WALLPAPER_CHANGED} is broadcast.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#SET_WALLPAPER}.
     *
     * @param bitmap The bitmap to save.
     *
     * @throws IOException If an error occurs reverting to the default
     * wallpaper.
     */
    CARAPI SetBitmap(
        /* [in] */ IBitmap* bitmap);

    /**
     * Change the current system wallpaper to a specific byte stream.  The
     * give InputStream is copied into persistent storage and will now be
     * used as the wallpaper.  Currently it must be either a JPEG or PNG
     * image.  On success, the intent {@link Intent#ACTION_WALLPAPER_CHANGED}
     * is broadcast.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#SET_WALLPAPER}.
     *
     * @param data A stream containing the raw data to install as a wallpaper.
     *
     * @throws IOException If an error occurs reverting to the default
     * wallpaper.
     */
    CARAPI SetStream(
        /* [in] */ IInputStream* data);

    /**
     * Return whether any users are currently set to use the wallpaper
     * with the given resource ID.  That is, their wallpaper has been
     * set through {@link #setResource(int)} with the same resource id.
     */
    CARAPI HasResourceWallpaper(
        /* [in] */ Int32 resid,
        /* [out] */ Boolean* hasResource);

    /**
     * Returns the desired minimum width for the wallpaper. Callers of
     * {@link #setBitmap(android.graphics.Bitmap)} or
     * {@link #setStream(java.io.InputStream)} should check this value
     * beforehand to make sure the supplied wallpaper respects the desired
     * minimum width.
     *
     * If the returned value is <= 0, the caller should use the width of
     * the default display instead.
     *
     * @return The desired minimum width for the wallpaper. This value should
     * be honored by applications that set the wallpaper but it is not
     * mandatory.
     */
    CARAPI GetDesiredMinimumWidth(
        /* [out] */ Int32* width);

    /**
     * Returns the desired minimum height for the wallpaper. Callers of
     * {@link #setBitmap(android.graphics.Bitmap)} or
     * {@link #setStream(java.io.InputStream)} should check this value
     * beforehand to make sure the supplied wallpaper respects the desired
     * minimum height.
     *
     * If the returned value is <= 0, the caller should use the height of
     * the default display instead.
     *
     * @return The desired minimum height for the wallpaper. This value should
     * be honored by applications that set the wallpaper but it is not
     * mandatory.
     */
    CARAPI GetDesiredMinimumHeight(
        /* [out] */ Int32* height);

    /**
     * For use only by the current home application, to specify the size of
     * wallpaper it would like to use.  This allows such applications to have
     * a virtual wallpaper that is larger than the physical screen, matching
     * the size of their workspace.
     *
     * <p>Note developers, who don't seem to be reading this.  This is
     * for <em>home screens</em> to tell what size wallpaper they would like.
     * Nobody else should be calling this!  Certainly not other non-home-screen
     * apps that change the wallpaper.  Those apps are supposed to
     * <b>retrieve</b> the suggested size so they can construct a wallpaper
     * that matches it.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#SET_WALLPAPER_HINTS}.
     *
     * @param minimumWidth Desired minimum width
     * @param minimumHeight Desired minimum height
     */
    CARAPI SuggestDesiredDimensions(
        /* [in] */ Int32 minimumWidth,
        /* [in] */ Int32 minimumHeight);

    /**
     * Set the position of the current wallpaper within any larger space, when
     * that wallpaper is visible behind the given window.  The X and Y offsets
     * are floating point numbers ranging from 0 to 1, representing where the
     * wallpaper should be positioned within the screen space.  These only
     * make sense when the wallpaper is larger than the screen.
     *
     * @param windowToken The window who these offsets should be associated
     * with, as returned by {@link android.view.View#getWindowToken()
     * View.getWindowToken()}.
     * @param xOffset The offset along the X dimension, from 0 to 1.
     * @param yOffset The offset along the Y dimension, from 0 to 1.
     */
    CARAPI SetWallpaperOffsets(
        /* [in] */ IBinder* windowToken,
        /* [in] */ Float xOffset,
        /* [in] */ Float yOffset);

    /**
     * For applications that use multiple virtual screens showing a wallpaper,
     * specify the step size between virtual screens. For example, if the
     * launcher has 3 virtual screens, it would specify an xStep of 0.5,
     * since the X offset for those screens are 0.0, 0.5 and 1.0
     * @param xStep The X offset delta from one screen to the next one
     * @param yStep The Y offset delta from one screen to the next one
     */
    CARAPI SetWallpaperOffsetSteps(
        /* [in] */ Float xStep,
        /* [in] */ Float yStep);

    /**
     * Send an arbitrary command to the current active wallpaper.
     *
     * @param windowToken The window who these offsets should be associated
     * with, as returned by {@link android.view.View#getWindowToken()
     * View.getWindowToken()}.
     * @param action Name of the command to perform.  This must be a scoped
     * name to avoid collisions, such as "com.mycompany.wallpaper.DOIT".
     * @param x Arbitrary integer argument based on command.
     * @param y Arbitrary integer argument based on command.
     * @param z Arbitrary integer argument based on command.
     * @param extras Optional additional information for the command, or null.
     */
    CARAPI SendWallpaperCommand(
        /* [in] */ IBinder* windowToken,
        /* [in] */ const String& action,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z,
        /* [in] */ IBundle* extras);

    /**
     * Clear the offsets previously associated with this window through
     * {@link #setWallpaperOffsets(IBinder, float, float)}.  This reverts
     * the window to its default state, where it does not cause the wallpaper
     * to scroll from whatever its last offsets were.
     *
     * @param windowToken The window who these offsets should be associated
     * with, as returned by {@link android.view.View#getWindowToken()
     * View.getWindowToken()}.
     */
    CARAPI ClearWallpaperOffsets(
        /* [in] */ IBinder* windowToken);

    /**
     * Remove any currently set wallpaper, reverting to the system's default
     * wallpaper. On success, the intent {@link Intent#ACTION_WALLPAPER_CHANGED}
     * is broadcast.
     *
     * <p>This method requires the caller to hold the permission
     * {@link android.Manifest.permission#SET_WALLPAPER}.
     *
     * @throws IOException If an error occurs reverting to the default
     * wallpaper.
     */
    CARAPI Clear();

    static CARAPI_(AutoPtr<IBitmap>) GenerateBitmap(
        /* [in] */ IContext* context,
        /* [in] */ IBitmap* bm,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler);

private:
    CARAPI_(void) SetWallpaper(
        /* [in] */ IInputStream* data,
        /* [in] */ IFileOutputStream* fos);

private:
    static const String TAG;
    static Boolean DEBUG;
    Float mWallpaperXStep;
    Float mWallpaperYStep;
    AutoPtr<IContext> mContext;
    static Mutex sSync;
    static AutoPtr<CGlobalsWallpaperManagerCallback> sGlobals;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __CWALLPAPERMANAGER_H__