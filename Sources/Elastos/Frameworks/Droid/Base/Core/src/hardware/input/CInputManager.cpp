
#include "hardware/input/CInputManager.h"
#include "hardware/input/CInputManagerInputDevicesChangedListener.h"
#include "os/CBinder.h"
#include "os/Looper.h"
#include "os/CServiceManager.h"
#include "os/ServiceManager.h"
#include "os/SystemClock.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Os::CBinder;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::EIID_IVibrator;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::CServiceManager;
using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::IIPowerManager;
//using Elastos::Droid::Provider::ISettingsSystem;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Input {

const char* CInputManager::TAG = "InputManager";
const Boolean CInputManager::DEBUG;
const Int32 CInputManager::MSG_DEVICE_ADDED;
const Int32 CInputManager::MSG_DEVICE_REMOVED;
const Int32 CInputManager::MSG_DEVICE_CHANGED;

AutoPtr<IInputManager> CInputManager::sInstance;
Mutex CInputManager::sInstanceLock;

CInputManager::InputDeviceListenerDelegate::InputDeviceListenerDelegate(
    /* [in] */ IInputDeviceListener* listener,
    /* [in] */ ILooper* looper)
    : HandlerBase(looper)
    , mListener(listener)
{
}

ECode CInputManager::InputDeviceListenerDelegate::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, deviceId;
    msg->GetWhat(&what);
    msg->GetArg1(&deviceId);

    switch (what) {
        case MSG_DEVICE_ADDED:
            mListener->OnInputDeviceAdded(deviceId);
            break;
        case MSG_DEVICE_REMOVED:
            mListener->OnInputDeviceRemoved(deviceId);
            break;
        case MSG_DEVICE_CHANGED:
            mListener->OnInputDeviceChanged(deviceId);
            break;
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(CInputManager::InputDeviceVibrator, IVibrator);

CInputManager::InputDeviceVibrator::InputDeviceVibrator(
    /* [in] */ Int32 deviceId,
    /* [in] */ CInputManager* owner)
    : mDeviceId(deviceId)
    , mOwner(owner)
{
    ASSERT_SUCCEEDED(CBinder::New((IBinder**)&mToken));
}

ECode CInputManager::InputDeviceVibrator::HasVibrator(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = TRUE;
    return NOERROR;
}

ECode CInputManager::InputDeviceVibrator::Vibrate(
    /* [in] */ Int64 milliseconds)
{
    AutoPtr<ArrayOf<Int64> > temp = ArrayOf<Int64>::Alloc(2);
    (*temp)[0] = 0;
    (*temp)[1] = milliseconds;
    return VibrateEx(*temp, -1);
}

ECode CInputManager::InputDeviceVibrator::VibrateEx(
    /* [in] */ const ArrayOf<Int64>& pattern,
    /* [in] */ Int32 repeat)
{
    if (repeat >= pattern.GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (FAILED(mOwner->mIm->Vibrate(mDeviceId, pattern, repeat, mToken))) {
        Logger::W(TAG, "Failed to vibrate.");
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

ECode CInputManager::InputDeviceVibrator::Cancel()
{
    if (FAILED(mOwner->mIm->CancelVibrate(mDeviceId, mToken))) {
        Logger::W(TAG, "Failed to cancel vibration.");
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

CInputManager::CInputManager()
{
    AutoPtr<IServiceManager> sm;
    ASSERT_SUCCEEDED(CServiceManager::AcquireSingleton((IServiceManager**)&sm));
    AutoPtr<IInterface> service;
    ASSERT_SUCCEEDED(sm->GetService(IContext::INPUT_SERVICE, (IInterface**)&service));
    mIm = IIInputManager::Probe(service);
}

CInputManager::~CInputManager()
{
    mInputDevices = NULL;
    mInputDeviceListeners.Clear();
}

AutoPtr<IInputManager> CInputManager::GetInstance()
{
    Mutex::Autolock lock(sInstanceLock);

    if (sInstance == NULL) {
        ASSERT_SUCCEEDED(CInputManager::New((IInputManager**)&sInstance));
    }

    return sInstance;
}

ECode CInputManager::GetInputDevice(
    /* [in] */ Int32 id,
    /* [out] */ IInputDevice** device)
{
    VALIDATE_NOT_NULL(device);

    Mutex::Autolock lock(mInputDevicesLock);
    FAIL_RETURN(PopulateInputDevicesLocked());

    HashMap<Int32, AutoPtr<IInputDevice> >::Iterator find = mInputDevices->Find(id);
    if (find == mInputDevices->End()) {
        *device = NULL;
        return NOERROR;
    }

    AutoPtr<IInputDevice> inputDevice = find->mSecond;
    if (inputDevice == NULL) {
        if (FAILED(mIm->GetInputDevice(id, (IInputDevice**)&inputDevice))) {
            Logger::D(TAG, "Could not get input device information.");
            return E_RUNTIME_EXCEPTION;
        }
    }
    find->mSecond = inputDevice;
    assert(inputDevice != NULL);
    *device = inputDevice;
    INTERFACE_ADDREF(*device);

    return NOERROR;
}

ECode CInputManager::GetInputDeviceByDescriptor(
    /* [in] */ const String& descriptor,
    /* [out] */ IInputDevice** device)
{
    VALIDATE_NOT_NULL(device);

    if (descriptor.IsNull()) {
        Logger::E(TAG, "descriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mInputDevicesLock);

    PopulateInputDevicesLocked();

    HashMap<Int32, AutoPtr<IInputDevice> >::Iterator iter = mInputDevices->Begin();
    for (; iter != mInputDevices->End(); ++iter) {
        AutoPtr<IInputDevice> inputDevice = iter->mSecond;
        if (inputDevice == NULL) {
            Int32 id = iter->mFirst;
            if (FAILED(mIm->GetInputDevice(id, (IInputDevice**)&inputDevice))); {
                continue;
            }

            assert(inputDevice != NULL);
            iter->mSecond = inputDevice;
        }

        String descriptor2;
        inputDevice->GetDescriptor(&descriptor2);
        if (descriptor.Equals(descriptor2)) {
            *device = inputDevice;
            INTERFACE_ADDREF(*device);
            return NOERROR;
        }
    }
    *device = NULL;

    return NOERROR;
}

ECode CInputManager::GetInputDeviceIds(
    /* [out, callee] */ ArrayOf<Int32>** deviceIds)
{
    VALIDATE_NOT_NULL(deviceIds);

    Mutex::Autolock lock(mInputDevicesLock);
    FAIL_RETURN(PopulateInputDevicesLocked());

    Int32 count = mInputDevices->GetSize();
    *deviceIds = ArrayOf<Int32>::Alloc(count);
    if (*deviceIds != NULL)
        return E_OUT_OF_MEMORY_ERROR;

    INTERFACE_ADDREF(*deviceIds);

    HashMap<Int32, AutoPtr<IInputDevice> >::Iterator iter = mInputDevices->Begin();
    for (Int32 i = 0; iter != mInputDevices->End(); ++iter, ++i) {
        (**deviceIds)[i] = iter->mFirst;
    }

    return NOERROR;
}

ECode CInputManager::RegisterInputDeviceListener(
    /* [in] */ IInputDeviceListener* listener,
    /* [in] */ IHandler* handler)
{
    if (listener == NULL) {
        Logger::E(TAG, "listener must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ILooper> looper;
    if (NULL == handler) {
        looper = Looper::MyLooper();
    }
    else {
        handler->GetLooper((ILooper**)&looper);
    }

    Mutex::Autolock lock(mInputDevicesLock);
    List<AutoPtr<InputDeviceListenerDelegate> >::Iterator find =
        FindInputDeviceListenerLocked(listener);
    if (find == mInputDeviceListeners.End()) {
        AutoPtr<InputDeviceListenerDelegate> delegate = new InputDeviceListenerDelegate(listener, looper);
        mInputDeviceListeners.PushBack(delegate);
    }

    return NOERROR;
}

ECode CInputManager::UnregisterInputDeviceListener(
    /* [in] */ IInputDeviceListener* listener)
{
    if (listener == NULL) {
        Logger::E(TAG, "listener must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mInputDevicesLock);
    List<AutoPtr<InputDeviceListenerDelegate> >::Iterator find =
        FindInputDeviceListenerLocked(listener);
    if (find != mInputDeviceListeners.End()) {
        (*find)->RemoveCallbacksAndMessages(NULL);
        mInputDeviceListeners.Erase(find);
    }

    return NOERROR;
}

List<AutoPtr<CInputManager::InputDeviceListenerDelegate> >::Iterator
CInputManager::FindInputDeviceListenerLocked(
    /* [in] */ IInputDeviceListener* listener)
{
    List<AutoPtr<InputDeviceListenerDelegate> >::Iterator iter =
        mInputDeviceListeners.Begin();
    for (; iter != mInputDeviceListeners.End(); ++iter) {
        if ((*iter)->mListener.Get() == listener)
            return iter;
    }

    return mInputDeviceListeners.End();
}

ECode CInputManager::GetKeyboardLayouts(
    /* [out, callee] */ ArrayOf<IKeyboardLayout*>** layouts)
{
    VALIDATE_NOT_NULL(layouts);
    if (FAILED(mIm->GetKeyboardLayouts(layouts))) {
        Logger::W(TAG, "Could not get list of keyboard layout informations.");
        *layouts = ArrayOf<IKeyboardLayout*>::Alloc(0);
        INTERFACE_ADDREF(*layouts);
    }

    return NOERROR;
}

ECode CInputManager::GetKeyboardLayout(
    /* [in] */ const String& keyboardLayoutDescriptor,
    /* [out] */ IKeyboardLayout** layout)
{
    VALIDATE_NOT_NULL(layout);

    if (keyboardLayoutDescriptor.IsNull()) {
        Logger::E(TAG, "keyboardLayoutDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mIm->GetKeyboardLayout(keyboardLayoutDescriptor, layout))) {
        Logger::W(TAG, "Could not getkeyboard layout information.");
        *layout = NULL;
    }

    return NOERROR;
}

ECode CInputManager::GetCurrentKeyboardLayoutForInputDevice(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [out] */ String* keyboardLayoutDescriptor)
{
    VALIDATE_NOT_NULL(keyboardLayoutDescriptor);

    if (inputDeviceDescriptor.IsNull()) {
        Logger::E(TAG, "inputDeviceDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mIm->GetCurrentKeyboardLayoutForInputDevice(
        inputDeviceDescriptor, keyboardLayoutDescriptor))) {
        Logger::W(TAG, "Could not get current keyboard layout for input device.");
        keyboardLayoutDescriptor = NULL;
    }

    return NOERROR;
}

ECode CInputManager::SetCurrentKeyboardLayoutForInputDevice(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ const String& keyboardLayoutDescriptor)
{
    if (inputDeviceDescriptor.IsNull()) {
        Logger::E(TAG, "inputDeviceDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (keyboardLayoutDescriptor.IsNull()) {
        Logger::E(TAG, "keyboardLayoutDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mIm->SetCurrentKeyboardLayoutForInputDevice(
        inputDeviceDescriptor, keyboardLayoutDescriptor))) {
        Logger::W(TAG, "Could not set current keyboard layout for input device.");
    }

    return NOERROR;
}

ECode CInputManager::GetKeyboardLayoutsForInputDevice(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [out, callee] */ ArrayOf<String>** keyboardLayoutDescriptors)
{
    VALIDATE_NOT_NULL(keyboardLayoutDescriptors);
    *keyboardLayoutDescriptors = NULL;

    if (inputDeviceDescriptor.IsNull()) {
        Logger::E(TAG, "inputDeviceDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mIm->GetKeyboardLayoutsForInputDevice(
        inputDeviceDescriptor, keyboardLayoutDescriptors))) {
        Logger::W(TAG, "Could not get keyboard layouts for input device.");
        *keyboardLayoutDescriptors = ArrayOf<String>::Alloc(0);
        INTERFACE_ADDREF(*keyboardLayoutDescriptors);
    }

    return NOERROR;
}

ECode CInputManager::AddKeyboardLayoutForInputDevice(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ const String& keyboardLayoutDescriptor)
{
    if (inputDeviceDescriptor.IsNull()) {
        Logger::E(TAG, "inputDeviceDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (keyboardLayoutDescriptor.IsNull()) {
        Logger::E(TAG, "keyboardLayoutDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mIm->AddKeyboardLayoutForInputDevice(
        inputDeviceDescriptor, keyboardLayoutDescriptor))) {
        Logger::W(TAG, "Could not add keyboard layout for input device.");
    }

    return NOERROR;
}

ECode CInputManager::RemoveKeyboardLayoutForInputDevice(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ const String& keyboardLayoutDescriptor)
{
    if (inputDeviceDescriptor.IsNull()) {
        Logger::E(TAG, "inputDeviceDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (keyboardLayoutDescriptor.IsNull()) {
        Logger::E(TAG, "keyboardLayoutDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mIm->RemoveKeyboardLayoutForInputDevice(
        inputDeviceDescriptor, keyboardLayoutDescriptor))) {
        Logger::W(TAG, "Could not remove keyboard layout for input device.");
    }

    return NOERROR;
}

ECode CInputManager::GetPointerSpeed(
    /* [in] */ IContext* context,
    /* [out] */ Int32* speed)
{
    VALIDATE_NOT_NULL(speed);
    assert(context);

    *speed = DEFAULT_POINTER_SPEED;

    // AutoPtr<IContentResolver> cr;
    // context->GetContentResolver((IContentResolver)&cr);
    // AutoPtr<ISettingsSystem> ss;
    // CSettingsSystem::AcquireSingleton((ISettingsSystem**)&ss);
    // ss->GetInt32(cr, ISettingsSystem::POINTER_SPEED, speed);

    return NOERROR;
}

ECode CInputManager::SetPointerSpeed(
    /* [in] */ IContext* context,
    /* [in] */ Int32 speed)
{
    if (speed < MIN_POINTER_SPEED || speed > MAX_POINTER_SPEED) {
        Logger::E(TAG, "speed out of range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // AutoPtr<IContentResolver> cr;
    // context->GetContentResolver((IContentResolver)&cr);
    // AutoPtr<ISettingsSystem> ss;
    // CSettingsSystem::AcquireSingleton((ISettingsSystem**)&ss);
    // return ss->PutInt32(cr, ISettingsSystem::POINTER_SPEED, speed);
    return E_NOT_IMPLEMENTED;
}

ECode CInputManager::TryPointerSpeed(
    /* [in] */ Int32 speed)
{
    if (speed < MIN_POINTER_SPEED || speed > MAX_POINTER_SPEED) {
        Logger::E(TAG, "speed out of range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mIm->TryPointerSpeed(speed))) {
        Logger::W(TAG, "Could not set temporary pointer speed.");
    }

    return NOERROR;
}

ECode CInputManager::DeviceHasKeys(
    /* [in] */ const ArrayOf<Int32>& keyCodes,
    /* [out, callee] */ ArrayOf<Boolean>** hasKeys)
{
    VALIDATE_NOT_NULL(hasKeys);
    *hasKeys = ArrayOf<Boolean>::Alloc(keyCodes.GetLength());

    if (NULL == *hasKeys)
        return E_OUT_OF_MEMORY_ERROR;

    INTERFACE_ADDREF(*hasKeys);
    Boolean res;
    mIm->HasKeys(-1, IInputDevice::SOURCE_ANY, keyCodes, *hasKeys, &res);

    return NOERROR;
}

ECode CInputManager::InjectInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    if (event == NULL) {
        Logger::E(TAG, "event must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mode != INJECT_INPUT_EVENT_MODE_ASYNC
        && mode != INJECT_INPUT_EVENT_MODE_WAIT_FOR_FINISH
        && mode != INJECT_INPUT_EVENT_MODE_WAIT_FOR_RESULT) {
        Logger::E(TAG, "mode is invalid");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mIm->InjectInputEvent(event, mode, res))) {
        *res = FALSE;
    }

    return NOERROR;
}

ECode CInputManager::PopulateInputDevicesLocked()
{
    if (mInputDevicesChangedListener == NULL) {
        AutoPtr<IInputDevicesChangedListener> listener;
        CInputManagerInputDevicesChangedListener::New(
            (Handle32)this, (IInputDevicesChangedListener**)&listener);
        if (FAILED(mIm->RegisterInputDevicesChangedListener(listener))) {
            Logger::E(TAG, "Could not get register input device changed listener");
            return E_RUNTIME_EXCEPTION;
        }

        mInputDevicesChangedListener = listener;
    }

    if (mInputDevices == NULL) {
        AutoPtr<ArrayOf<Int32> > ids;
        if (FAILED(mIm->GetInputDeviceIds((ArrayOf<Int32>**)&ids))) {
            Logger::E(TAG, "Could not get input device ids.");
            return E_RUNTIME_EXCEPTION;
        }

        mInputDevices = new HashMap<Int32, AutoPtr<IInputDevice> >;
        Int32 length = ids->GetLength();
        for (Int32 i = 0; i < length; i++) {
            (*mInputDevices)[(*ids)[i]] = NULL;
        }
    }

    return NOERROR;
}

void CInputManager::OnInputDevicesChanged(
    /* [in] */ ArrayOf<Int32>* deviceIdAndGeneration)
{
    if (DEBUG) {
        Logger::D(TAG, "Received input devices changed.");
    }

    Mutex::Autolock lock(mInputDevicesLock);
    //TODO: from size - 1 to 0
    //
    HashMap<Int32, AutoPtr<IInputDevice> >::Iterator iter = mInputDevices->Begin();
    for (; iter != mInputDevices->End();) {
        Int32 deviceId = iter->mFirst;
        if (!ContainsDeviceId(deviceIdAndGeneration, deviceId)) {
            if (DEBUG) {
                Logger::D(TAG, "Device removed: %d", deviceId);
            }

            mInputDevices->Erase(iter++);
            SendMessageToInputDeviceListenersLocked(MSG_DEVICE_REMOVED, deviceId);
        }
        else {
            ++iter;
        }
    }

    Int32 length = deviceIdAndGeneration->GetLength();
    for (Int32 i = 0; i < length; i += 2) {
        Int32 deviceId = (*deviceIdAndGeneration)[i];
        HashMap<Int32, AutoPtr<IInputDevice> >::Iterator find
            = mInputDevices->Find(deviceId);
        if (iter != mInputDevices->End()) {
            AutoPtr<IInputDevice> device = find->mSecond;
            if (device != NULL) {
                Int32 generation = (*deviceIdAndGeneration)[i + 1];
                Int32 generation2;
                device->GetGeneration(&generation2);
                if (generation2 != generation) {
                    if (DEBUG) {
                        Logger::D(TAG, "Device changed: %d", deviceId);
                    }
                    find->mSecond = NULL;
                    SendMessageToInputDeviceListenersLocked(MSG_DEVICE_CHANGED, deviceId);
                }
            }
        }
        else {
            if (DEBUG) {
                Logger::D(TAG, "Device added: %d" + deviceId);
            }
            (*mInputDevices)[deviceId] = NULL;
            SendMessageToInputDeviceListenersLocked(MSG_DEVICE_ADDED, deviceId);

            /**
            *add by hechuanlong 2013-08-13 start{{--------------------------------
            *When screen status is off and usb input device plug-in, wake up android!
             */
            AutoPtr<IInterface> service = ServiceManager::GetService(IContext::POWER_SERVICE);
            AutoPtr<IIPowerManager> ipm = IIPowerManager::Probe(service.Get());
            Boolean isScreenOn = FALSE;
            ipm->IsScreenOn(&isScreenOn);

            if( !isScreenOn ) {
                ipm->WakeUp(SystemClock::GetUptimeMillis());
            }
            /**
            *When screen status is off and usb input device plug-in, wake up android!
            *add by hechuanlong 2013-08-13
            *end---------------------------------------}}
            */
        }
    }
}

void CInputManager::SendMessageToInputDeviceListenersLocked(
    /* [in] */ Int32 what,
    /* [in] */ Int32 deviceId)
{
    Boolean result;
    List<AutoPtr<InputDeviceListenerDelegate> >::Iterator iter = mInputDeviceListeners.Begin();
    for (; iter != mInputDeviceListeners.End(); ++iter) {
        AutoPtr<IMessage> msg;
        (*iter)->ObtainMessageEx2(what, deviceId, 0, (IMessage**)&msg);
        Boolean result;
        (*iter)->SendMessage(msg, &result);
    }
}

Boolean CInputManager::ContainsDeviceId(
    /* [in] */ ArrayOf<Int32>* deviceIdAndGeneration,
    /* [in] */ Int32 deviceId)
{
    Int32 length = deviceIdAndGeneration->GetLength();
    for (Int32 i = 0; i < length; i += 2) {
        if ((*deviceIdAndGeneration)[i] == deviceId) {
            return TRUE;
        }
    }
    return FALSE;
}

ECode CInputManager::GetInputDeviceVibrator(
    /* [in] */ Int32 deviceId,
    /* [out] */ IVibrator** vibrator)
{
    VALIDATE_NOT_NULL(vibrator);
    *vibrator = new InputDeviceVibrator(deviceId, this);
    if (NULL == *vibrator)
        return E_OUT_OF_MEMORY_ERROR;

    INTERFACE_ADDREF(*vibrator);
    return NOERROR;
}

} // namespace Input
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos
