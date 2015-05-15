

#ifdef DROID_CORE
#include "inputmethodservice/InputMethodService.h"
#include "view/ViewTreeObserver.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/inputmethod/CEditorInfo.h"
#include "view/inputmethod/CExtractedTextRequest.h"
#include "view/inputmethod/CExtractedText.h"
#include "view/CKeyEvent.h"
#include "view/animation/AnimationUtils.h"
#include "widget/CLinearLayoutLayoutParams.h"
#include "content/res/CConfiguration.h"
#include "content/res/CResources.h"
#include "content/res/CResourcesHelper.h"
#include "graphics/CRegion.h"
#include "app/CActivityManagerHelper.h"
// #include "provider/CSettingsGlobal.h"
using Elastos::Droid::View::Animation::AnimationUtils;

#else
#include "inputmethodservice/InputMethodService.h"

#define VALIDATE_NOT_NULL(x) if (!(x)) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }
using Elastos::Droid::View::Animation::CAnimationUtils;
#endif

#include <elastos/Character.h>
#include <elastos/Slogger.h>
#include <elastos/Logger.h>
#include "R.h"
#include "os/SystemClock.h"


using namespace Elastos::Core;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::R;
using Elastos::Droid::App::CActivityManagerHelper;
using Elastos::Droid::App::IActivityManagerHelper;
using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Graphics::CRegion;
using Elastos::Droid::Os::SystemClock;
//using Elastos::Droid::Provider::ISettingsGlobal;
// using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Text::IInputType;
using Elastos::Droid::Text::ISpannable;
using Elastos::Droid::Text::ILayout;
using Elastos::Droid::Text::Method::IMovementMethod;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::CKeyEvent;
using Elastos::Droid::View::IKeyCharacterMap;
using Elastos::Droid::View::IViewTreeObserver;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IOnComputeInternalInsetsListener;
using Elastos::Droid::View::Animation::IAnimation;
using Elastos::Droid::View::Animation::IAnimationUtils;

using Elastos::Droid::View::InputMethod::EIID_IInputMethod;
using Elastos::Droid::View::InputMethod::IInputMethodSession;
using Elastos::Droid::View::InputMethod::EIID_IInputMethodSession;
using Elastos::Droid::View::InputMethod::CExtractedTextRequest;
using Elastos::Droid::View::InputMethod::IExtractedTextRequest;
using Elastos::Droid::Widget::ILinearLayoutLayoutParams;


namespace Elastos {
namespace Droid {
namespace InputMethodService {

const String InputMethodService::TAG("InputMethodService");
const Boolean InputMethodService::DEBUG = FALSE;

const Int32 InputMethodService::MOVEMENT_DOWN;
const Int32 InputMethodService::MOVEMENT_UP;
const Int32 InputMethodService::Insets::TOUCHABLE_INSETS_FRAME
                = IInternalInsetsInfo::TOUCHABLE_INSETS_FRAME;
const Int32 InputMethodService::Insets::TOUCHABLE_INSETS_CONTENT
                = IInternalInsetsInfo::TOUCHABLE_INSETS_CONTENT;
const Int32 InputMethodService::Insets::TOUCHABLE_INSETS_VISIBLE
                = IInternalInsetsInfo::TOUCHABLE_INSETS_VISIBLE;
const Int32 InputMethodService::Insets::TOUCHABLE_INSETS_REGION
                = IInternalInsetsInfo::TOUCHABLE_INSETS_REGION;

InputMethodService::_OnComputeInternalInsetsListener::_OnComputeInternalInsetsListener(
    /* [in] */ InputMethodService* host)
    : mHost(host)
{}

InputMethodService::_OnComputeInternalInsetsListener::~_OnComputeInternalInsetsListener()
{}

PInterface InputMethodService::_OnComputeInternalInsetsListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IOnComputeInternalInsetsListener) {
        return (IOnComputeInternalInsetsListener*)this;
    }

    return NULL;
}

UInt32 InputMethodService::_OnComputeInternalInsetsListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::_OnComputeInternalInsetsListener::Release()
{
    return ElRefBase::Release();
}

ECode InputMethodService::_OnComputeInternalInsetsListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IOnComputeInternalInsetsListener*)this) {
        *pIID = EIID_IOnComputeInternalInsetsListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode InputMethodService::_OnComputeInternalInsetsListener::OnComputeInternalInsets(
    /* [in] */ IInternalInsetsInfo* info)
{
    assert(mHost != NULL && info != NULL);
    Boolean shown = FALSE;
    mHost->IsExtractViewShown(&shown);
    if (shown) {
        // In true fullscreen mode, we just say the window isn't covering
        // any content so we don't impact whatever is behind.
        AutoPtr<IDialog> dialog;
        mHost->GetWindow((IDialog**)&dialog);
        AutoPtr<IWindow> window;
        dialog->GetWindow((IWindow**)&window);
        AutoPtr<IView> decor;
        window->GetDecorView((IView**)&decor);
        Int32 h = 0;
        decor->GetHeight(&h);
        AutoPtr<IRect> contentInsets;
        info->GetContentInsets((IRect**)&contentInsets);
        contentInsets->SetTop(h);
        AutoPtr<IRect> visibleInsets;
        info->GetVisibleInsets((IRect**)&visibleInsets);
        visibleInsets->SetTop(h);

        AutoPtr<IRegion> touchableRegion;
        //((ViewTreeObserver::InternalInsetsInfo*)info)->GetTouchableRegion((IRegion**)&touchableRegion);
        info->GetTouchableRegion((IRegion**)&touchableRegion);
        touchableRegion->SetEmpty();
        info->SetTouchableInsets(IInternalInsetsInfo::TOUCHABLE_INSETS_FRAME);
    }
    else {
        mHost->OnComputeInsets(mHost->mTmpInsets);
        AutoPtr<IRect> contentInsets;
        info->GetContentInsets((IRect**)&contentInsets);
        contentInsets->SetTop(mHost->mTmpInsets->mContentTopInsets);
        AutoPtr<IRect> visibleInsets;
        info->GetVisibleInsets((IRect**)&visibleInsets);
        visibleInsets->SetTop(mHost->mTmpInsets->mVisibleTopInsets);

        AutoPtr<IRegion> touchableRegion;
        //((ViewTreeObserver::InternalInsetsInfo*)info)->GetTouchableRegion((IRegion**)&touchableRegion);
        info->GetTouchableRegion((IRegion**)&touchableRegion);
        Boolean tmp = FALSE;
        touchableRegion->Set(mHost->mTmpInsets->mTouchableRegion.Get(), &tmp);
        info->SetTouchableInsets(mHost->mTmpInsets->mTouchableInsets);
    }
    return NOERROR;
}

InputMethodService::_OnClickListener::_OnClickListener(
    /* [in] */ InputMethodService* host)
    : mHost(host)
{}

InputMethodService::_OnClickListener::~_OnClickListener()
{}

PInterface InputMethodService::_OnClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IViewOnClickListener) {
        return (IViewOnClickListener*)this;
    }

    return NULL;
}

UInt32 InputMethodService::_OnClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::_OnClickListener::Release()
{
    return ElRefBase::Release();
}

ECode InputMethodService::_OnClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IViewOnClickListener*)this) {
        *pIID = EIID_IViewOnClickListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode InputMethodService::_OnClickListener::OnClick(
    /* [in] */ IView* v)
{
    AutoPtr<IEditorInfo> ei;
    mHost->GetCurrentInputEditorInfo((IEditorInfo**)&ei);
    AutoPtr<IInputConnection> ic;
    mHost->GetCurrentInputConnection((IInputConnection**)&ic);
    if (ei != NULL && ic != NULL) {
        Boolean ret = FALSE;
        Int32 actionId, imeOptions;
        if (ei->GetActionId(&actionId), actionId != 0) {
            ic->PerformEditorAction(actionId, &ret);
        }
        else if (ei->GetImeOptions(&imeOptions), (imeOptions & IEditorInfo::IME_MASK_ACTION)
                != IEditorInfo::IME_ACTION_NONE) {
            ic->PerformEditorAction(imeOptions & IEditorInfo::IME_MASK_ACTION, &ret);
        }
    }
    return NOERROR;
}


InputMethodService::InputMethodImpl::InputMethodImpl(
    /* [in] */ InputMethodService* host)
    : AbstractInputMethodImpl((AbstractInputMethodService*)host)
    , mHost(host)
{}

InputMethodService::InputMethodImpl::~InputMethodImpl()
{}

ECode InputMethodService::InputMethodImpl::AttachToken(
    /* [in] */ IBinder* token)
{
    assert(mHost != NULL);
    if (mHost->mToken == NULL) {
        mHost->mToken = token;
        mHost->mWindow->SetToken(token);
    }
    return NOERROR;
}

ECode InputMethodService::InputMethodImpl::BindInput(
    /* [in] */ IInputBinding* binding)
{
    assert(mHost != NULL);
    mHost->mInputBinding = binding;
    mHost->mInputConnection = NULL;
    binding->GetConnection((IInputConnection**)&mHost->mInputConnection);
    if (DEBUG) Logger::V(TAG, "bindInput(): binding=%p,  ic=%p", binding, mHost->mInputConnection.Get());

    AutoPtr<IInputConnection> ic;
    mHost->GetCurrentInputConnection((IInputConnection**)&ic);
    if (ic != NULL) {
        Boolean tmpState = FALSE;
        ic->ReportFullscreenMode(mHost->mIsFullscreen, &tmpState);
    }
    mHost->Initialize();
    return mHost->OnBindInput();
}

ECode InputMethodService::InputMethodImpl::UnbindInput()
{
    assert(mHost != NULL);
    if (DEBUG) Logger::V(TAG, "unbindInput(): binding=%p, ic=%p", mHost->mInputBinding.Get(), mHost->mInputConnection.Get());
    mHost->OnUnbindInput();
    mHost->mInputStarted = FALSE;
    mHost->mInputBinding = NULL;
    mHost->mInputConnection = NULL;
    return NOERROR;
}

ECode InputMethodService::InputMethodImpl::StartInput(
    /* [in] */ IInputConnection* ic,
    /* [in] */ IEditorInfo* attribute)
{
    assert(mHost != NULL);
    if (DEBUG) Logger::V(TAG, "startInput(): editor=%p", attribute);
    mHost->DoStartInput(ic, attribute, FALSE);
    return NOERROR;
}

ECode InputMethodService::InputMethodImpl::RestartInput(
    /* [in] */ IInputConnection* ic,
    /* [in] */ IEditorInfo* attribute)
{
    assert(mHost != NULL);
    if (DEBUG) Logger::V(TAG, "restartInput(): editor=%p", attribute);
    mHost->DoStartInput(ic, attribute, TRUE);
    return NOERROR;
}

ECode InputMethodService::InputMethodImpl::HideSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    assert(mHost != NULL);
    if (DEBUG) Logger::V(TAG, "hideSoftInput()");
    Boolean wasVis = FALSE;
    mHost->IsInputViewShown(&wasVis);
    mHost->mShowInputFlags = 0;
    mHost->mShowInputRequested = FALSE;
    mHost->mShowInputForced = FALSE;
    mHost->DoHideWindow();
    if (resultReceiver != NULL) {
        Boolean tmpVis = FALSE;
        mHost->IsInputViewShown(&tmpVis);
        resultReceiver->Send(wasVis != tmpVis
                ? IInputMethodManager::RESULT_HIDDEN
                : (wasVis ? IInputMethodManager::RESULT_UNCHANGED_SHOWN
                        : IInputMethodManager::RESULT_UNCHANGED_HIDDEN), NULL);
    }
    return NOERROR;
}

ECode InputMethodService::InputMethodImpl::ShowSoftInput(
    /* [in] */ Int32 flags,
    /* [in] */ IResultReceiver* resultReceiver)
{
    assert(mHost != NULL);
    if (DEBUG) Logger::V(TAG, "showSoftInput()");
    Boolean wasVis = FALSE;
    mHost->IsInputViewShown(&wasVis);
    mHost->mShowInputFlags = 0;
    Boolean requested = FALSE;
    mHost->OnShowInputRequested(flags, FALSE, &requested);
    if (requested) {
        mHost->ShowWindow(TRUE);
    }

    // If user uses hard keyboard, IME button should always be shown.
    Boolean showing = FALSE;
    mHost->OnEvaluateInputViewShown(&showing);
    mHost->mImm->SetImeWindowStatus(mHost->mToken, IInputMethodService::IME_ACTIVE | (showing ? IInputMethodService::IME_VISIBLE : 0),
            mHost->mBackDisposition);

    if (resultReceiver != NULL) {
        Boolean tmpVis = FALSE;
        mHost->IsInputViewShown(&tmpVis);
        resultReceiver->Send(wasVis != tmpVis
                ? IInputMethodManager::RESULT_SHOWN
                : (wasVis ? IInputMethodManager::RESULT_UNCHANGED_SHOWN
                        : IInputMethodManager::RESULT_UNCHANGED_HIDDEN), NULL);
    }
    return NOERROR;
}

ECode InputMethodService::InputMethodImpl::ChangeInputMethodSubtype(
    /* [in] */ IInputMethodSubtype* subtype)
{
    mHost->OnCurrentInputMethodSubtypeChanged(subtype);
    return NOERROR;
}


InputMethodService::InputMethodSessionImpl::InputMethodSessionImpl(
    /* [in] */ InputMethodService* host)
    : AbstractInputMethodSessionImpl((AbstractInputMethodService*)host)
    , mHost(host)
{}

InputMethodService::InputMethodSessionImpl::~InputMethodSessionImpl()
{}

PInterface InputMethodService::InputMethodSessionImpl::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInputMethodSession == riid) {
        return (IInputMethodSession *)this;
    }

    if (EIID_IAbstractInputMethodSessionImpl == riid) {
        return (IAbstractInputMethodSessionImpl *)this;
    }

    return NULL;
}

UInt32 InputMethodService::InputMethodSessionImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::InputMethodSessionImpl::Release()
{
    return ElRefBase::Release();
}

ECode InputMethodService::InputMethodSessionImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IInputMethodSession*)this) {
        *pIID = EIID_IInputMethodSession;
    }
    else if (pObject == (IInterface*)(IAbstractInputMethodSessionImpl*)this) {
        *pIID = EIID_IAbstractInputMethodSessionImpl;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode InputMethodService::InputMethodSessionImpl::FinishInput()
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    if (DEBUG) Logger::V(TAG, "finishInput() in %p", this);
    mHost->DoFinishInput();
    return NOERROR;
}

ECode InputMethodService::InputMethodSessionImpl::DisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    mHost->mCurCompletions = completions;
    return mHost->OnDisplayCompletions(completions);
}

ECode InputMethodService::InputMethodSessionImpl::UpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    return mHost->OnUpdateExtractedText(token, text);
}

ECode InputMethodService::InputMethodSessionImpl::UpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    return mHost->OnUpdateSelection(oldSelStart, oldSelEnd,
            newSelStart, newSelEnd, candidatesStart, candidatesEnd);
}

ECode InputMethodService::InputMethodSessionImpl::ViewClicked(
    /* [in] */ Boolean focusChanged)
{
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }

    return mHost->OnViewClicked(focusChanged);
}

ECode InputMethodService::InputMethodSessionImpl::UpdateCursor(
    /* [in] */ IRect* newCursor)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    return mHost->OnUpdateCursor(newCursor);
}

ECode InputMethodService::InputMethodSessionImpl::AppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    assert(mHost != NULL);
    Boolean enabled = FALSE;
    if (IsEnabled(&enabled), !enabled) {
        return NOERROR;
    }
    return mHost->OnAppPrivateCommand(action, data);
}

ECode InputMethodService::InputMethodSessionImpl::ToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    mHost->OnToggleSoftInput(showFlags, hideFlags);
    return NOERROR;
}

ECode InputMethodService::InputMethodSessionImpl::DispatchGenericMotionEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* event,
    /* [in] */ ILocalInputMethodSessionEventCallback* callback)
{
    return AbstractInputMethodSessionImpl::DispatchGenericMotionEvent(seq, event, callback);
}


InputMethodService::Insets::Insets()
    : mContentTopInsets(0)
    , mVisibleTopInsets(0)
    , mTouchableInsets(0)
{
    CRegion::New((IRegion**)&mTouchableRegion);
}

UInt32 InputMethodService::Insets::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::Insets::Release()
{
    return ElRefBase::Release();
}

InputMethodService::InputMethodService()
    : mTheme(0)
    , mHardwareAccelerated(FALSE)
    , mInitialized(FALSE)
    , mWindowCreated(FALSE)
    , mWindowAdded(FALSE)
    , mWindowVisible(FALSE)
    , mWindowWasVisible(FALSE)
    , mInShowWindow(FALSE)
    , mInputStarted(FALSE)
    , mInputViewStarted(FALSE)
    , mCandidatesViewStarted(FALSE)
    , mShowInputFlags(0)
    , mShowInputRequested(FALSE)
    , mLastShowInputRequested(FALSE)
    , mCandidatesVisibility(0)
    , mCurCompletions(NULL)
    , mShowInputForced(FALSE)
    , mFullscreenApplied(FALSE)
    , mIsFullscreen(FALSE)
    , mExtractViewHidden(FALSE)
    , mExtractedToken(0)
    , mIsInputViewShown(FALSE)
    , mStatusIcon(0)
    , mBackDisposition(0)
{
    mTmpInsets = new InputMethodService::Insets();
    mTmpLocation = new Int32[2];
    mInsetsComputer = new _OnComputeInternalInsetsListener(this);
    mActionClickListener = new _OnClickListener(this);
}

InputMethodService::~InputMethodService()
{
    if (mTmpLocation != NULL) {
        delete mTmpLocation;
        mTmpLocation = NULL;
    }

}

UInt32 InputMethodService::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InputMethodService::Release()
{
    return ElRefBase::Release();
}

ECode InputMethodService::SetTheme(
    /* [in] */ Int32 theme)
{
    if (mWindow != NULL) {
        // throw new IllegalStateException("Must be called before onCreate()");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mTheme = theme;
    return NOERROR;
}

ECode InputMethodService::EnableHardwareAcceleration(
    /* [out] */ Boolean* enable)
{
    assert(enable != NULL);
    if (mWindow != NULL) {
        assert(0);
        // throw new IllegalStateException("Must be called before onCreate()");
    }

    AutoPtr<IActivityManagerHelper> helper;
    CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&helper);

    Boolean tmp = FALSE;
    if (helper->IsHighEndGfx(&tmp), tmp) {
        mHardwareAccelerated = TRUE;
        *enable = TRUE;
        return NOERROR;
    }

    *enable = FALSE;
    return NOERROR;
}

ECode InputMethodService::OnCreate()
{
    AutoPtr<IApplicationInfo> appInfo;
    GetApplicationInfo((IApplicationInfo**)&appInfo);
    Int32 targetSdkVersion = 0;
    appInfo->GetTargetSdkVersion(&targetSdkVersion);
    AutoPtr<IResourcesHelper> helper;
    CResourcesHelper::AcquireSingleton((IResourcesHelper**)&helper);
    helper->SelectSystemTheme(mTheme,
            targetSdkVersion,
            R::style::Theme_InputMethod,
            R::style::Theme_Holo_InputMethod,
            R::style::Theme_DeviceDefault_InputMethod,
            &mTheme);
    AbstractInputMethodService::SetTheme(mTheme);
    AbstractInputMethodService::OnCreate();
    AbstractInputMethodService::GetSystemService(
        IContext::INPUT_METHOD_SERVICE, (IInterface**)&mImm);
    AbstractInputMethodService::GetSystemService(
        IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&mInflater);
    CSoftInputWindow::New((IContext*)this->Probe(EIID_IContext),
            mTheme, mDispatcherState, (ISoftInputWindow**)&mWindow);

    AutoPtr<IWindow> window;
    mWindow->GetWindow((IWindow**)&window);
    if (mHardwareAccelerated) {
        window->AddFlags(IWindowManagerLayoutParams::FLAG_HARDWARE_ACCELERATED);
    }
    InitViews();
    return window->SetLayout(IViewGroupLayoutParams::MATCH_PARENT,
            IViewGroupLayoutParams::WRAP_CONTENT);
}

ECode InputMethodService::OnInitializeInterface()
{
    // Intentionally empty
    return NOERROR;
}

ECode InputMethodService::Initialize()
{
    if (!mInitialized) {
        mInitialized = TRUE;
        OnInitializeInterface();
    }
    return NOERROR;
}

void InputMethodService::InitViews()
{
    mInitialized = FALSE;
    mWindowCreated = FALSE;
    mShowInputRequested = FALSE;
    mShowInputForced = FALSE;

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::InputMethodService),
            ARRAY_SIZE(R::styleable::InputMethodService));
    AbstractInputMethodService::ObtainStyledAttributes(attrIds, (ITypedArray**)&mThemeAttrs);
    mInflater->Inflate(R::layout::input_method, NULL, (IView**)&mRootView);
    mWindow->SetContentViewEx(mRootView);
    AutoPtr<IViewTreeObserver> vto;
    mRootView->GetViewTreeObserver((IViewTreeObserver**)&vto);
    vto->AddOnComputeInternalInsetsListener(mInsetsComputer);
    // AutoPtr<IContentResolver> resolver;
    // GetContentResolver((IContentResolver**)&resolver);
    // Int32 value = 0;
    // AutoPtr<ISettingsGlobal> settingsGlobal;
    // CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&settingsGlobal);
    // settingsGlobal->GetInt32Ex(resolver,
    //         ISettingsGlobal::FANCY_IME_ANIMATIONS, 0, &value);
    // if (value != 0) {
    //     AutoPtr<IWindow> window;
    //     mWindow->GetWindow((IWindow**)&window);
    //     assert(window != NULL);
    //     window->SetWindowAnimations(
    //             R::style::Animation_InputMethodFancy);
    // }
    mRootView->FindViewById(R::id::fullscreenArea, (IView**)&mFullscreenArea);
    mExtractViewHidden = FALSE;
    mRootView->FindViewById(R::id::extractArea, (IView**)&mExtractFrame);
    mExtractView = NULL;
    mExtractEditText = NULL;
    mExtractAccessories = NULL;
    mExtractAction = NULL;
    mFullscreenApplied = FALSE;

    mRootView->FindViewById(R::id::candidatesArea, (IView**)&mCandidatesFrame);
    mRootView->FindViewById(R::id::inputArea, (IView**)&mInputFrame);
    mInputView = NULL;
    mIsInputViewShown = FALSE;

    mExtractFrame->SetVisibility(IView::GONE);
    GetCandidatesHiddenVisibility(&mCandidatesVisibility);
    mCandidatesFrame->SetVisibility(mCandidatesVisibility);
    mInputFrame->SetVisibility(IView::GONE);
}

ECode InputMethodService::OnDestroy()
{
    AbstractInputMethodService::OnDestroy();
    AutoPtr<IViewTreeObserver> observer;
    mRootView->GetViewTreeObserver((IViewTreeObserver**)&observer);
    assert(observer != NULL);
    observer->RemoveOnComputeInternalInsetsListener(mInsetsComputer);

    FinishViews();
    if (mWindowAdded) {
        // Disable exit animation for the current IME window
        // to avoid the race condition between the exit and enter animations
        // when the current IME is being switched to another one.
        AutoPtr<IWindow> window;
        mWindow->GetWindow((IWindow**)&window);
        assert(window != NULL);
        window->SetWindowAnimations(0);

        return mWindow->Dismiss();
    }
    return NOERROR;
}

ECode InputMethodService::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    AbstractInputMethodService::OnConfigurationChanged(newConfig);

    Boolean visible = mWindowVisible;
    Int32 showFlags = mShowInputFlags;
    Boolean showingInput = mShowInputRequested;
    AutoPtr<ArrayOf<ICompletionInfo*> > completions = mCurCompletions;
    InitViews();
    mInputViewStarted = FALSE;
    mCandidatesViewStarted = FALSE;
    if (mInputStarted) {
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        AutoPtr<IEditorInfo> ei;
        GetCurrentInputEditorInfo((IEditorInfo**)&ei);
        DoStartInput(ic, ei, TRUE);
    }
    if (visible) {
        if (showingInput) {
            // If we were last showing the soft keyboard, try to do so again.
            Boolean requested = FALSE;
            OnShowInputRequested(showFlags, TRUE, &requested);
            if (requested) {
                ShowWindow(TRUE);
                if (completions != NULL) {
                    //todo: maybe mCurCompletions should be free first
                    mCurCompletions = completions;
                    OnDisplayCompletions(completions);
                }
            }
            else {
                DoHideWindow();
            }
        }
        else if (mCandidatesVisibility == IView::VISIBLE) {
            // If the candidates are currently visible, make sure the
            // window is shown for them.
            ShowWindow(FALSE);
        }
        else {
            // Otherwise hide the window.
            DoHideWindow();
        }

        // If user uses hard keyboard, IME button should always be shown.
        Boolean showing = FALSE;
        OnEvaluateInputViewShown(&showing);
        mImm->SetImeWindowStatus(mToken, IInputMethodService::IME_ACTIVE | (showing ? IInputMethodService::IME_VISIBLE : 0),
                mBackDisposition);
    }
    return NOERROR;
}

ECode InputMethodService::OnCreateInputMethodInterface(
    /* [out] */ IAbstractInputMethodImpl** inputMethodImpl)
{
    VALIDATE_NOT_NULL(inputMethodImpl)
    *inputMethodImpl = new InputMethodImpl(this);
    INTERFACE_ADDREF(*inputMethodImpl);
    return NOERROR;
}

ECode InputMethodService::OnCreateInputMethodSessionInterface(
    /* [out] */ IAbstractInputMethodSessionImpl** abstractInputMethodSessionImpl)
{
    VALIDATE_NOT_NULL(abstractInputMethodSessionImpl)
    *abstractInputMethodSessionImpl = new InputMethodSessionImpl(this);
    INTERFACE_ADDREF(*abstractInputMethodSessionImpl);
    return NOERROR;
}

ECode InputMethodService::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    assert(inflater != NULL);
    *inflater = mInflater;
    INTERFACE_ADDREF(*inflater);
    return NOERROR;
}

ECode InputMethodService::GetWindow(
    /* [out] */ IDialog** dialog)
{
    assert(dialog != NULL);
    *dialog = mWindow;
    INTERFACE_ADDREF(*dialog);
    return NOERROR;
}

ECode InputMethodService::SetBackDisposition(
    /* [in] */ Int32 disposition)
{
    mBackDisposition = disposition;
    return NOERROR;
}

ECode InputMethodService::GetBackDisposition(
    /* [out] */ Int32* pos)
{
    assert(pos != NULL);
    *pos = mBackDisposition;
    return NOERROR;
}

ECode InputMethodService::GetMaxWidth(
    /* [out] */ Int32* maxWidth)
{
    assert(maxWidth != NULL);
    AutoPtr<IWindowManager> wm;
    AbstractInputMethodService::GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&wm);
    assert(wm != NULL);
    AutoPtr<IDisplay> display;
    wm->GetDefaultDisplay((IDisplay**)&display);
    assert(display != NULL);
    return display->GetWidth(maxWidth);
}

/**
 * Return the currently active InputBinding for the input method, or
 * NULL if there is none.
 */
ECode InputMethodService::GetCurrentInputBinding(
    /* [out] */ IInputBinding** binding)
{
    assert(binding != NULL);
    *binding = mInputBinding;
    INTERFACE_ADDREF(*binding);
    return NOERROR;
}

ECode InputMethodService::GetCurrentInputConnection(
    /* [out] */ IInputConnection** inputConnection)
{
    assert(inputConnection != NULL);
    if (mStartedInputConnection != NULL) {
        *inputConnection = mStartedInputConnection;
    }
    else {
        *inputConnection = mInputConnection;
    }
    INTERFACE_ADDREF(*inputConnection);
    return NOERROR;
}

ECode InputMethodService::GetCurrentInputStarted(
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = mInputStarted;
    return NOERROR;
}

ECode InputMethodService::GetCurrentInputEditorInfo(
    /* [out] */ IEditorInfo** editorInfo)
{
    assert(editorInfo != NULL);
    *editorInfo = mInputEditorInfo;
    INTERFACE_ADDREF(*editorInfo);
    return NOERROR;
}

ECode InputMethodService::UpdateFullscreenMode()
{
    Boolean tmpMode = FALSE;
    OnEvaluateFullscreenMode(&tmpMode);
    PFL_EX("TODO") // if isFullscreen = TRUE, SetExtractView will be crashed
    // Boolean isFullscreen = mShowInputRequested && tmpMode;
    Boolean isFullscreen = FALSE;
    Boolean changed = mLastShowInputRequested != mShowInputRequested;
    if (mIsFullscreen != isFullscreen || !mFullscreenApplied) {
        changed = TRUE;
        mIsFullscreen = isFullscreen;
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic != NULL) {
            Boolean tmpState = FALSE;
            ic->ReportFullscreenMode(isFullscreen, &tmpState);
        }
        mFullscreenApplied = TRUE;
        Initialize();
        assert(mFullscreenArea != NULL);
        AutoPtr<IViewGroupLayoutParams> _lp;
        mFullscreenArea->GetLayoutParams((IViewGroupLayoutParams**)&_lp);
        ILinearLayoutLayoutParams* lp = ILinearLayoutLayoutParams::Probe(_lp.Get());
        assert(lp != NULL);
        if (isFullscreen) {
            assert(mThemeAttrs != NULL);
            AutoPtr<IDrawable> drawable;
            mThemeAttrs->GetDrawable(
                R::styleable::InputMethodService_imeFullscreenBackground,
                (IDrawable**)&drawable);
            mFullscreenArea->SetBackgroundDrawable(drawable);
            lp->SetHeight(0);
            lp->SetWeight(1);
        }
        else {
            mFullscreenArea->SetBackgroundDrawable(NULL);
            lp->SetHeight(IViewGroupLayoutParams::WRAP_CONTENT);
            lp->SetWeight(0);
        }
        AutoPtr<IViewParent> parent;
        mFullscreenArea->GetParent((IViewParent**)&parent);
        assert(parent != NULL);
        AutoPtr<IViewManager> viewM = IViewManager::Probe(parent);
        assert(viewM != NULL);
        viewM->UpdateViewLayout(mFullscreenArea, lp);
        if (isFullscreen) {
            if (mExtractView == NULL) {
                AutoPtr<IView> v;
                OnCreateExtractTextView((IView**)&v);
                if (v != NULL) {
                    SetExtractView(v);
                }
            }
            StartExtractingText(FALSE);
        }
        UpdateExtractFrameVisibility();
    }

    if (changed) {
        AutoPtr<IWindow> window;
        mWindow->GetWindow((IWindow**)&window);
        OnConfigureWindow(window, isFullscreen, !mShowInputRequested);
        mLastShowInputRequested = mShowInputRequested;
    }
    return NOERROR;
}

ECode InputMethodService::OnConfigureWindow(
    /* [in] */ IWindow* win,
    /* [in] */ Boolean isFullscreen,
    /* [in] */ Boolean isCandidatesOnly)
{
    assert(mWindow != NULL);
    AutoPtr<IWindow> window;
    mWindow->GetWindow((IWindow**)&window);
    assert(window != NULL);
    AutoPtr<IWindowManagerLayoutParams> wmlp;
    window->GetAttributes((IWindowManagerLayoutParams**)&wmlp);
    Int32 currentHeight;
    wmlp->GetHeight(&currentHeight);
    Int32 newHeight = isFullscreen ? IViewGroupLayoutParams::MATCH_PARENT : IViewGroupLayoutParams::WRAP_CONTENT;
    if (mIsInputViewShown && currentHeight != newHeight) {
        Slogger::W(TAG, "Window size has been changed. This may cause jankiness of resizing window: %d -> %d"
                , currentHeight, newHeight);
    }
    window->SetLayout(IViewGroupLayoutParams::MATCH_PARENT, newHeight);
    return NOERROR;
}

ECode InputMethodService::IsFullscreenMode(
    /* [out] */ Boolean* screenMode)
{
    assert(screenMode != NULL);
    *screenMode = mIsFullscreen;
    return NOERROR;
}

/**
 * Override this to control when the input method should run in
 * fullscreen mode.  The default implementation runs in fullsceen only
 * when the screen is in landscape mode.  If you change what
 * this returns, you will need to call {@link #updateFullscreenMode()}
 * yourself whenever the returned value may have changed to have it
 * re-evaluated and applied.
 */
ECode InputMethodService::OnEvaluateFullscreenMode(
    /* [out] */ Boolean* screenMode)
{
    assert(screenMode != NULL);
    AutoPtr<IResources> res;
    AbstractInputMethodService::GetResources((IResources**)&res);
    assert(res != NULL);
    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**)&config);
    assert(config != NULL);
    Int32 orientation;
    config->GetOrientation(&orientation);
    if (orientation != IConfiguration::ORIENTATION_LANDSCAPE) {
        *screenMode = FALSE;
        return NOERROR;
    }
    Int32 imeOptions;
    if (mInputEditorInfo != NULL
            && (mInputEditorInfo->GetImeOptions(&imeOptions), (imeOptions & IEditorInfo::IME_FLAG_NO_FULLSCREEN) != 0)) {
        *screenMode = FALSE;
        return NOERROR;
    }
    *screenMode = TRUE;
    return NOERROR;
}

ECode InputMethodService::SetExtractViewShown(
    /* [in] */ Boolean shown)
{
    if (mExtractViewHidden == shown) {
        mExtractViewHidden = !shown;
        UpdateExtractFrameVisibility();
    }
    return NOERROR;
}

ECode InputMethodService::IsExtractViewShown(
    /* [out] */ Boolean* shown)
{
    assert(shown != NULL);
    *shown = mIsFullscreen && !mExtractViewHidden;
    return NOERROR;
}

void InputMethodService::UpdateExtractFrameVisibility()
{
    Int32 vis = 0;
    Boolean fullscreen = FALSE;
    if (IsFullscreenMode(&fullscreen), fullscreen) {
        vis = mExtractViewHidden ? IView::INVISIBLE : IView::VISIBLE;
        mExtractFrame->SetVisibility(IView::VISIBLE);
    }
    else {
        vis = IView::VISIBLE;
        mExtractFrame->SetVisibility(IView::GONE);
    }
    UpdateCandidatesVisibility(mCandidatesVisibility == IView::VISIBLE);
    Int32 tmpVis = 0;
    mFullscreenArea->GetVisibility(&tmpVis);
    if (mWindowWasVisible && tmpVis != vis) {
        Int32 animRes = 0;
        mThemeAttrs->GetResourceId(vis == IView::VISIBLE
                ? R::styleable::InputMethodService_imeExtractEnterAnimation
                : R::styleable::InputMethodService_imeExtractExitAnimation,
                0, &animRes);
        if (animRes != 0) {
            AutoPtr<IAnimation> ani;
#ifdef DROID_CORE
            AnimationUtils::LoadAnimation((IContext*)this->Probe(EIID_IContext),
                    animRes, (IAnimation**)&ani);
#else
            AutoPtr<IAnimationUtils> animationUtils;
            CAnimationUtils::AcquireSingleton((IAnimationUtils**)&animationUtils);
            assert(animationUtils != NULL);
            animationUtils->LoadAnimation((IContext*)this->Probe(EIID_IContext),
                    animRes, (IAnimation**)&ani);
#endif
            mFullscreenArea->StartAnimation(ani);
        }
    }
    mFullscreenArea->SetVisibility(vis);
}

ECode InputMethodService::OnComputeInsets(
    /* [in] */ Insets* outInsets)
{
    Int32 vis = 0;
    if (mInputFrame->GetVisibility(&vis), vis == IView::VISIBLE) {
        mInputFrame->GetLocationInWindow(&mTmpLocation[0], &mTmpLocation[1]);
    }
    else {
        AutoPtr<IDialog> dlg;
        GetWindow((IDialog**)&dlg);
        assert(dlg != NULL);
        AutoPtr<IWindow> win;
        dlg->GetWindow((IWindow**)&win);
        assert(win != NULL);
        AutoPtr<IView> decor;
        win->GetDecorView((IView**)&decor);
        assert(decor != NULL);
        decor->GetHeight(&mTmpLocation[1]);
    }
    Boolean ret = FALSE;
    if (IsFullscreenMode(&ret), ret) {
        // In fullscreen mode, we never resize the underlying window.
        AutoPtr<IDialog> dlg;
        GetWindow((IDialog**)&dlg);
        assert(dlg != NULL);
        AutoPtr<IWindow> win;
        dlg->GetWindow((IWindow**)&win);
        assert(win != NULL);
        AutoPtr<IView> decor;
        win->GetDecorView((IView**)&decor);
        assert(decor != NULL);
        decor->GetHeight(&outInsets->mContentTopInsets);
    }
    else {
        outInsets->mContentTopInsets = mTmpLocation[1];
    }
    if (mCandidatesFrame->GetVisibility(&vis), vis == IView::VISIBLE) {
        mCandidatesFrame->GetLocationInWindow(&mTmpLocation[0], &mTmpLocation[1]);
    }
    outInsets->mVisibleTopInsets = mTmpLocation[1];
    outInsets->mTouchableInsets = Insets::TOUCHABLE_INSETS_VISIBLE;
    outInsets->mTouchableRegion->SetEmpty();
    return NOERROR;
}

ECode InputMethodService::UpdateInputViewShown()
{
    Boolean isShown = FALSE;
    OnEvaluateInputViewShown(&isShown);
    isShown = mShowInputRequested && isShown;
    if (mIsInputViewShown != isShown && mWindowVisible) {
        mIsInputViewShown = isShown;
        mInputFrame->SetVisibility(isShown ? IView::VISIBLE : IView::GONE);
        if (mInputView == NULL) {
            Initialize();
            AutoPtr<IView> v;
            OnCreateInputView((IView**)&v);
            if (v != NULL) {
                return SetInputView(v);
            }
        }
    }
    return NOERROR;
}

ECode InputMethodService::IsShowInputRequested(
    /* [out] */ Boolean* requested)
{
    assert(requested != NULL);
    *requested = mShowInputRequested;
    return NOERROR;
}

ECode InputMethodService::IsInputViewShown(
    /* [out] */ Boolean* shown)
{
    assert(shown != NULL);
    *shown = mIsInputViewShown && mWindowVisible;
    return NOERROR;
}

ECode InputMethodService::OnEvaluateInputViewShown(
    /* [out] */ Boolean* shown)
{
    // assert(shown != NULL);

    // AutoPtr<IResources> res;
    // GetResources((IResources**)&res);
    // assert(res != NULL);
    // AutoPtr<IConfiguration> config;
    // res->GetConfiguration((IConfiguration**)&config);

    // Int32 keyboard, hardKeyboardHidden;
    // config->GetKeyboard(&keyboard);
    // config->GetHardKeyboardHidden(&hardKeyboardHidden);

    // *shown = keyboard == IConfiguration::KEYBOARD_NOKEYS
    //     || hardKeyboardHidden == IConfiguration::HARDKEYBOARDHIDDEN_YES;

    //todo:
    *shown = TRUE;
    return NOERROR;
}

ECode InputMethodService::SetCandidatesViewShown(
    /* [in] */ Boolean shown)
{
    UpdateCandidatesVisibility(shown);
    if (!mShowInputRequested && mWindowVisible != shown) {
        // If we are being asked to show the candidates view while the app
        // has not asked for the input view to be shown, then we need
        // to update whether the window is shown.
        if (shown) {
            ShowWindow(FALSE);
        }
        else {
            DoHideWindow();
        }
    }
    return NOERROR;
}

void InputMethodService::UpdateCandidatesVisibility(
    /* [in] */ Boolean shown)
{
    Int32 vis = IView::VISIBLE;
    if (!shown) {
        GetCandidatesHiddenVisibility(&vis);
    }
    if (mCandidatesVisibility != vis) {
        mCandidatesFrame->SetVisibility(vis);
        mCandidatesVisibility = vis;
    }
}

ECode InputMethodService::GetCandidatesHiddenVisibility(
    /* [out] */ Int32* vis)
{
    assert(vis != NULL);
    Boolean shown = FALSE;
    IsExtractViewShown(&shown);
    *vis = shown ? IView::GONE : IView::INVISIBLE;
    return NOERROR;
}

ECode InputMethodService::ShowStatusIcon(
    /* [in] */ Int32 iconResId)
{
    mStatusIcon = iconResId;
    String packageName;
    GetPackageName(&packageName);
    return mImm->ShowStatusIcon(mToken, packageName, iconResId);
}

ECode InputMethodService::HideStatusIcon()
{
    mStatusIcon = 0;
    return mImm->HideStatusIcon(mToken);
}

ECode InputMethodService::SwitchInputMethod(
    /* [in] */ const String& id)
{
    return mImm->SetInputMethod(mToken, id);
}

ECode InputMethodService::SetExtractView(
    /* [in] */ IView* view)
{
    mExtractFrame->RemoveAllViews();
    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(
        IViewGroupLayoutParams::MATCH_PARENT, IViewGroupLayoutParams::MATCH_PARENT,
        (IViewGroupLayoutParams**)&params);
    mExtractFrame->AddViewEx3(view, params.Get());
    mExtractView = view;

    mExtractEditText = NULL;
    mExtractAccessories = NULL;
    mExtractAction = NULL;

    if (view != NULL) {
        view->FindViewById(R::id::inputExtractEditText,
            (IView**)&mExtractEditText);
        mExtractEditText->SetIME(THIS_PROBE(IInputMethodService));
        view->FindViewById(R::id::inputExtractAction,
            (IView**)&mExtractAction);
        if (mExtractAction != NULL) {
            view->FindViewById(R::id::inputExtractAccessories,
                (IView**)&mExtractAccessories);
        }
        StartExtractingText(FALSE);
    }

    return NOERROR;
}

/**
 * Replaces the current candidates view with a new one.  You only need to
 * call this when dynamically changing the view; normally, you should
 * implement {@link #onCreateCandidatesView()} and create your view when
 * first needed by the input method.
 */
ECode InputMethodService::SetCandidatesView(
    /* [in] */ IView* view)
{
    mCandidatesFrame->RemoveAllViews();
    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(
        IViewGroupLayoutParams::MATCH_PARENT, IViewGroupLayoutParams::WRAP_CONTENT,
        (IViewGroupLayoutParams**)&params);
    return mCandidatesFrame->AddViewEx3(view, params.Get());
}

/**
 * Replaces the current input view with a new one.  You only need to
 * call this when dynamically changing the view; normally, you should
 * implement {@link #onCreateInputView()} and create your view when
 * first needed by the input method.
 */
ECode InputMethodService::SetInputView(
    /* [in] */ IView* view)
{
    mInputFrame->RemoveAllViews();
    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(
        IViewGroupLayoutParams::MATCH_PARENT, IViewGroupLayoutParams::WRAP_CONTENT,
        (IViewGroupLayoutParams**)&params);
    mInputFrame->AddViewEx3(view, params.Get());
    mInputView = view;
    return NOERROR;
}

ECode InputMethodService::OnCreateExtractTextView(
    /* [out] */ IView** view)
{
    assert(mInflater != NULL);
    return mInflater->Inflate(R::layout::input_method_extract_view,
            NULL, view);
}

ECode InputMethodService::OnCreateCandidatesView(
    /* [out] */ IView** view)
{
    assert(view != NULL);
    *view = NULL;
    return NOERROR;
}

ECode InputMethodService::OnCreateInputView(
    /* [out] */ IView** view)
{
    assert(view != NULL);
    *view = NULL;
    return NOERROR;
}

ECode InputMethodService::OnStartInputView(
    /* [in] */ IEditorInfo* info,
    /* [in] */ Boolean restarting)
{
    // Intentionally empty
    return NOERROR;
}

/**
 * Called when the input view is being hidden from the user.  This will
 * be called either prior to hiding the window, or prior to switching to
 * another target for editing.
 *
 * <p>The default
 * implementation uses the InputConnection to clear any active composing
 * text; you can override this (not calling the base class implementation)
 * to perform whatever behavior you would like.
 *
 * @param finishingInput If TRUE, {@link #onFinishInput} will be
 * called immediately after.
 */
ECode InputMethodService::OnFinishInputView(
    /* [in] */ Boolean finishingInput)
{
    if (!finishingInput) {
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic != NULL) {
            Boolean flag = FALSE;
            return ic->FinishComposingText(&flag);
        }
    }
    return NOERROR;
}

ECode InputMethodService::OnStartCandidatesView(
    /* [in] */ IEditorInfo* info,
    /* [in] */ Boolean restarting)
{
    return NOERROR;
}

ECode InputMethodService::OnFinishCandidatesView(
    /* [in] */ Boolean finishingInput)
{
    if (!finishingInput) {
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic != NULL) {
            Boolean flag = FALSE;
            return ic->FinishComposingText(&flag);
        }
    }
    return NOERROR;
}

ECode InputMethodService::OnShowInputRequested(
    /* [in] */ Int32 flags,
    /* [in] */ Boolean configChange,
    /* [out] */ Boolean* requested)
{
    assert(requested != NULL);
    Boolean result = FALSE;
    if (OnEvaluateInputViewShown(&result), !result) {
        *requested = FALSE;
        return NOERROR;
    }
    if ((flags & IInputMethod::SHOW_EXPLICIT) == 0) {
        if (!configChange && (OnEvaluateFullscreenMode(&result), result)) {
            // Don't show if this is not explicitly requested by the user and
            // the input method is fullscreen.  That would be too disruptive.
            // However, we skip this change for a config change, since if
            // the IME is already shown we do want to go into fullscreen
            // mode at this point.
            *requested = FALSE;
            return NOERROR;
        }
        AutoPtr<IResources> res;
        AbstractInputMethodService::GetResources((IResources**)&res);
        AutoPtr<IConfiguration> config;
        res->GetConfiguration((IConfiguration**)&config);
        Int32 keyboard;
        config->GetKeyboard(&keyboard);
        if (keyboard != IConfiguration::KEYBOARD_NOKEYS) {
            // And if the device has a hard keyboard, even if it is
            // currently hidden, don't show the input method implicitly.
            // These kinds of devices don't need it that much.
            *requested = FALSE;
            return NOERROR;
        }
    }
    if ((flags & IInputMethod::SHOW_FORCED) != 0) {
        mShowInputForced = TRUE;
    }
    *requested = TRUE;
    return NOERROR;
}

ECode InputMethodService::ShowWindow(
    /* [in] */ Boolean showInput)
{
    // if (DEBUG) Logger::V(TAG, "Showing window: showInput=" + showInput
    //         + " mShowInputRequested=" + mShowInputRequested
    //         + " mWindowAdded=" + mWindowAdded
    //         + " mWindowCreated=" + mWindowCreated
    //         + " mWindowVisible=" + mWindowVisible
    //         + " mInputStarted=" + mInputStarted);

    if (mInShowWindow) {
        // Log.w(TAG, "Re-entrance in to showWindow");
        return NOERROR;
    }

    mWindowWasVisible = mWindowVisible;
    mInShowWindow = TRUE;
    ShowWindowInner(showInput);
    mWindowWasVisible = TRUE;
    mInShowWindow = FALSE;
    return NOERROR;
}

void InputMethodService::ShowWindowInner(
    /* [in] */ Boolean showInput)
{
    Boolean doShowInput = FALSE;
    Boolean wasVisible = mWindowVisible;
    mWindowVisible = TRUE;
    if (!mShowInputRequested) {
        if (mInputStarted) {
            if (showInput) {
                doShowInput = TRUE;
                mShowInputRequested = TRUE;
            }
        }
    }
    else {
        showInput = TRUE;
    }

    // if (DEBUG) Logger::V(TAG, "showWindow: updating UI");
    Initialize();
    UpdateFullscreenMode();
    UpdateInputViewShown();

    if (!mWindowAdded || !mWindowCreated) {
        mWindowAdded = TRUE;
        mWindowCreated = TRUE;
        Initialize();
        // if (DEBUG) Logger::V(TAG, "CALL: onCreateCandidatesView");
        AutoPtr<IView> v;
        OnCreateCandidatesView((IView**)&v);
        // if (DEBUG) Logger::V(TAG, "showWindow: candidates=" + v);
        if (v != NULL) {
            SetCandidatesView(v);
        }
    }
    if (mShowInputRequested) {
        if (!mInputViewStarted) {
            // if (DEBUG) Logger::V(TAG, "CALL: onStartInputView");
            mInputViewStarted = TRUE;
            OnStartInputView(mInputEditorInfo, FALSE);
        }
    }
    else if (!mCandidatesViewStarted) {
        // if (DEBUG) Logger::V(TAG, "CALL: onStartCandidatesView");
        mCandidatesViewStarted = TRUE;
        OnStartCandidatesView(mInputEditorInfo, FALSE);
    }

    if (doShowInput) {
        StartExtractingText(FALSE);
    }

    if (!wasVisible) {
        // if (DEBUG) Logger::V(TAG, "showWindow: showing!");
        OnWindowShown();
        mWindow->Show();
    }
}

void InputMethodService::FinishViews()
{
    if (mInputViewStarted) {
        // if (DEBUG) Logger::V(TAG, "CALL: onFinishInputView");
        OnFinishInputView(FALSE);
    } else if (mCandidatesViewStarted) {
        // if (DEBUG) Logger::V(TAG, "CALL: onFinishCandidatesView");
        OnFinishCandidatesView(FALSE);
    }
    mInputViewStarted = FALSE;
    mCandidatesViewStarted = FALSE;
}

void InputMethodService::DoHideWindow()
{
    mImm->SetImeWindowStatus(mToken, 0, mBackDisposition);
    HideWindow();
}

ECode InputMethodService::HideWindow()
{
    FinishViews();
    if (mWindowVisible) {
        mWindow->Hide();
        mWindowVisible = FALSE;
        OnWindowHidden();
        mWindowWasVisible = FALSE;
    }
    return NOERROR;
}

/**
 * Called when the input method window has been shown to the user, after
 * previously not being visible.  This is done after all of the UI setup
 * for the window has occurred (creating its views etc).
 */
ECode InputMethodService::OnWindowShown()
{
    // Intentionally empty.
    return NOERROR;
}

/**
 * Called when the input method window has been hidden from the user,
 * after previously being visible.
 */
ECode InputMethodService::OnWindowHidden()
{
    // Intentionally empty.
    return NOERROR;
}

ECode InputMethodService::OnBindInput()
{
    // Intentionally empty.
    return NOERROR;
}

ECode InputMethodService::OnUnbindInput()
{
    // Intentionally empty.
    return NOERROR;
}

ECode InputMethodService::OnStartInput(
    /* [in] */ IEditorInfo* attribute,
    /* [in] */ Boolean restarting)
{
    // Intentionally empty.
    return NOERROR;
}

void InputMethodService::DoFinishInput()
{
    if (mInputViewStarted) {
        if (DEBUG) Logger::V(TAG, "CALL: onFinishInputView");
        OnFinishInputView(TRUE);
    } else if (mCandidatesViewStarted) {
        if (DEBUG) Logger::V(TAG, "CALL: onFinishCandidatesView");
        OnFinishCandidatesView(TRUE);
    }
    mInputViewStarted = FALSE;
    mCandidatesViewStarted = FALSE;
    if (mInputStarted) {
        if (DEBUG) Logger::V(TAG, "CALL: onFinishInput");
        OnFinishInput();
    }
    mInputStarted = FALSE;
    mStartedInputConnection = NULL;
    mCurCompletions = NULL;
}

void InputMethodService::DoStartInput(
    /* [in] */ IInputConnection* ic,
    /* [in] */ IEditorInfo* attribute,
    /* [in] */ Boolean restarting)
{
    if (!restarting) {
        DoFinishInput();
    }
    mInputStarted = TRUE;
    mStartedInputConnection = ic;
    mInputEditorInfo = attribute;
    Initialize();
    // if (DEBUG) Logger::V(TAG, "CALL: onStartInput");
    OnStartInput(attribute, restarting);
    if (mWindowVisible) {
        if (mShowInputRequested) {
            // if (DEBUG) Logger::V(TAG, "CALL: onStartInputView");
            mInputViewStarted = TRUE;
            OnStartInputView(mInputEditorInfo, restarting);
            StartExtractingText(TRUE);
        }
        else if (mCandidatesVisibility == IView::VISIBLE) {
            // if (DEBUG) Logger::V(TAG, "CALL: onStartCandidatesView");
            mCandidatesViewStarted = TRUE;
            OnStartCandidatesView(mInputEditorInfo, restarting);
        }
    }
}

ECode InputMethodService::OnFinishInput()
{
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**)&ic);
    if (ic != NULL) {
        Boolean flag = FALSE;
        return ic->FinishComposingText(&flag);
    }
    return NOERROR;
}

ECode InputMethodService::OnDisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    // Intentionally empty.
    return NOERROR;
}

ECode InputMethodService::OnUpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    if (mExtractedToken != token) {
        return NOERROR;
    }
    if (text != NULL) {
        if (mExtractEditText != NULL) {
            mExtractedText = text;
            return mExtractEditText->SetExtractedText(text);
        }
    }
    return NOERROR;
}

ECode InputMethodService::OnUpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    Boolean fullscreen = FALSE;
    IsFullscreenMode(&fullscreen);
    if (mExtractEditText != NULL && fullscreen && mExtractedText != NULL) {
        Int32 off;
        mExtractedText->GetStartOffset(&off);
        mExtractEditText->StartInternalChanges();
        newSelStart -= off;
        newSelEnd -= off;
        Int32 len = 0;
        AutoPtr<ICharSequence> text;
        mExtractEditText->GetText((ICharSequence**)&text);
        assert(text != NULL);
        text->GetLength(&len);
        if (newSelStart < 0) {
            newSelStart = 0;
        }
        else if (newSelStart > len) {
            newSelStart = len;
        }
        if (newSelEnd < 0) {
            newSelEnd = 0;
        }
        else if (newSelEnd > len) {
            newSelEnd = len;
        }

        mExtractEditText->SetSelection(newSelStart, newSelEnd);
        mExtractEditText->FinishInternalChanges();
    }
    return NOERROR;
}

ECode InputMethodService::OnViewClicked(
    /* [in] */ Boolean focusChanged)
{
    // Intentionally empty
    return NOERROR;
}

ECode InputMethodService::OnUpdateCursor(
    /* [in] */ IRect* newCursor)
{
    // Intentionally empty
    return NOERROR;
}

ECode InputMethodService::RequestHideSelf(
    /* [in] */ Int32 flags)
{
    return mImm->HideSoftInputFromInputMethod(mToken, flags);
}

ECode InputMethodService::RequestShowSelf(
    /* [in] */ Int32 flags)
{
    return mImm->ShowSoftInputFromInputMethod(mToken, flags);
}

Boolean InputMethodService::HandleBack(
    /* [in] */ Boolean doIt)
{
    if (mShowInputRequested) {
        Boolean tmp = FALSE;
        IsExtractViewShown(&tmp);
        if (tmp && IExtractEditLayout::Probe(mExtractView) != NULL) {
            AutoPtr<IExtractEditLayout> extractEditLayout = IExtractEditLayout::Probe(mExtractView);

            Boolean started = FALSE;
            if (extractEditLayout->IsActionModeStarted(&started), started) {
                if (doIt) extractEditLayout->FinishActionMode();
                return TRUE;
            }
        }

        // If the soft input area is shown, back closes it and we
        // consume the back key.
        if (doIt) RequestHideSelf(0);
        return TRUE;
    }
    else if (mWindowVisible) {
        if (mCandidatesVisibility == IView::VISIBLE) {
            // If we are showing candidates even if no input area, then
            // hide them.
            if (doIt) SetCandidatesViewShown(FALSE);
        }
        else {
            // If we have the window visible for some other reason --
            // most likely to show candidates -- then just get rid
            // of it.  This really shouldn't happen, but just in case...
            if (doIt) DoHideWindow();
        }
        return TRUE;
    }
    return FALSE;
}

ECode InputMethodService::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state)
{
    assert(event != NULL && state != NULL);
    if (keyCode == IKeyEvent::KEYCODE_BACK) {
        if (HandleBack(FALSE)) {
            event->StartTracking();
            *state = TRUE;
            return NOERROR;
        }
        *state = FALSE;
        return NOERROR;
    }
   *state = DoMovementKey(keyCode, event, MOVEMENT_DOWN);
    return NOERROR;
}

ECode InputMethodService::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = FALSE;
    return NOERROR;
}

ECode InputMethodService::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = DoMovementKey(keyCode, event, count);
    return NOERROR;
}

ECode InputMethodService::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* state)
{
    assert(event != NULL && state != NULL);
    Int32 code = 0;
    event->GetKeyCode(&code);
    Boolean tracking = FALSE;
    event->IsTracking(&tracking);
    Boolean canceld = FALSE;
    event->IsCanceled(&canceld);
    if (code == IKeyEvent::KEYCODE_BACK && tracking && !canceld) {
        *state = HandleBack(TRUE);
        return NOERROR;
    }
    *state = DoMovementKey(keyCode, event, MOVEMENT_UP);
    return NOERROR;
}

ECode InputMethodService::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = FALSE;
    return NOERROR;
}

ECode InputMethodService::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
    // if (DEBUG) Logger::V(TAG, "onGenericMotionEvent(): event " + event);
    assert(state != NULL);
    *state = FALSE;
    return NOERROR;
}

ECode InputMethodService::OnAppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    return NOERROR;
}

void InputMethodService::OnToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    // if (DEBUG) Logger::V(TAG, "toggleSoftInput()");
    Boolean shown = FALSE;
    IsInputViewShown(&shown);
    if (shown) {
        RequestHideSelf(hideFlags);
    }
    else {
        RequestShowSelf(showFlags);
    }
}

void InputMethodService::ReportExtractedMovement(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count)
{
    Int32 dx = 0, dy = 0;
    switch (keyCode) {
        case IKeyEvent::KEYCODE_DPAD_LEFT:
            dx = -count;
            break;
        case IKeyEvent::KEYCODE_DPAD_RIGHT:
            dx = count;
            break;
        case IKeyEvent::KEYCODE_DPAD_UP:
            dy = -count;
            break;
        case IKeyEvent::KEYCODE_DPAD_DOWN:
            dy = count;
            break;
    }
    OnExtractedCursorMovement(dx, dy);
}

Boolean InputMethodService::DoMovementKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 count)
{
    Boolean extractViewShown = FALSE, inputViewShown = FALSE;
    IsExtractViewShown(&extractViewShown);
    IsInputViewShown(&inputViewShown);
    if (extractViewShown && inputViewShown && mExtractEditText != NULL) {
        // If we are in fullscreen mode, the cursor will move around
        // the extract edit text, but should NOT cause focus to move
        // to other fields.
        AutoPtr<IMovementMethod> movement;
        mExtractEditText->GetMovementMethod((IMovementMethod**)&movement);
        AutoPtr<ILayout> layout;
        mExtractEditText->GetLayout((ILayout**)&layout);
        if (movement != NULL && layout != NULL) {
            // We want our own movement method to handle the key, so the
            // cursor will properly move in our own word wrapping.
            AutoPtr<ICharSequence> text;
            mExtractEditText->GetText((ICharSequence**)&text);
            Boolean keyRet = FALSE;
            if (count == MOVEMENT_DOWN) {
                movement->OnKeyDown(mExtractEditText, ISpannable::Probe(text),
                    keyCode, event, &keyRet);
                if (keyRet) {
                    ReportExtractedMovement(keyCode, 1);
                    return TRUE;
                }
            }
            else if (count == MOVEMENT_UP) {
                movement->OnKeyUp(mExtractEditText, ISpannable::Probe(text),
                    keyCode, event, &keyRet);
                if (keyRet) {
                    return TRUE;
                }
            }
            else {
                movement->OnKeyOther(mExtractEditText, ISpannable::Probe(text),
                    event, &keyRet);
                if (keyRet) {
                    ReportExtractedMovement(keyCode, count);
                }
                else {
                    AutoPtr<IKeyEvent> down;
                    // CKeyEvent::ChangeAction(event, KeyEvent_ACTION_DOWN, (IKeyEvent**)&down);
                    movement->OnKeyDown(mExtractEditText, ISpannable::Probe(text),
                            keyCode, down, &keyRet);
                    if (keyRet) {
                        AutoPtr<IKeyEvent> up;
                        // CKeyEvent::ChangeAction(event, KeyEvent_ACTION_UP, (IKeyEvent**)&up);
                        movement->OnKeyUp(mExtractEditText, ISpannable::Probe(text),
                                keyCode, up, &keyRet);
                        while (--count > 0) {
                            movement->OnKeyDown(mExtractEditText,
                                    ISpannable::Probe(text), keyCode, down, &keyRet);

                            movement->OnKeyUp(mExtractEditText,
                                    ISpannable::Probe(text), keyCode, up, &keyRet);
                        }
                        ReportExtractedMovement(keyCode, count);
                    }
                }
            }
        }
        // Regardless of whether the movement method handled the key,
        // we never allow DPAD navigation to the application.
        switch (keyCode) {
            case IKeyEvent::KEYCODE_DPAD_LEFT:
            case IKeyEvent::KEYCODE_DPAD_RIGHT:
            case IKeyEvent::KEYCODE_DPAD_UP:
            case IKeyEvent::KEYCODE_DPAD_DOWN:
                return TRUE;
        }
    }

    return FALSE;
}

ECode InputMethodService::SendDownUpKeyEvents(
    /* [in] */ Int32 keyEventCode)
{
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**)&ic);
    if (ic == NULL) return NOERROR;
    long eventTime = SystemClock::GetUptimeMillis();
    AutoPtr<IKeyEvent> event;
    CKeyEvent::New(eventTime, eventTime,
            IKeyEvent::ACTION_DOWN, keyEventCode, 0, 0, IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
            IKeyEvent::FLAG_SOFT_KEYBOARD | IKeyEvent::FLAG_KEEP_TOUCH_MODE,
            (IKeyEvent**)&event);
    Boolean tmpState = FALSE;
    ic->SendKeyEvent(event, &tmpState);

    event = NULL;
    CKeyEvent::New(SystemClock::GetUptimeMillis(), eventTime,
            IKeyEvent::ACTION_UP, keyEventCode, 0, 0, IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
            IKeyEvent::FLAG_SOFT_KEYBOARD | IKeyEvent::FLAG_KEEP_TOUCH_MODE,
            (IKeyEvent**)&event);
    return ic->SendKeyEvent(event, &tmpState);
}

ECode InputMethodService::SendDefaultEditorAction(
    /* [in] */ Boolean fromEnterKey,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    AutoPtr<IEditorInfo> ei;
    GetCurrentInputEditorInfo((IEditorInfo**)&ei);
    Int32 imeOptions;
    if (ei != NULL &&
        (!fromEnterKey || (ei->GetImeOptions(&imeOptions), (imeOptions & IEditorInfo::IME_FLAG_NO_ENTER_ACTION) == 0)) &&
        (imeOptions & IEditorInfo::IME_MASK_ACTION) != IEditorInfo::IME_ACTION_NONE) {
        // If the enter key was pressed, and the editor has a default
        // action associated with pressing enter, then send it that
        // explicit action instead of the key event.
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic != NULL) {
            Boolean tmpState = FALSE;
            ic->PerformEditorAction(imeOptions & IEditorInfo::IME_MASK_ACTION, &tmpState);
        }
        *state = TRUE;
        return NOERROR;
    }
    *state = FALSE;
    return NOERROR;
}

ECode InputMethodService::SendKeyChar(
    /* [in] */ Char32 charCode)
{
    switch (charCode) {
        case '\n': // Apps may be listening to an enter key to perform an action
            {
                Boolean ret = FALSE;
                SendDefaultEditorAction(TRUE, &ret);
                if (!ret) {
                    SendDownUpKeyEvents(IKeyEvent::KEYCODE_ENTER);
                }
                break;
            }
        default:
            {
                // Make sure that digits go through any text watcher on the client side.
                if (charCode >= '0' && charCode <= '9') {
                    SendDownUpKeyEvents(charCode - '0' + IKeyEvent::KEYCODE_0);
                }
                else {
                    AutoPtr<IInputConnection> ic;
                    GetCurrentInputConnection((IInputConnection**)&ic);
                    if (ic != NULL) {
                        String str("");
                        str.Append(charCode);
                        AutoPtr<ICharSequence> cs;
                        CStringWrapper::New(str, (ICharSequence**)&cs);
                        Boolean tmpState = FALSE;
                        ic->CommitText(cs, 1, &tmpState);
                    }
                }
                break;
            }
    }
    return NOERROR;
}

ECode InputMethodService::OnExtractedSelectionChanged(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<IInputConnection> conn;
    GetCurrentInputConnection((IInputConnection**)&conn);
    if (conn != NULL) {
        Boolean tmpState = FALSE;
        return conn->SetSelection(start, end, &tmpState);
    }
    return NOERROR;
}

ECode InputMethodService::OnExtractedDeleteText(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<IInputConnection> conn;
    GetCurrentInputConnection((IInputConnection**)&conn);
    if (conn != NULL) {
        Boolean tmpState = FALSE;
        conn->SetSelection(start, start, &tmpState);
        conn->DeleteSurroundingText(0, end - start, &tmpState);
    }

    return NOERROR;
}

ECode InputMethodService::OnExtractedReplaceText(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* text)
{
    AutoPtr<IInputConnection> conn;
    GetCurrentInputConnection((IInputConnection**)&conn);
    if (conn != NULL) {
        Boolean tmp = FALSE;
        conn->SetComposingRegion(start, end, &tmp);
        conn->CommitText(text, 1, &tmp);
    }

    return NOERROR;
}

ECode InputMethodService::OnExtractedSetSpan(
    /* [in] */ IInterface* span,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    AutoPtr<IInputConnection> conn;
    GetCurrentInputConnection((IInputConnection**)&conn);
    if (conn != NULL) {
        Boolean state = FALSE;
        if (!(conn->SetSelection(start, end, &state), state)) return NOERROR;

        AutoPtr<ICharSequence> text;
        conn->GetSelectedText(IInputConnection::GET_TEXT_WITH_STYLES, (ICharSequence**)&text);
        if (ISpannable::Probe(text) != NULL) {
            Int32 len = 0;
            text->GetLength(&len);
            ISpannable::Probe(text)->SetSpan(span, 0, len, flags);

            Boolean tmp = FALSE;
            conn->SetComposingRegion(start, end, &tmp);
            conn->CommitText(text, 1, &tmp);
        }
    }

    return NOERROR;
}

ECode InputMethodService::OnExtractedTextClicked()
{
    if (mExtractEditText == NULL) {
        return NOERROR;
    }
    Boolean has = FALSE;
    mExtractEditText->HasVerticalScrollBar(&has);
    if (has) {
        return SetCandidatesViewShown(FALSE);
    }
    return NOERROR;
}

ECode InputMethodService::OnExtractedCursorMovement(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    if (mExtractEditText == NULL || dy == 0) {
        return NOERROR;
    }
    Boolean has = FALSE;
    mExtractEditText->HasVerticalScrollBar(&has);
    if (has) {
        return SetCandidatesViewShown(FALSE);
    }
    return NOERROR;
}

ECode InputMethodService::OnExtractTextContextMenuItem(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    AutoPtr<IInputConnection> ic;
    GetCurrentInputConnection((IInputConnection**)&ic);
    if (ic != NULL) {
        Boolean tmpState = FALSE;
        ic->PerformContextMenuAction(id, &tmpState);
    }
    *state = TRUE;
    return NOERROR;
}

ECode InputMethodService::GetTextForImeAction(
    /* [in] */ Int32 imeOptions,
    /* [out] */ ICharSequence** text)
{
    assert(text != NULL);
    switch (imeOptions & IEditorInfo::IME_MASK_ACTION) {
        case IEditorInfo::IME_ACTION_NONE:
            *text = NULL;
            return NOERROR;
        case IEditorInfo::IME_ACTION_GO:
            return AbstractInputMethodService::GetText(
                    R::string::ime_action_go, text);
        case IEditorInfo::IME_ACTION_SEARCH:
            return AbstractInputMethodService::GetText(
                    R::string::ime_action_search, text);
        case IEditorInfo::IME_ACTION_SEND:
            return AbstractInputMethodService::GetText(
                    R::string::ime_action_send, text);
        case IEditorInfo::IME_ACTION_NEXT:
            return AbstractInputMethodService::GetText(
                    R::string::ime_action_next, text);
        case IEditorInfo::IME_ACTION_DONE:
            return AbstractInputMethodService::GetText(
                    R::string::ime_action_done, text);
        case IEditorInfo::IME_ACTION_PREVIOUS:
            return AbstractInputMethodService::GetText(
                R::string::ime_action_previous, text);
        default:
            return AbstractInputMethodService::GetText(
                    R::string::ime_action_default, text);
    }
}

ECode InputMethodService::OnUpdateExtractingVisibility(
    /* [in] */ IEditorInfo* ei)
{
    assert(ei != NULL);
    Int32 inputType, imeOptions;
    ei->GetInputType(&inputType);
    ei->GetImeOptions(&imeOptions);
    if (inputType == IInputType::TYPE_NULL ||
            (imeOptions & IEditorInfo::IME_FLAG_NO_EXTRACT_UI) != 0) {
        // No reason to show extract UI!
        return SetExtractViewShown(FALSE);
    }
    return SetExtractViewShown(TRUE);
}

ECode InputMethodService::OnUpdateExtractingViews(
    /* [in] */ IEditorInfo* ei)
{
    Boolean shown = FALSE;
    IsExtractViewShown(&shown);
    if (!shown) {
        return NOERROR;
    }

    if (mExtractAccessories == NULL) {
        return NOERROR;
    }
    AutoPtr<ICharSequence> actionLabel;
    ei->GetActionLabel((ICharSequence**)&actionLabel);
    Int32 imeOptions, inputType;
    ei->GetImeOptions(&imeOptions);
    ei->GetInputType(&inputType);
    Boolean hasAction = actionLabel != NULL || (
            (imeOptions & IEditorInfo::IME_MASK_ACTION) != IEditorInfo::IME_ACTION_NONE &&
            (imeOptions & IEditorInfo::IME_FLAG_NO_ACCESSORY_ACTION) == 0 &&
            inputType != IInputType::TYPE_NULL);
    if (hasAction) {
        mExtractAccessories->SetVisibility(IView::VISIBLE);
        if (mExtractAction != NULL) {
            if (actionLabel != NULL) {
                mExtractAction->SetText(actionLabel);
            }
            else {
                AutoPtr<ICharSequence> text;
                GetTextForImeAction(imeOptions, (ICharSequence**)&text);
                mExtractAction->SetText(text);
            }

            mExtractAction->SetOnClickListener(mActionClickListener);
        }
    }
    else {
        mExtractAccessories->SetVisibility(IView::GONE);
        if (mExtractAction != NULL) {
            mExtractAction->SetOnClickListener(NULL);
        }
    }
    return NOERROR;
}

ECode InputMethodService::OnExtractingInputChanged(
    /* [in] */ IEditorInfo* ei)
{
    Int32 inputType;
    ei->GetInputType(&inputType);
    if (inputType == IInputType::TYPE_NULL) {
        return RequestHideSelf(IInputMethodManager::HIDE_NOT_ALWAYS);
    }
    return NOERROR;
}

void InputMethodService::StartExtractingText(
    /* [in] */ Boolean inputChanged)
{
    Boolean tmpState = FALSE, fullscreen = FALSE;
    GetCurrentInputStarted(&tmpState);
    IsFullscreenMode(&fullscreen);
    if (mExtractEditText != NULL && tmpState && fullscreen) {
        mExtractedToken++;
        AutoPtr<IExtractedTextRequest> req;
        CExtractedTextRequest::New((IExtractedTextRequest**)&req);
        req->SetToken(mExtractedToken);
        req->SetFlags(IInputConnection::GET_TEXT_WITH_STYLES);
        req->SetHintMaxLines(10);
        req->SetHintMaxChars(10000);
        AutoPtr<IInputConnection> ic;
        GetCurrentInputConnection((IInputConnection**)&ic);
        if (ic == NULL) {
            mExtractedText = NULL;
        }
        else {
            mExtractedText = NULL;
            ic->GetExtractedText(req, IInputConnection::GET_EXTRACTED_TEXT_MONITOR,
                    (IExtractedText**)&mExtractedText);
        }
        if (mExtractedText == NULL || ic == NULL) {
            // Log.e(TAG, "Unexpected NULL in startExtractingText : mExtractedText = "
            //         + mExtractedText + ", input connection = " + ic);
        }
        AutoPtr<IEditorInfo> ei;
        GetCurrentInputEditorInfo((IEditorInfo**)&ei);

        // try {
        mExtractEditText->StartInternalChanges();
        OnUpdateExtractingVisibility(ei);
        OnUpdateExtractingViews(ei);
        Int32 inputType;
        ei->GetInputType(&inputType);
        if ((inputType & IInputType::TYPE_MASK_CLASS)
                == IInputType::TYPE_CLASS_TEXT) {
            if ((inputType & IInputType::TYPE_TEXT_FLAG_IME_MULTI_LINE) != 0) {
                inputType |= IInputType::TYPE_TEXT_FLAG_MULTI_LINE;
            }
        }
        mExtractEditText->SetInputType(inputType);
        AutoPtr<ICharSequence> hintText;
        ei->GetHintText((ICharSequence**)&hintText);
        mExtractEditText->SetHint(hintText);
        if (mExtractedText != NULL) {
            mExtractEditText->SetEnabled(TRUE);
            mExtractEditText->SetExtractedText(mExtractedText);
        }
        else {
            mExtractEditText->SetEnabled(FALSE);
            AutoPtr<ICharSequence> tmpText;
            CStringWrapper::New(String(""), (ICharSequence**)&tmpText);
            mExtractEditText->SetText(tmpText);
        }
        // } finally {
        //     eet.finishInternalChanges();
        // }
        mExtractEditText->FinishInternalChanges();

        if (inputChanged) {
            OnExtractingInputChanged(ei);
        }
    }
}

void InputMethodService::OnCurrentInputMethodSubtypeChanged(
    /* [in] */ IInputMethodSubtype* newSubtype)
{
    // if (DEBUG) {
    //     int nameResId = newSubtype.getNameResId();
    //     String mode = newSubtype.getMode();
    //     String output = "changeInputMethodSubtype:"
    //         + (nameResId == 0 ? "<none>" : getString(nameResId)) + ","
    //         + mode + ","
    //         + newSubtype.getLocale() + "," + newSubtype.getExtraValue();
    //     Log.v(TAG, "--- " + output);
    // }
}

ECode InputMethodService::GetBaseContext(
    /* [out] */ IContext** context)
{
    return AbstractInputMethodService::GetBaseContext(context);
}

ECode InputMethodService::Attach(
    /* [in] */ IContext* ctx,
    /* [in] */ IActivityThread* apartment,
    /* [in] */ const String& className,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IIActivityManager* activityManager)
{
    return AbstractInputMethodService::Attach(ctx, apartment, className,
            token, application, activityManager);
}

ECode InputMethodService::GetClassName(
    /* [out] */ String* className)
{
    return AbstractInputMethodService::GetClassName(className);
}

ECode InputMethodService::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** dispatcherState)
{
    return AbstractInputMethodService::GetKeyDispatcherState(dispatcherState);
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos