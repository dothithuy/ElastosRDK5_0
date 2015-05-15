#ifndef __CPRESETREVERB_H__
#define __CPRESETREVERB_H__

#include "_CPresetReverb.h"
#include "ext/frameworkext.h"
#include <elastos/Mutex.h>
#include "media/media/audiofx/AudioEffect.h"
#include "media/media/audiofx/AudioEffectMacro.h"

using Elastos::Core::Threading::Mutex;
using Elastos::Droid::Media::Audiofx::AudioEffect;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

/**
 * A sound generated within a room travels in many directions. The listener first hears the
 * direct sound from the source itself. Later, he or she hears discrete echoes caused by sound
 * bouncing off nearby walls, the ceiling and the floor. As sound waves arrive after
 * undergoing more and more reflections, individual reflections become indistinguishable and
 * the listener hears continuous reverberation that decays over time.
 * Reverb is vital for modeling a listener's environment. It can be used in music applications
 * to simulate music being played back in various environments, or in games to immerse the
 * listener within the game's environment.
 * The PresetReverb class allows an application to configure the global reverb using a reverb preset.
 * This is primarily used for adding some reverb in a music playback context. Applications
 * requiring control over a more advanced environmental reverb are advised to use the
 * {@link android.media.audiofx.EnvironmentalReverb} class.
 * <p>An application creates a PresetReverb object to instantiate and control a reverb engine in the
 * audio framework.
 * <p>The methods, parameter types and units exposed by the PresetReverb implementation are
 * directly mapping those defined by the OpenSL ES 1.0.1 Specification
 * (http://www.khronos.org/opensles/) for the SLPresetReverbItf interface.
 * Please refer to this specification for more details.
 * <p>The PresetReverb is an output mix auxiliary effect and should be created on
 * Audio session 0. In order for a MediaPlayer or AudioTrack to be fed into this effect,
 * they must be explicitely attached to it and a send level must be specified. Use the effect ID
 * returned by getId() method to designate this particular effect when attaching it to the
 * MediaPlayer or AudioTrack.
 * <p>Creating a reverb on the output mix (audio session 0) requires permission
 * {@link android.Manifest.permission#MODIFY_AUDIO_SETTINGS}
 * <p>See {@link android.media.audiofx.AudioEffect} class for more details on controlling
 * audio effects.
 */

CarClass(CPresetReverb), public AudioEffect
{

private:
    /**
     * Listener used internally to receive unformatted parameter change events from AudioEffect
     * super class.
     */
    class BaseParameterListener
        : public ElRefBase
        , public IAudioEffectOnParameterChangeListener
    {
    public:
        BaseParameterListener(
           /* [in] */  CPresetReverb* host) : mHost(host)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnParameterChange(
            /* [in] */ IAudioEffect* effect,
            /* [in] */ Int32 status,
            /* [in] */ ArrayOf<Byte>* param,
            /* [in] */ ArrayOf<Byte>* value);

    private:
        CPresetReverb* mHost;
    };

public:
    IAUDIOEFFECT_METHODS_DECL()

    CPresetReverb();

    /**
     * Class constructor.
     * @param priority the priority level requested by the application for controlling the
     * PresetReverb engine. As the same engine can be shared by several applications, this
     * parameter indicates how much the requesting application needs control of effect parameters.
     * The normal priority is 0, above normal is a positive number, below normal a negative number.
     * @param audioSession  system wide unique audio session identifier. If audioSession
     *  is not 0, the PresetReverb will be attached to the MediaPlayer or AudioTrack in the
     *  same audio session. Otherwise, the PresetReverb will apply to the output mix.
     *  As the PresetReverb is an auxiliary effect it is recommended to instantiate it on
     *  audio session 0 and to attach it to the MediaPLayer auxiliary output.
     *
     * @throws java.lang.IllegalArgumentException
     * @throws java.lang.UnsupportedOperationException
     * @throws java.lang.RuntimeException
     */
    CARAPI constructor(
        /* [in] */ Int32 priority,
        /* [in] */ Int32 audioSession);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     *  Enables a preset on the reverb.
     *  <p>The reverb PRESET_NONE disables any reverb from the current output but does not free the
     *  resources associated with the reverb. For an application to signal to the implementation
     *  to free the resources, it must call the release() method.
     * @param preset this must be one of the the preset constants defined in this class.
     * e.g. {@link #PRESET_SMALLROOM}
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetPreset(
        /* [in] */ Int16 preset);

    /**
     * Gets current reverb preset.
     * @return the preset that is set at the moment.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetPreset(
        /* [out] */ Int16* preset);

    /**
     * Registers an OnParameterChangeListener interface.
     * @param listener OnParameterChangeListener interface registered
     */
    CARAPI SetParameterListenerEx(
        /* [in] */ IPresetReverbOnParameterChangeListener* listener);

    /**
     * Gets the preset reverb properties. This method is useful when a snapshot of current
     * preset reverb settings must be saved by the application.
     * @return a PresetReverb.Settings object containing all current parameters values
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetProperties(
        /* [out] */ IPresetReverbSettings** properties);

    /**
     * Sets the preset reverb properties. This method is useful when preset reverb settings have to
     * be applied from a previous backup.
     * @param settings a PresetReverb.Settings object containing the properties to apply
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetProperties(
        /* [in] */ IPresetReverbSettings* settings);

private:
    static const String TAG;

    /**
     * Registered listener for parameter changes.
     */
    AutoPtr<IPresetReverbOnParameterChangeListener> mParamListener;

    /**
     * Listener used internally to to receive raw parameter change event from AudioEffect super class
     */
    AutoPtr<BaseParameterListener> mBaseParamListener;

    /**
     * Lock for access to mParamListener
     */
    Mutex mParamListenerLock;
};

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif //__CPRESETREVERB_H__