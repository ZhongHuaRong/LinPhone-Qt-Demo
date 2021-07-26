#include "linphonesettings.h"
#include "linphonecoremanager.h"
#include <QDir>
#include <QtDebug>
#include <QPluginLoader>
#include <QJsonDocument>
#include <cstdlib>
#include <cmath>

// =============================================================================

using namespace std;

namespace {
    constexpr char DefaultRlsUri[] = "sips:rls@sip.linphone.org";
    constexpr char DefaultLogsEmail[] = "linphone-desktop@belledonne-communications.com";
}

const string LinphoneSettings::UiSection("ui");
const string LinphoneSettings::ContactsSection("contacts_import");

LinphoneSettings::LinphoneSettings (QObject *parent) : QObject(parent) {
    LinphoneCoreManager *coreManager = LinphoneCoreManager::getInstance();
    mConfig = coreManager->getCore()->getConfig();

    QObject::connect(coreManager->getHandlers().get(), &CoreHandlers::callCreated,
             this, &LinphoneSettings::handleCallCreated);
    QObject::connect(coreManager->getHandlers().get(), &CoreHandlers::callStateChanged,
             this, &LinphoneSettings::handleCallStateChanged);
    QObject::connect(coreManager->getHandlers().get(), &CoreHandlers::ecCalibrationResult,
             this, &LinphoneSettings::handleEcCalibrationResult);

    configureRlsUri();
}
LinphoneSettings::~LinphoneSettings()
{
    if(mSimpleCaptureGraph )
    {
        delete mSimpleCaptureGraph;
        mSimpleCaptureGraph = nullptr;
    }
}

// =============================================================================
// Assistant.
// =============================================================================

bool LinphoneSettings::getUseAppSipAccountEnabled () const {
    return !!mConfig->getInt(UiSection, "use_app_sip_account_enabled", 1);
}

void LinphoneSettings::setUseAppSipAccountEnabled (bool status) {
    mConfig->setInt(UiSection, "use_app_sip_account_enabled", status);
    emit useAppSipAccountEnabledChanged(status);
}

bool LinphoneSettings::getUseOtherSipAccountEnabled () const {
    return !!mConfig->getInt(UiSection, "use_other_sip_account_enabled", 1);
}

void LinphoneSettings::setUseOtherSipAccountEnabled (bool status) {
    mConfig->setInt(UiSection, "use_other_sip_account_enabled", status);
    emit useOtherSipAccountEnabledChanged(status);
}

bool LinphoneSettings::getCreateAppSipAccountEnabled () const {
    return !!mConfig->getInt(UiSection, "create_app_sip_account_enabled", 1);
}

void LinphoneSettings::setCreateAppSipAccountEnabled (bool status) {
    mConfig->setInt(UiSection, "create_app_sip_account_enabled", status);
    emit createAppSipAccountEnabledChanged(status);
}

bool LinphoneSettings::getFetchRemoteConfigurationEnabled () const {
    return !!mConfig->getInt(UiSection, "fetch_remote_configuration_enabled", 1);
}

void LinphoneSettings::setFetchRemoteConfigurationEnabled (bool status) {
    mConfig->setInt(UiSection, "fetch_remote_configuration_enabled", status);
    emit fetchRemoteConfigurationEnabledChanged(status);
}

// ---------------------------------------------------------------------------

bool LinphoneSettings::getAssistantSupportsPhoneNumbers () const {
    return !!mConfig->getInt(UiSection, "assistant_supports_phone_numbers", 1);
}

void LinphoneSettings::setAssistantSupportsPhoneNumbers (bool status) {
    mConfig->setInt(UiSection, "assistant_supports_phone_numbers", status);
    emit assistantSupportsPhoneNumbersChanged(status);
}

// =============================================================================
// Audio.
// =============================================================================

void LinphoneSettings::createCaptureGraph() {
    if (mSimpleCaptureGraph)  {
        delete mSimpleCaptureGraph;
        mSimpleCaptureGraph = nullptr;
    }
    if (!mSimpleCaptureGraph) {
        mSimpleCaptureGraph =
            new MediastreamerUtils::SimpleCaptureGraph(Utils::appStringToCoreString(getCaptureDevice()), Utils::appStringToCoreString(getPlaybackDevice()));
    }
    mSimpleCaptureGraph->start();
    emit captureGraphRunningChanged(getCaptureGraphRunning());
}

//Force a call on the 'detect' method of all audio filters, updating new or removed devices
void LinphoneSettings::accessAudioSettings() {
    LinphoneCoreManager::getInstance()->getCore()->reloadSoundDevices();
    emit captureDevicesChanged(getCaptureDevices());
    emit playbackDevicesChanged(getPlaybackDevices());
    emit playbackDeviceChanged(getPlaybackDevice());
    emit captureDeviceChanged(getCaptureDevice());
    emit ringerDeviceChanged(getRingerDevice());

    if (!getIsInCall()) {
        createCaptureGraph();
    }
}

void LinphoneSettings::closeAudioSettings() {
    if (mSimpleCaptureGraph) {
        if (mSimpleCaptureGraph->isRunning()) {
            mSimpleCaptureGraph->stop();
        }
        delete mSimpleCaptureGraph;
        mSimpleCaptureGraph = nullptr;
    }
    emit captureGraphRunningChanged(getCaptureGraphRunning());
}

bool LinphoneSettings::getCaptureGraphRunning() {
    return mSimpleCaptureGraph && mSimpleCaptureGraph->isRunning() && !getIsInCall();
}

float LinphoneSettings::getMicVolume() {
    float v = 0.0;

    if (mSimpleCaptureGraph && mSimpleCaptureGraph->isRunning()) {
        v = mSimpleCaptureGraph->getCaptureVolume();
    }
    return v;
}

float LinphoneSettings::getPlaybackGain() const {
    float dbGain = LinphoneCoreManager::getInstance()->getCore()->getPlaybackGainDb();
    return MediastreamerUtils::dbToLinear(dbGain);
}

void LinphoneSettings::setPlaybackGain(float gain) {
    if (mSimpleCaptureGraph && mSimpleCaptureGraph->isRunning()) {
        mSimpleCaptureGraph->setPlaybackGain(gain);
    }
}

float LinphoneSettings::getCaptureGain() const {
    float dbGain = LinphoneCoreManager::getInstance()->getCore()->getMicGainDb();
    return MediastreamerUtils::dbToLinear(dbGain);
}

void LinphoneSettings::setCaptureGain(float gain) {
    if (mSimpleCaptureGraph && mSimpleCaptureGraph->isRunning()) {
        mSimpleCaptureGraph->setCaptureGain(gain);
    }
}

QStringList LinphoneSettings::getCaptureDevices () const {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    QStringList list;

    for (const auto &device : core->getSoundDevicesList()) {
        if (core->soundDeviceCanCapture(device))
            list << Utils::coreStringToAppString(device);
    }
    return list;
}

QStringList LinphoneSettings::getPlaybackDevices () const {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    QStringList list;

    for (const auto &device : core->getSoundDevicesList()) {
        if (core->soundDeviceCanPlayback(device)) {
            list << Utils::coreStringToAppString(device);
        }
    }

    return list;
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getCaptureDevice () const {
    auto audioDevice = LinphoneCoreManager::getInstance()->getCore()->getInputAudioDevice();
    return Utils::coreStringToAppString(audioDevice? audioDevice->getId() : LinphoneCoreManager::getInstance()->getCore()->getCaptureDevice());
}

void LinphoneSettings::setCaptureDevice (const QString &device) {
    std::string devId = Utils::appStringToCoreString(device);
    auto list = LinphoneCoreManager::getInstance()->getCore()->getExtendedAudioDevices();
    auto audioDevice = find_if(list.cbegin(), list.cend(), [&] ( const std::shared_ptr<linphone::AudioDevice> & audioItem) {
       return audioItem->getId() == devId;
    });
    if(audioDevice != list.cend()){
        LinphoneCoreManager::getInstance()->getCore()->setCaptureDevice(devId);
        LinphoneCoreManager::getInstance()->getCore()->setInputAudioDevice(*audioDevice);
        emit captureDeviceChanged(device);
        if (mSimpleCaptureGraph && mSimpleCaptureGraph->isRunning()) {
            createCaptureGraph();
        }
    }else
        qWarning() << "Cannot set Capture device. The ID cannot be matched with an existant device : " << device;
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getPlaybackDevice () const {
    auto audioDevice = LinphoneCoreManager::getInstance()->getCore()->getOutputAudioDevice();
    return Utils::coreStringToAppString(audioDevice? audioDevice->getId() : LinphoneCoreManager::getInstance()->getCore()->getPlaybackDevice());
}

void LinphoneSettings::setPlaybackDevice (const QString &device) {
    std::string devId = Utils::appStringToCoreString(device);

    auto list = LinphoneCoreManager::getInstance()->getCore()->getExtendedAudioDevices();
    auto audioDevice = find_if(list.cbegin(), list.cend(), [&] ( const std::shared_ptr<linphone::AudioDevice> & audioItem) {
       return audioItem->getId() == devId;
    });
    if(audioDevice != list.cend()){

        LinphoneCoreManager::getInstance()->getCore()->setPlaybackDevice(devId);
        LinphoneCoreManager::getInstance()->getCore()->setOutputAudioDevice(*audioDevice);
        emit playbackDeviceChanged(device);
        if (mSimpleCaptureGraph && mSimpleCaptureGraph->isRunning()) {
            createCaptureGraph();
        }
    }else
        qWarning() << "Cannot set Playback device. The ID cannot be matched with an existant device : " << device;
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getRingerDevice () const {
    return Utils::coreStringToAppString(
                        LinphoneCoreManager::getInstance()->getCore()->getRingerDevice()
                        );
}

void LinphoneSettings::setRingerDevice (const QString &device) {
    LinphoneCoreManager::getInstance()->getCore()->setRingerDevice(
                                   Utils::appStringToCoreString(device)
                                   );
    emit ringerDeviceChanged(device);
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getRingPath () const {
    return Utils::coreStringToAppString(LinphoneCoreManager::getInstance()->getCore()->getRing());
}

void LinphoneSettings::setRingPath (const QString &path) {
    QString cleanedPath = QDir::cleanPath(path);

    LinphoneCoreManager::getInstance()->getCore()->setRing(
                               Utils::appStringToCoreString(cleanedPath)
                               );

    emit ringPathChanged(cleanedPath);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getEchoCancellationEnabled () const {
    return LinphoneCoreManager::getInstance()->getCore()->echoCancellationEnabled();
}

void LinphoneSettings::setEchoCancellationEnabled (bool status) {
    LinphoneCoreManager::getInstance()->getCore()->enableEchoCancellation(status);
    emit echoCancellationEnabledChanged(status);
}

void LinphoneSettings::startEchoCancellerCalibration(){
    LinphoneCoreManager::getInstance()->getCore()->startEchoCancellerCalibration();

}
// -----------------------------------------------------------------------------

bool LinphoneSettings::getShowAudioCodecs () const {
    return !!mConfig->getInt(UiSection, "show_audio_codecs", 1);
}

void LinphoneSettings::setShowAudioCodecs (bool status) {
    mConfig->setInt(UiSection, "show_audio_codecs", status);
    emit showAudioCodecsChanged(status);
}


// =============================================================================
// Video.
// =============================================================================

//Force a call on the 'detect' method of all video filters, updating new or removed devices
void LinphoneSettings::accessVideoSettings() {
    if(!getIsInCall())// TODO : This is a workaround to a crash when reloading video devices while in call. Spotted on Macos.
        LinphoneCoreManager::getInstance()->getCore()->reloadVideoDevices();
    emit videoDevicesChanged(getVideoDevices());
}

QStringList LinphoneSettings::getVideoDevices () const {
    QStringList list;

    for (const auto &device : LinphoneCoreManager::getInstance()->getCore()->getVideoDevicesList())
        list << Utils::coreStringToAppString(device);

    return list;
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getVideoDevice () const {
    return Utils::coreStringToAppString(
                        LinphoneCoreManager::getInstance()->getCore()->getVideoDevice()
                        );
}

void LinphoneSettings::setVideoDevice (const QString &device) {
    LinphoneCoreManager::getInstance()->getCore()->setVideoDevice(
                                  Utils::appStringToCoreString(device)
                                  );
    emit videoDeviceChanged(device);
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getVideoPreset () const {
    return Utils::coreStringToAppString(
                        LinphoneCoreManager::getInstance()->getCore()->getVideoPreset()
                        );
}

void LinphoneSettings::setVideoPreset (const QString &preset) {
    LinphoneCoreManager::getInstance()->getCore()->setVideoPreset(
                                  Utils::appStringToCoreString(preset)
                                  );
    emit videoPresetChanged(preset);
}

// -----------------------------------------------------------------------------

int LinphoneSettings::getVideoFramerate () const {
    return int(LinphoneCoreManager::getInstance()->getCore()->getPreferredFramerate());
}

void LinphoneSettings::setVideoFramerate (int framerate) {
    LinphoneCoreManager::getInstance()->getCore()->setPreferredFramerate(float(framerate));
    emit videoFramerateChanged(framerate);
}

// -----------------------------------------------------------------------------

static inline QVariantMap createMapFromVideoDefinition (const shared_ptr<const linphone::VideoDefinition> &definition) {
    QVariantMap map;

    if (!definition) {
        Q_ASSERT(!LinphoneCoreManager::getInstance()->getCore()->videoSupported());

        map["name"] = QStringLiteral("Bad EGG");
        map["width"] = QStringLiteral("?????");
        map["height"] = QStringLiteral("?????");

        return map;
    }

    map["name"] = Utils::coreStringToAppString(definition->getName());
    map["width"] = definition->getWidth();
    map["height"] = definition->getHeight();
    map["__definition"] = QVariant::fromValue(definition);

    return map;
}

QVariantList LinphoneSettings::getSupportedVideoDefinitions () const {
    QVariantList list;
    for (const auto &definition : linphone::Factory::get()->getSupportedVideoDefinitions())
        list << createMapFromVideoDefinition(definition);
    return list;
}

QVariantMap LinphoneSettings::getVideoDefinition () const {
    return createMapFromVideoDefinition(LinphoneCoreManager::getInstance()->getCore()->getPreferredVideoDefinition());
}

void LinphoneSettings::setVideoDefinition (const QVariantMap &definition) {
    LinphoneCoreManager::getInstance()->getCore()->setPreferredVideoDefinition(
                                       definition.value("__definition").value<shared_ptr<const linphone::VideoDefinition>>()->clone()
                                       );

    emit videoDefinitionChanged(definition);
}

bool LinphoneSettings::getVideoSupported () const {
    return LinphoneCoreManager::getInstance()->getCore()->videoSupported();
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getShowVideoCodecs () const {
    return !!mConfig->getInt(UiSection, "show_video_codecs", 1);
}

void LinphoneSettings::setShowVideoCodecs (bool status) {
    mConfig->setInt(UiSection, "show_video_codecs", status);
    emit showVideoCodecsChanged(status);
}

// =============================================================================
// Chat & calls.
// =============================================================================

bool LinphoneSettings::getAutoAnswerStatus () const {
    return !!mConfig->getInt(UiSection, "auto_answer", 0);
}

void LinphoneSettings::setAutoAnswerStatus (bool status) {
    mConfig->setInt(UiSection, "auto_answer", status);
    emit autoAnswerStatusChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getAutoAnswerVideoStatus () const {
    return !!mConfig->getInt(UiSection, "auto_answer_with_video", 0);
}

void LinphoneSettings::setAutoAnswerVideoStatus (bool status) {
    mConfig->setInt(UiSection, "auto_answer_with_video", status);
    emit autoAnswerVideoStatusChanged(status);
}

// -----------------------------------------------------------------------------

int LinphoneSettings::getAutoAnswerDelay () const {
    return mConfig->getInt(UiSection, "auto_answer_delay", 0);
}

void LinphoneSettings::setAutoAnswerDelay (int delay) {
    mConfig->setInt(UiSection, "auto_answer_delay", delay);
    emit autoAnswerDelayChanged(delay);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getShowTelKeypadAutomatically () const {
    return !!mConfig->getInt(UiSection, "show_tel_keypad_automatically", 0);
}

void LinphoneSettings::setShowTelKeypadAutomatically (bool status) {
    mConfig->setInt(UiSection, "show_tel_keypad_automatically", status);
    emit showTelKeypadAutomaticallyChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getKeepCallsWindowInBackground () const {
    return !!mConfig->getInt(UiSection, "keep_calls_window_in_background", 0);
}

void LinphoneSettings::setKeepCallsWindowInBackground (bool status) {
    mConfig->setInt(UiSection, "keep_calls_window_in_background", status);
    emit keepCallsWindowInBackgroundChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getOutgoingCallsEnabled () const {
    return !!mConfig->getInt(UiSection, "outgoing_calls_enabled", 1);
}

void LinphoneSettings::setOutgoingCallsEnabled (bool status) {
    mConfig->setInt(UiSection, "outgoing_calls_enabled", status);
    emit outgoingCallsEnabledChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getCallRecorderEnabled () const {
    return !!mConfig->getInt(UiSection, "call_recorder_enabled", 1);
}

void LinphoneSettings::setCallRecorderEnabled (bool status) {
    mConfig->setInt(UiSection, "call_recorder_enabled", status);
    emit callRecorderEnabledChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getAutomaticallyRecordCalls () const {
    return !!mConfig->getInt(UiSection, "automatically_record_calls", 0);
}

void LinphoneSettings::setAutomaticallyRecordCalls (bool status) {
    mConfig->setInt(UiSection, "automatically_record_calls", status);
    emit automaticallyRecordCallsChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getCallPauseEnabled () const {
    return !!mConfig->getInt(UiSection, "call_pause_enabled", 1);
}

void LinphoneSettings::setCallPauseEnabled (bool status) {
    mConfig->setInt(UiSection, "call_pause_enabled", status);
    emit callPauseEnabledChanged(status);
}

bool LinphoneSettings::getMuteMicrophoneEnabled () const {
    return !!mConfig->getInt(UiSection, "mute_microphone_enabled", 1);
}

void LinphoneSettings::setMuteMicrophoneEnabled (bool status) {
    mConfig->setInt(UiSection, "mute_microphone_enabled", status);
    emit muteMicrophoneEnabledChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getChatEnabled () const {
    return !!mConfig->getInt(UiSection, "chat_enabled", 1);
}

void LinphoneSettings::setChatEnabled (bool status) {
    mConfig->setInt(UiSection, "chat_enabled", status);
    emit chatEnabledChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getConferenceEnabled () const {
    return !!mConfig->getInt(UiSection, "conference_enabled", 1);
}

void LinphoneSettings::setConferenceEnabled (bool status) {
    mConfig->setInt(UiSection, "conference_enabled", status);
    emit conferenceEnabledChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getChatNotificationSoundEnabled () const {
    return !!mConfig->getInt(UiSection, "chat_sound_notification_enabled", 1);
}

void LinphoneSettings::setChatNotificationSoundEnabled (bool status) {
    mConfig->setInt(UiSection, "chat_sound_notification_enabled", status);
    emit chatNotificationSoundEnabledChanged(status);
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getChatNotificationSoundPath () const {
    static const string defaultFile = linphone::Factory::get()->getSoundResourcesDir() + "/incoming_chat.wav";
    return Utils::coreStringToAppString(mConfig->getString(UiSection, "chat_sound_notification_file", defaultFile));
}

void LinphoneSettings::setChatNotificationSoundPath (const QString &path) {
    QString cleanedPath = QDir::cleanPath(path);
    mConfig->setString(UiSection, "chat_sound_notification_file", Utils::appStringToCoreString(cleanedPath));
    emit chatNotificationSoundPathChanged(cleanedPath);
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getFileTransferUrl () const {
    return Utils::coreStringToAppString(
                        LinphoneCoreManager::getInstance()->getCore()->getFileTransferServer()
                        );
}

void LinphoneSettings::setFileTransferUrl (const QString &url) {
    LinphoneCoreManager::getInstance()->getCore()->setFileTransferServer(
                                     Utils::appStringToCoreString(url)
                                     );
    emit fileTransferUrlChanged(url);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getLimeIsSupported () const {
    return LinphoneCoreManager::getInstance()->getCore()->limeX3DhAvailable();
}

// -----------------------------------------------------------------------------

static inline QVariant buildEncryptionDescription (LinphoneSettings::MediaEncryption encryption, const char *description) {
    return QVariantList() << encryption << description;
}

QVariantList LinphoneSettings::getSupportedMediaEncryptions () const {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    QVariantList list;

    if (core->mediaEncryptionSupported(linphone::MediaEncryption::SRTP))
        list << buildEncryptionDescription(MediaEncryptionSrtp, "SRTP");

    if (core->mediaEncryptionSupported(linphone::MediaEncryption::ZRTP))
        list << buildEncryptionDescription(MediaEncryptionZrtp, "ZRTP");

    if (core->mediaEncryptionSupported(linphone::MediaEncryption::DTLS))
        list << buildEncryptionDescription(MediaEncryptionDtls, "DTLS");

    return list;
}

// -----------------------------------------------------------------------------

LinphoneSettings::MediaEncryption LinphoneSettings::getMediaEncryption () const {
    return static_cast<LinphoneSettings::MediaEncryption>(
                               LinphoneCoreManager::getInstance()->getCore()->getMediaEncryption()
                               );
}

void LinphoneSettings::setMediaEncryption (MediaEncryption encryption) {
    if (encryption == getMediaEncryption())
        return;

    if (encryption != LinphoneSettings::MediaEncryptionZrtp)
        setLimeState(false);

    LinphoneCoreManager::getInstance()->getCore()->setMediaEncryption(
                                  static_cast<linphone::MediaEncryption>(encryption)
                                  );
    if (mandatoryMediaEncryptionEnabled() && encryption == LinphoneSettings::MediaEncryptionNone) {
        //Disable mandatory encryption if none is selected
        enableMandatoryMediaEncryption(false);
    }

    emit mediaEncryptionChanged(encryption);
}

bool LinphoneSettings::mandatoryMediaEncryptionEnabled () const {
    return LinphoneCoreManager::getInstance()->getCore()->isMediaEncryptionMandatory();
}

void LinphoneSettings::enableMandatoryMediaEncryption(bool mandatory) {
    if (mandatoryMediaEncryptionEnabled() == mandatory) {
        return;
    }
    LinphoneCoreManager::getInstance()->getCore()->setMediaEncryptionMandatory(mandatory);
    if (mandatory && getMediaEncryption() == LinphoneSettings::MediaEncryptionNone) {
        //Force	to SRTP if mandatory but 'none' was selected
        setMediaEncryption(LinphoneSettings::MediaEncryptionSrtp);
    } else {
        emit mediaEncryptionChanged(getMediaEncryption());
    }
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getLimeState () const {
    return  LinphoneCoreManager::getInstance()->getCore()->limeX3DhEnabled();
}

void LinphoneSettings::setLimeState (const bool& state) {
    if (state == getLimeState())
        return;

    if (state)
        setMediaEncryption(LinphoneSettings::MediaEncryptionZrtp);

    LinphoneCoreManager::getInstance()->getCore()->enableLimeX3Dh(!state);

    emit limeStateChanged(state);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getContactsEnabled () const {
    return !!mConfig->getInt(UiSection, "contacts_enabled", 1);
}

void LinphoneSettings::setContactsEnabled (bool status) {
    mConfig->setInt(UiSection, "contacts_enabled", status);
    emit contactsEnabledChanged(status);
}

// =============================================================================
// Network.
// =============================================================================

bool LinphoneSettings::getShowNetworkSettings () const {
    return !!mConfig->getInt(UiSection, "show_network_settings", 1);
}

void LinphoneSettings::setShowNetworkSettings (bool status) {
    mConfig->setInt(UiSection, "show_network_settings", status);
    emit showNetworkSettingsChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getUseSipInfoForDtmfs () const {
    return LinphoneCoreManager::getInstance()->getCore()->getUseInfoForDtmf();
}

void LinphoneSettings::setUseSipInfoForDtmfs (bool status) {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();

    if (status) {
        core->setUseRfc2833ForDtmf(false);
        core->setUseInfoForDtmf(true);
    } else {
        core->setUseInfoForDtmf(false);
        core->setUseRfc2833ForDtmf(true);
    }

    emit dtmfsProtocolChanged();
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getUseRfc2833ForDtmfs () const {
    return LinphoneCoreManager::getInstance()->getCore()->getUseRfc2833ForDtmf();
}

void LinphoneSettings::setUseRfc2833ForDtmfs (bool status) {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();

    if (status) {
        core->setUseInfoForDtmf(false);
        core->setUseRfc2833ForDtmf(true);
    } else {
        core->setUseRfc2833ForDtmf(false);
        core->setUseInfoForDtmf(true);
    }

    emit dtmfsProtocolChanged();
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getIpv6Enabled () const {
    return LinphoneCoreManager::getInstance()->getCore()->ipv6Enabled();
}

void LinphoneSettings::setIpv6Enabled (bool status) {
    LinphoneCoreManager::getInstance()->getCore()->enableIpv6(status);
    emit ipv6EnabledChanged(status);
}

// -----------------------------------------------------------------------------

int LinphoneSettings::getDownloadBandwidth () const {
    return LinphoneCoreManager::getInstance()->getCore()->getDownloadBandwidth();
}

void LinphoneSettings::setDownloadBandwidth (int bandwidth) {
    LinphoneCoreManager::getInstance()->getCore()->setDownloadBandwidth(bandwidth);
    emit downloadBandWidthChanged(getDownloadBandwidth());
}

// -----------------------------------------------------------------------------

int LinphoneSettings::getUploadBandwidth () const {
    return LinphoneCoreManager::getInstance()->getCore()->getUploadBandwidth();
}

void LinphoneSettings::setUploadBandwidth (int bandwidth) {
    LinphoneCoreManager::getInstance()->getCore()->setUploadBandwidth(bandwidth);
    emit uploadBandWidthChanged(getUploadBandwidth());
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getAdaptiveRateControlEnabled () const {
    return LinphoneCoreManager::getInstance()->getCore()->adaptiveRateControlEnabled();
}

void LinphoneSettings::setAdaptiveRateControlEnabled (bool status) {
    LinphoneCoreManager::getInstance()->getCore()->enableAdaptiveRateControl(status);
    emit adaptiveRateControlEnabledChanged(status);
}

// -----------------------------------------------------------------------------

int LinphoneSettings::getTcpPort () const {
    return LinphoneCoreManager::getInstance()->getCore()->getTransports()->getTcpPort();
}

void LinphoneSettings::setTcpPort (int port) {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    shared_ptr<linphone::Transports> transports = core->getTransports();

    transports->setTcpPort(port);
    core->setTransports(transports);

    emit tcpPortChanged(port);
}

// -----------------------------------------------------------------------------

int LinphoneSettings::getUdpPort () const {
    return LinphoneCoreManager::getInstance()->getCore()->getTransports()->getUdpPort();
}

void LinphoneSettings::setUdpPort (int port) {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    shared_ptr<linphone::Transports> transports = core->getTransports();

    transports->setUdpPort(port);
    core->setTransports(transports);

    emit udpPortChanged(port);
}

// -----------------------------------------------------------------------------

QList<int> LinphoneSettings::getAudioPortRange () const {
    shared_ptr<linphone::Range> range = LinphoneCoreManager::getInstance()->getCore()->getAudioPortsRange();
    return QList<int>() << range->getMin() << range->getMax();
}

void LinphoneSettings::setAudioPortRange (const QList<int> &range) {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    int a = range[0];
    int b = range[1];

    if (b == -1)
        core->setAudioPort(a);
    else
        core->setAudioPortRange(a, b);

    emit audioPortRangeChanged(a, b);
}

// -----------------------------------------------------------------------------

QList<int> LinphoneSettings::getVideoPortRange () const {
    shared_ptr<linphone::Range> range = LinphoneCoreManager::getInstance()->getCore()->getVideoPortsRange();
    return QList<int>() << range->getMin() << range->getMax();
}

void LinphoneSettings::setVideoPortRange (const QList<int> &range) {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    int a = range[0];
    int b = range[1];

    if (b == -1)
        core->setVideoPort(a);
    else
        core->setVideoPortRange(a, b);

    emit videoPortRangeChanged(a, b);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getIceEnabled () const {
    return LinphoneCoreManager::getInstance()->getCore()->getNatPolicy()->iceEnabled();
}

void LinphoneSettings::setIceEnabled (bool status) {
    shared_ptr<linphone::NatPolicy> natPolicy = LinphoneCoreManager::getInstance()->getCore()->getNatPolicy();

    natPolicy->enableIce(status);
    natPolicy->enableStun(status);

    emit iceEnabledChanged(status);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getTurnEnabled () const {
    return LinphoneCoreManager::getInstance()->getCore()->getNatPolicy()->turnEnabled();
}

void LinphoneSettings::setTurnEnabled (bool status) {
    LinphoneCoreManager::getInstance()->getCore()->getNatPolicy()->enableTurn(status);
    emit turnEnabledChanged(status);
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getStunServer () const {
    return Utils::coreStringToAppString(
                        LinphoneCoreManager::getInstance()->getCore()->getNatPolicy()->getStunServer()
                        );
}

void LinphoneSettings::setStunServer (const QString &stunServer) {
    LinphoneCoreManager::getInstance()->getCore()->getNatPolicy()->setStunServer(
                                         Utils::appStringToCoreString(stunServer)
                                         );
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getTurnUser () const {
    return Utils::coreStringToAppString(
                        LinphoneCoreManager::getInstance()->getCore()->getNatPolicy()->getStunServerUsername()
                        );
}

void LinphoneSettings::setTurnUser (const QString &user) {
    LinphoneCoreManager::getInstance()->getCore()->getNatPolicy()->setStunServerUsername(
                                             Utils::appStringToCoreString(user)
                                             );

    emit turnUserChanged(user);
}

// -----------------------------------------------------------------------------

QString LinphoneSettings::getTurnPassword () const {
    shared_ptr<linphone::Core> core(LinphoneCoreManager::getInstance()->getCore());
    shared_ptr<linphone::NatPolicy> natPolicy(core->getNatPolicy());
    shared_ptr<const linphone::AuthInfo> authInfo(core->findAuthInfo(
                                     "",
                                     natPolicy->getStunServerUsername(),
                                     natPolicy->getStunServer()
                                     ));
    return authInfo ? Utils::coreStringToAppString(authInfo->getPassword()) : QString("");
}

void LinphoneSettings::setTurnPassword (const QString &password) {
    shared_ptr<linphone::Core> core(LinphoneCoreManager::getInstance()->getCore());
    shared_ptr<linphone::NatPolicy> natPolicy(core->getNatPolicy());

    const string &turnUser(natPolicy->getStunServerUsername());
    shared_ptr<const linphone::AuthInfo> authInfo(core->findAuthInfo("", turnUser, natPolicy->getStunServer()));
    if (authInfo) {
        shared_ptr<linphone::AuthInfo> clonedAuthInfo(authInfo->clone());
        clonedAuthInfo->setPassword(Utils::appStringToCoreString(password));

        core->addAuthInfo(clonedAuthInfo);
        core->removeAuthInfo(authInfo);
    } else
        core->addAuthInfo(linphone::Factory::get()->createAuthInfo(
                                       turnUser,
                                       turnUser,
                                       Utils::appStringToCoreString(password),
                                       "",
                                       "",
                                       ""
                                       ));

    emit turnPasswordChanged(password);
}

// -----------------------------------------------------------------------------

int LinphoneSettings::getDscpSip () const {
    return LinphoneCoreManager::getInstance()->getCore()->getSipDscp();
}

void LinphoneSettings::setDscpSip (int dscp) {
    LinphoneCoreManager::getInstance()->getCore()->setSipDscp(dscp);
    emit dscpSipChanged(dscp);
}

int LinphoneSettings::getDscpAudio () const {
    return LinphoneCoreManager::getInstance()->getCore()->getAudioDscp();
}

void LinphoneSettings::setDscpAudio (int dscp) {
    LinphoneCoreManager::getInstance()->getCore()->setAudioDscp(dscp);
    emit dscpAudioChanged(dscp);
}

int LinphoneSettings::getDscpVideo () const {
    return LinphoneCoreManager::getInstance()->getCore()->getVideoDscp();
}

void LinphoneSettings::setDscpVideo (int dscp) {
    LinphoneCoreManager::getInstance()->getCore()->setVideoDscp(dscp);
    emit dscpVideoChanged(dscp);
}

// -----------------------------------------------------------------------------

bool LinphoneSettings::getRlsUriEnabled () const {
    return !!mConfig->getInt(UiSection, "rls_uri_enabled", true);
}

void LinphoneSettings::setRlsUriEnabled (bool status) {
    mConfig->setInt(UiSection, "rls_uri_enabled", status);
    mConfig->setString("sip", "rls_uri", status ? DefaultRlsUri : "");
    emit rlsUriEnabledChanged(status);
}

static string getRlsUriDomain () {
    static string domain;
    if (!domain.empty())
        return domain;

    shared_ptr<linphone::Address> linphoneAddress = LinphoneCoreManager::getInstance()->getCore()->createAddress(DefaultRlsUri);
    Q_CHECK_PTR(linphoneAddress);
    domain = linphoneAddress->getDomain();
    return domain;
}

void LinphoneSettings::configureRlsUri () {
    // Ensure rls uri is empty.
    if (!getRlsUriEnabled()) {
        mConfig->setString("sip", "rls_uri", "");
        return;
    }

    // Set rls uri if necessary.
    const string domain = getRlsUriDomain();
    for (const auto &proxyConfig : LinphoneCoreManager::getInstance()->getCore()->getProxyConfigList())
        if (proxyConfig->getDomain() == domain) {
            mConfig->setString("sip", "rls_uri", DefaultRlsUri);
            return;
        }

    mConfig->setString("sip", "rls_uri", "");
}

void LinphoneSettings::configureRlsUri (const shared_ptr<const linphone::ProxyConfig> &proxyConfig) {
    if (!getRlsUriEnabled()) {
        mConfig->setString("sip", "rls_uri", "");
        return;
    }

    const string domain = getRlsUriDomain();
    if (proxyConfig->getDomain() == domain) {
        mConfig->setString("sip", "rls_uri", DefaultRlsUri);
        return;
    }

    mConfig->setString("sip", "rls_uri", "");
}

//
// =============================================================================
// Advanced.
// =============================================================================
bool LinphoneSettings::getDeveloperSettingsEnabled () const {
#ifdef DEBUG
    return !!mConfig->getInt(UiSection, "developer_settings", 0);
#else
    return false;
#endif // ifdef DEBUG
}

void LinphoneSettings::setDeveloperSettingsEnabled (bool status) {
#ifdef DEBUG
    mConfig->setInt(UiSection, "developer_settings", status);
    emit developerSettingsEnabledChanged(status);
#else
    Q_UNUSED(status)
    qWarning() << QStringLiteral("Unable to change developer settings mode in release version.");
#endif // ifdef DEBUG
}

void LinphoneSettings::handleCallCreated(const shared_ptr<linphone::Call> &) {
    emit isInCallChanged(getIsInCall());
}

void LinphoneSettings::handleCallStateChanged(const shared_ptr<linphone::Call> &, linphone::Call::State) {
    emit isInCallChanged(getIsInCall());
}
void LinphoneSettings::handleEcCalibrationResult(linphone::EcCalibratorStatus status, int delayMs){
    emit echoCancellationStatus((int)status, delayMs);
}
bool LinphoneSettings::getIsInCall() const {
    return LinphoneCoreManager::getInstance()->getCore()->getCallsNb() != 0;
}
