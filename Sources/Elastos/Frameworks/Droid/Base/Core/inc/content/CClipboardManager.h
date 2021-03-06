
#ifndef __CCLIPBOARDMANAGER_H__
#define __CCLIPBOARDMANAGER_H__

#include "_CClipboardManager.h"
#include <ext/frameworkext.h>
#include "os/HandlerBase.h"
#include <elastos/Mutex.h>
#include <elastos/List.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility;
using namespace Elastos::Droid::Os;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CClipboardManager)
{
private:

    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ CClipboardManager* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        CClipboardManager* mHost;
    };

    class PrimaryClipChangedServiceListener
        : public ElRefBase
        , public IOnPrimaryClipChangedListener
    {
    public:
        PrimaryClipChangedServiceListener(
            /* [in] */ CClipboardManager* context);

        ~PrimaryClipChangedServiceListener();

        CAR_INTERFACE_DECL()

        CARAPI DispatchPrimaryClipChanged();

    private:
        AutoPtr<CClipboardManager> mHost;
    };

public:
    CClipboardManager();

    ~CClipboardManager();

    /**
     * Sets the current primary clip on the clipboard.  This is the clip that
     * is involved in normal cut and paste operations.
     *
     * @param clip The clipped data item to set.
     */
    CARAPI SetPrimaryClip(
        /* [in] */ IClipData* clip);

    /**
     * Returns the current primary clip on the clipboard.
     */
    CARAPI GetPrimaryClip(
        /* [out] */ IClipData** clipData);

    /**
     * Returns a description of the current primary clip on the clipboard
     * but not a copy of its data.
     */
    CARAPI GetPrimaryClipDescription(
        /* [out] */ IClipDescription** clipDescription);

    /**
     * Returns true if there is currently a primary clip on the clipboard.
     */
    CARAPI HasPrimaryClip(
        /* [out] */ Boolean* hasClip);

    CARAPI AddPrimaryClipChangedListener(
        /* [in] */ IClipboardManagerOnPrimaryClipChangedListener* what);

    CARAPI RemovePrimaryClipChangedListener(
        /* [in] */ IClipboardManagerOnPrimaryClipChangedListener* what);

    /**
     * Returns the text on the clipboard.  It will eventually be possible
     * to store types other than text too, in which case this will return
     * null if the type cannot be coerced to text.
     */
    CARAPI GetText(
        /* [out] */ ICharSequence** text);

    /**
     * Sets the contents of the clipboard to the specified text.
     */
    CARAPI SetText(
        /* [in] */ ICharSequence* text);

    /**
     * Returns true if the clipboard contains text; false otherwise.
     */
    CARAPI HasText(
        /* [out] */ Boolean* hasText);

    /** {@hide} */
    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    friend class PrimaryClipChangedServiceListener;

    static CARAPI GetService(
        /* [out] */ IClipboard** clipboard);

    CARAPI ReportPrimaryClipChanged();

private:
    static AutoPtr<IClipboard> sService;
    static Mutex sStaticLock;
    static const Int32 MSG_REPORT_PRIMARY_CLIP_CHANGED;

private:
    AutoPtr<IContext> mContext;
    List<AutoPtr<IClipboardManagerOnPrimaryClipChangedListener> > mPrimaryClipChangedListeners;
    AutoPtr<IOnPrimaryClipChangedListener> mPrimaryClipChangedServiceListener;
    Mutex mPrimaryClipChangedListenersLock;
    AutoPtr<IHandler> mHandler;
};

}
}
}

#endif // __CCLIPBOARDMANAGER_H__
