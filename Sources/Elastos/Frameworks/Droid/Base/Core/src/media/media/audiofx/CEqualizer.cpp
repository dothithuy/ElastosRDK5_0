#include "media/media/audiofx/CEqualizer.h"
#include "media/media/audiofx/CEqualizerSettings.h"

using Elastos::Utility::IUUIDHelper;
using Elastos::Utility::CUUIDHelper;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

const String CEqualizer::TAG("BassBoost");
const Int32 CEqualizer::PARAM_PROPERTIES = 9;

IAUDIOEFFECT_METHODS_IMPL(CEqualizer, AudioEffect)

CAR_INTERFACE_IMPL(CEqualizer::BaseParameterListener, IAudioEffectOnParameterChangeListener)

ECode CEqualizer::BaseParameterListener::OnParameterChange(
    /* [in] */ IAudioEffect* effect,
    /* [in] */ Int32 status,
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Byte>* value)
{
    AutoPtr<IEqualizerOnParameterChangeListener> l;
    {
        Mutex::Autolock lock(&mHost->mParamListenerLock);
        if (mHost->mParamListener != NULL) {
            l = mHost->mParamListener;
        }
    }
    if (l != NULL) {
        Int32 p1 = -1;
        Int32 p2 = -1;
        Int32 v = -1;

        if (param->GetLength() >= 4) {
            mHost->ByteArrayToInt32Ex(param, 0, &p1);
            if (param->GetLength() >= 8) {
                mHost->ByteArrayToInt32Ex(param, 4, &p2);
            }
        }
        if (value->GetLength() == 2) {
            Int16 status;
            mHost->ByteArrayToInt16Ex(value, 0, &status);
            v = (Int32) status;
        } else if (value->GetLength() == 4) {
            mHost->ByteArrayToInt32Ex(value, 0, &v);
        }
        if (p1 != -1 && v != -1) {
            l->OnParameterChange((IEqualizer*)mHost, status, p1, p2, v);
        }
    }
    return NOERROR;
}

CEqualizer::CEqualizer()
    : mNumBands(0)
    , mNumPresets(0)
{
}

ECode CEqualizer::constructor(
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    // super(EFFECT_TYPE_EQUALIZER, EFFECT_TYPE_NULL, priority, audioSession);
    AutoPtr<IUUIDHelper> helper;
    CUUIDHelper::AcquireSingleton((IUUIDHelper**)&helper);
    AutoPtr<IUUID> typeEQUALIZER;
    AutoPtr<IUUID> typeNULL;
    helper->FromString(IAudioEffect::EFFECT_TYPE_EQUALIZER, (IUUID**)&typeEQUALIZER);
    helper->FromString(IAudioEffect::EFFECT_TYPE_NULL, (IUUID**)&typeNULL);
    Init(typeEQUALIZER, typeNULL, priority, audioSession);

    if (audioSession == 0) {
        //Log.w(TAG, "WARNING: attaching an Equalizer to global output mix is deprecated!");
    }

    Int16 numOfBands;
    GetNumberOfBands(&numOfBands);
    Int16 tempInt16;
    GetNumberOfPresets(&tempInt16);
    mNumPresets = (Int32) tempInt16;
    if (mNumPresets != 0) {
        mPresetNames = ArrayOf<String>::Alloc(mNumPresets);
        AutoPtr<ArrayOf<Byte> > value = ArrayOf<Byte>::Alloc(PARAM_STRING_SIZE_MAX);
        AutoPtr<ArrayOf<Int32> > param = ArrayOf<Int32>::Alloc(2);
        param->Set(0, IEqualizer::PARAM_GET_PRESET_NAME);
        for (int i = 0; i < mNumPresets; i++) {
            param->Set(1, i);
            Int32 status;
            GetParameterEx6(param, value, &status);
            FAIL_RETURN(CheckStatus(status));
            Int32 length = 0 ;
            while ((*value)[length] != 0 ) length++;

            // try {
            String s((const char*)value->GetPayload(), length);
            mPresetNames->Set(i, s);
            // } catch (java.io.UnsupportedEncodingException e) {
            //     Log.e(TAG, "preset name decode error");
            // }

        }
    }
    return NOERROR;
}

PInterface CEqualizer::Probe(
    /* [in] */ REIID riid)
{
    return _CEqualizer::Probe(riid);
}

ECode CEqualizer::GetNumberOfBands(
    /* [out] */ Int16* numBands)
{
    VALIDATE_NOT_NULL(numBands);

    if (mNumBands != 0) {
        *numBands = mNumBands;
        return NOERROR;
    }
    ArrayOf_<Int32,1> param;
    param[0] = IEqualizer::PARAM_NUM_BANDS;
    ArrayOf_<Int16,1> result;
    Int32 status;
    GetParameterEx5(&param, &result, &status);
    FAIL_RETURN(CheckStatus(status));
    mNumBands = result[0];
    *numBands = mNumBands;
    return NOERROR;
}

ECode CEqualizer::GetBandLevelRange(
    /* [out, callee] */ ArrayOf<Int16>** bandLevelRange)
{
    VALIDATE_NOT_NULL(bandLevelRange);

    ArrayOf_<Int16,2> result;
    Int32 status;
    GetParameterEx3(IEqualizer::PARAM_LEVEL_RANGE, &result, &status);
    FAIL_RETURN(CheckStatus(status));
    *bandLevelRange = &result;
    return NOERROR;
}

ECode CEqualizer::SetBandLevel(
    /* [in] */ Int16 band,
    /* [in] */ Int16 level)
{
    ArrayOf_<Int32,2> param;
    ArrayOf_<Int16,1> value;

    param[0] = IEqualizer::PARAM_BAND_LEVEL;
    param[1] = (Int32) band;
    value[0] = level;
    Int32 status;
    GetParameterEx5(&param, &value, &status);
    FAIL_RETURN(CheckStatus(status));
    return NOERROR;
}

ECode CEqualizer::GetBandLevel(
    /* [in] */ Int16 band,
    /* [out] */ Int16* bandLevel)
{
    VALIDATE_NOT_NULL(bandLevel);

    ArrayOf_<Int32,2> param;
    ArrayOf_<Int16,1> result;

    param[0] = IEqualizer::PARAM_BAND_LEVEL;
    param[1] = (Int32) band;
    Int32 status;
    GetParameterEx5(&param, &result, &status);
    FAIL_RETURN(CheckStatus(status));
    *bandLevel = result[0];
    return NOERROR;
}

ECode CEqualizer::GetCenterFreq(
    /* [in] */ Int16 band,
    /* [out] */ Int32* centerFreq)
{
    VALIDATE_NOT_NULL(centerFreq);

    ArrayOf_<Int32,2> param;
    ArrayOf_<Int32,1> result;

    param[0] = IEqualizer::PARAM_CENTER_FREQ;
    param[1] = (Int32) band;
    Int32 status;
    GetParameterEx4(&param, &result, &status);
    FAIL_RETURN(CheckStatus(status));
    *centerFreq = result[0];
    return NOERROR;
}

ECode CEqualizer::GetBandFreqRange(
    /* [in] */ Int16 band,
    /* [out, callee] */ ArrayOf<Int32>** bandFreqRange)
{
    VALIDATE_NOT_NULL(bandFreqRange);

    ArrayOf_<Int32,2> param;
    ArrayOf_<Int32,2> result;

    param[0] = IEqualizer::PARAM_BAND_FREQ_RANGE;
    param[1] = (Int32) band;
    Int32 status;
    GetParameterEx4(&param, &result, &status);
    FAIL_RETURN(CheckStatus(status));
    *bandFreqRange = &result;
    return NOERROR;
}

ECode CEqualizer::GetBand(
    /* [in] */ Int32 frequency,
    /* [out] */ Int16* band)
{
    VALIDATE_NOT_NULL(band);

    ArrayOf_<Int32,2> param;
    ArrayOf_<Int16,1> result;

    param[0] = IEqualizer::PARAM_GET_BAND;
    param[1] = frequency;
    Int32 status;
    GetParameterEx5(&param, &result, &status);
    FAIL_RETURN(CheckStatus(status));
    *band = result[0];
    return NOERROR;
}

ECode CEqualizer::GetCurrentPreset(
        /* [out] */ Int16* preset)
{
    VALIDATE_NOT_NULL(preset);

    ArrayOf_<Int16,1> result;
    Int32 status;
    GetParameterEx3(IEqualizer::PARAM_CURRENT_PRESET, &result, &status);
    FAIL_RETURN(CheckStatus(status));
    *preset = result[0];
    return NOERROR;
}

ECode CEqualizer::UsePreset(
    /* [in] */ Int16 preset)
{
    Int32 status;
    SetParameterEx2(IEqualizer::PARAM_CURRENT_PRESET, preset, &status);
    return CheckStatus(status);
}

ECode CEqualizer::GetNumberOfPresets(
    /* [out] */ Int16* numPresets)
{
    VALIDATE_NOT_NULL(numPresets);

    ArrayOf_<Int16,1> result;
    Int32 status;
    GetParameterEx3(IEqualizer::PARAM_GET_NUM_OF_PRESETS, &result, &status);
    FAIL_RETURN(CheckStatus(status));
    *numPresets = result[0];
    return NOERROR;
}

ECode CEqualizer::GetPresetName(
    /* [in] */ Int16 preset,
    /* [out] */ String* presetName)
{
    VALIDATE_NOT_NULL(presetName);

    if (preset >= 0 && preset < mNumPresets) {
        *presetName = (*mPresetNames)[preset];
    } else {
        *presetName = "";
    }
    return NOERROR;
}

ECode CEqualizer::SetParameterListenerEx(
    /* [in] */ IEqualizerOnParameterChangeListener* listener)
{
    Mutex::Autolock lock(mParamListenerLock);
    if (mParamListener != NULL) {
        mParamListener = listener;
        mBaseParamListener = new BaseParameterListener(this);
        SetParameterListener(mBaseParamListener);
    }
    return NOERROR;
}

ECode CEqualizer::GetProperties(
    /* [out] */ IEqualizerSettings** properties)
{
    VALIDATE_NOT_NULL(properties);

    AutoPtr<ArrayOf<Byte> > param = ArrayOf<Byte>::Alloc(4 + mNumBands * 2);
    Int32 status;
    GetParameterEx(PARAM_PROPERTIES,param,&status);
    FAIL_RETURN(CheckStatus(status));
    AutoPtr<IEqualizerSettings> settings;
    CEqualizerSettings::New((IEqualizerSettings**)&settings);
    Int16 statusInt16;
    ByteArrayToInt16Ex(param, 0, &statusInt16);
    settings->SetCurPreset(statusInt16);
    ByteArrayToInt16Ex(param, 2, &statusInt16);
    settings->SetNumBands(statusInt16);
    AutoPtr<ArrayOf<Int16> > tempInt16Array1 = ArrayOf<Int16>::Alloc(mNumBands);
    for (int i = 0; i < mNumBands; i++) {
        ByteArrayToInt16Ex(param, 4 + 2*i, &statusInt16);
        (*tempInt16Array1)[i] = statusInt16;
    }
    settings->SetBandLevels(tempInt16Array1);
    *properties = settings;
    INTERFACE_ADDREF(*properties);
    return NOERROR;
}

ECode CEqualizer::SetProperties(
    /* [in] */ IEqualizerSettings* settings)
{
    Int16 tempInt16Parameter1;
    AutoPtr< ArrayOf<Byte> > tempByteArray1;
    AutoPtr< ArrayOf<Byte> > tempByteArray2;
    AutoPtr< ArrayOf<Byte> > param;
    AutoPtr< ArrayOf<Int16> > tempInt16Array1;

    settings->GetNumBands(&tempInt16Parameter1);
    settings->GetBandLevels((ArrayOf<Int16>**)&tempInt16Array1);
    if (tempInt16Parameter1 != tempInt16Array1->GetLength() ||
        tempInt16Parameter1 != mNumBands) {
           // throw new IllegalArgumentException("settings invalid band count: " +settings.numBands);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    settings->GetCurPreset(&tempInt16Parameter1);
    Int16ToByteArray(tempInt16Parameter1, (ArrayOf<Byte>**)&tempByteArray1);
    Int16ToByteArray(mNumBands, (ArrayOf<Byte>**)&tempByteArray2);
    ConcatArrays(tempByteArray1, tempByteArray2, (ArrayOf<Byte>**)&param);

    for (int i = 0; i < mNumBands; i++) {
        Int16ToByteArray((*tempInt16Array1)[i], (ArrayOf<Byte>**)&tempByteArray1);
        ConcatArrays(param, tempByteArray1, (ArrayOf<Byte>**)&param);
    }
    Int32 status;
    SetParameterEx3(PARAM_PROPERTIES, param, &status);
    return CheckStatus(status);
}


} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos
