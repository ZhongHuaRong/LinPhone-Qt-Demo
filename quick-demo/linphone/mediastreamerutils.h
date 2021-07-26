#ifndef MEDIASTREAMERUTILS_H
#define MEDIASTREAMERUTILS_H

#include <cmath>

#include "mediastreamer2/mssndcard.h"
#include "mediastreamer2/msvolume.h"
#include "mediastreamer2/msfilter.h"
#include "mediastreamer2/msticker.h"
#include <linphone++/linphone.hh>

#include <QtGlobal>

// =============================================================================

namespace MediastreamerUtils {

    inline float computeVu (float volume) {
        constexpr float VuMin = -20.f;
        constexpr float VuMax = 4.f;

        if (volume < VuMin)
            return 0.f;
        if (volume > VuMax)
            return 1.f;

        return (volume - VuMin) / (VuMax - VuMin);
    }

    inline float dbToLinear(float volume) {
        return static_cast<float>(pow(10.0, volume / 10.0));
    }

    inline float linearToDb(float volume) {
        if (qFuzzyIsNull(volume)) {
            return MS_VOLUME_DB_LOWEST;
        }
        return static_cast<float>(10.0 * log10(volume));
    }

    //Simple mediastreamer audio capture graph
    //Used to get current microphone volume in audio settings
    class SimpleCaptureGraph {
    public:
        SimpleCaptureGraph(const std::string &captureCardId, const std::string &playbackCardId);
        ~SimpleCaptureGraph();

        void start();
        void stop();

        float getCaptureVolume();

        float getCaptureGain();
        float getPlaybackGain();
        void setCaptureGain(float volume);
        void setPlaybackGain(float volume);

        bool isRunning() const {
            return running;
        }
    protected:
        void init();
        void destroy();

        bool running = false;

        std::string captureCardId;
        std::string playbackCardId;

        MSFilter *audioSink = nullptr;
        MSFilter *audioCapture = nullptr;
        MSFilter *captureVolumeFilter = nullptr;
        MSFilter *playbackVolumeFilter = nullptr;
        MSTicker *ticker = nullptr;
        MSSndCard *playbackCard = nullptr;
        MSSndCard *captureCard = nullptr;
        MSFactory *msFactory = nullptr;
    };

}

#endif // MEDIASTREAMERUTILS_H
