#include "callcore.h"
#include "linphonecoremanager.h"
#include "utils.h"
#include "corehandlers.h"
#include <QDateTime>
#include <QDebug>
#include <QGuiApplication>
#include <QRegularExpression>
#include <linphone++/call.hh>

using namespace std;

static inline CallCore::CallState mapLinphoneCallStateToUi (linphone::Call::State state) {
    switch (state) {
    case linphone::Call::State::End:
    case linphone::Call::State::Released:
    case linphone::Call::State::Error:
        return CallCore::CallStateEnd;
    case linphone::Call::State::StreamsRunning:
    case linphone::Call::State::Connected:
    case linphone::Call::State::IncomingReceived:
    case linphone::Call::State::PushIncomingReceived:
    case linphone::Call::State::Updating:
        return CallCore::CallStateRunning;
    case linphone::Call::State::PausedByRemote:
    case linphone::Call::State::Paused:
    case linphone::Call::State::Pausing:
        return CallCore::CallStatePausing;
    case linphone::Call::State::Idle:
    case linphone::Call::State::EarlyUpdating:
    case linphone::Call::State::EarlyUpdatedByRemote:
    case linphone::Call::State::IncomingEarlyMedia:
    case linphone::Call::State::UpdatedByRemote:
    case linphone::Call::State::Referred:
    case linphone::Call::State::OutgoingEarlyMedia:
    case linphone::Call::State::OutgoingRinging:
    case linphone::Call::State::OutgoingProgress:
    case linphone::Call::State::OutgoingInit:
    case linphone::Call::State::Resuming:
        return CallCore::CallStateReadying;
    }

    return CallCore::CallStateUnknown;
}


CallCore::CallCore(QObject *parent) : QObject(parent)
{
    auto mCoreHandlers = LinphoneCoreManager::getInstance()->getHandlers();
    QObject::connect(
                mCoreHandlers.get(), &CoreHandlers::callStateChanged,
                this, &CallCore::handleCallStateChanged
                );
}

CallCore::~CallCore()
{

}

void CallCore::launchAudioCall (const QString &sipAddress, const QHash<QString, QString> &headers) const {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();

    shared_ptr<linphone::Address> address = core->interpretUrl(Utils::appStringToCoreString(sipAddress));
    if (!address)
        return;

    shared_ptr<linphone::CallParams> params = core->createCallParams(nullptr);
    params->enableVideo(false);

    QHashIterator<QString, QString> iterator(headers);
    while (iterator.hasNext()) {
        iterator.next();
        params->addCustomHeader(Utils::appStringToCoreString(iterator.key()), Utils::appStringToCoreString(iterator.value()));
    }
    params->setProxyConfig(core->getDefaultProxyConfig());
    //  CallModel::setRecordFile(params, QString::fromStdString(address->getUsername()));
    shared_ptr<linphone::ProxyConfig> currentProxyConfig = core->getDefaultProxyConfig();
    if(currentProxyConfig){
        if(currentProxyConfig->getState() == linphone::RegistrationState::Ok)
            core->inviteAddressWithParams(address, params);
        else{
            QObject * context = new QObject();
            QObject::connect(LinphoneCoreManager::getInstance()->getHandlers().get(), &CoreHandlers::registrationStateChanged,context,
                             [address,core,params,currentProxyConfig, context](const std::shared_ptr<linphone::ProxyConfig> &proxyConfig, linphone::RegistrationState state) mutable {
                if(context && proxyConfig==currentProxyConfig && state==linphone::RegistrationState::Ok){
                    delete context;
                    context = nullptr;
                    core->inviteAddressWithParams(address, params);
                }
            });
        }
    }else
    core->inviteAddressWithParams(address, params);
}

void CallCore::launchVideoCall (const QString &sipAddress) const {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    if (!core->videoSupported()) {
        qWarning() << QStringLiteral("Unable to launch video call. (Video not supported.) Launching audio call...");
        launchAudioCall(sipAddress);
        return;
    }

    shared_ptr<linphone::Address> address = core->interpretUrl(Utils::appStringToCoreString(sipAddress));
    if (!address)
        return;

    shared_ptr<linphone::CallParams> params = core->createCallParams(nullptr);
    params->enableVideo(true);
    params->setProxyConfig(core->getDefaultProxyConfig());
    //  CallModel::setRecordFile(params, QString::fromStdString(address->getUsername()));
    core->inviteAddressWithParams(address, params);
}

// -----------------------------------------------------------------------------

void CallCore::takeSnapshot()
{
    if(!currentCall)
        return;

    static QString oldName;
    QString newName(generateSavedFilename().append(".jpg"));

    if (newName == oldName) {
        qWarning() << QStringLiteral("Unable to take snapshot. Wait one second.");
        return;
    }
    oldName = newName;

    qInfo() << QStringLiteral("Take snapshot of call:") << this;

    const QString filePath(qApp->applicationDirPath() + "/screenshot/" + newName);
    qInfo() << filePath;
    currentCall->takeVideoSnapshot(Utils::appStringToCoreString(filePath));
}

void CallCore::enableCamera(bool enabled)
{
    if(!currentCall || currentCall->cameraEnabled() == enabled)
        return;

    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    if (!core->videoSupported()) {
        qWarning() << QStringLiteral("Unable to update video call property. (Video not supported.)");
        return;
    }

    switch (currentCall->getState()) {
    case linphone::Call::State::Connected:
    case linphone::Call::State::StreamsRunning:
        break;
    default: return;
    }

    shared_ptr<linphone::CallParams> params = core->createCallParams(currentCall);
    params->enableVideo(enabled);

    currentCall->update(params);
}

void CallCore::terminateAllCalls () const {
    LinphoneCoreManager::getInstance()->getCore()->terminateAllCalls();
}

void CallCore::terminateCall (const QString& sipAddress) const{
    auto coreManager = LinphoneCoreManager::getInstance();
    shared_ptr<linphone::Address> address = coreManager->getCore()->interpretUrl(Utils::appStringToCoreString(sipAddress));
    if (!address)
        qWarning() << "Cannot terminate Call. The address cannot be parsed : " << sipAddress;
    else{
        std::shared_ptr<linphone::Call> call = coreManager->getCore()->getCallByRemoteAddress2(address);
        if( call){
            coreManager->lockVideoRender();
            call->terminate();
            coreManager->unlockVideoRender();
        }else{
            qWarning() << "Cannot terminate call as it doesn't exist : " << sipAddress;
        }
    }
}

void CallCore::callAccept(){
    if(!currentCall)
        return;
	
    qApp->processEvents();  // Process GUI events before accepting in order to be synchronized with Call objects and be ready to get SDK events
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    shared_ptr<linphone::CallParams> params = core->createCallParams(currentCall);
//    params->enableVideo(false);
	//端口复用,防止信令接收失败
	params->enableRtpBundle(true);
    currentCall->acceptWithParams(params);
}

void CallCore::callTerminate(){
    if(currentCall)
        currentCall->terminate();
}

void CallCore::reloadCamera()
{
	auto core = LinphoneCoreManager::getInstance()->getCore();
    core->reloadVideoDevices();
    m_videoDevices.clear();
    for (const auto &device : core->getVideoDevicesList())
		m_videoDevices << Utils::coreStringToAppString(device);
    setVideoDevice(m_videoDevice);
    
	emit videoDevicesChanged(m_videoDevices);
}

void CallCore::reloadSoundDevice()
{
	auto core = LinphoneCoreManager::getInstance()->getCore();
	core->reloadSoundDevices();
	m_playbackDevices.clear();
	for (const auto &device : core->getSoundDevicesList()) {
		if (core->soundDeviceCanPlayback(device)) {
			m_playbackDevices << Utils::coreStringToAppString(device);
		}
	}
	setPlaybackDevice(m_playbackDevice);
	
	emit playbackDevicesChanged(m_playbackDevices);
}

void CallCore::reloadPlayerVolume()
{
	setPlayerVolume(currentCall?currentCall->getSpeakerVolumeGain():0.f);
}

void CallCore::setCall(std::shared_ptr<linphone::Call> c)
{
    if(c == currentCall)
        return;
    currentCall = c;
    setMicroMuted(c?c->getMicrophoneMuted():false);
    reloadPlayerVolume();
	//在Call重置时需要重置状态值(只有调用reset重置状态时才会为空)
    if(!c)
        setCallState(CallState::CallStateUnknown);
    emit callChanged();
}

void CallCore::setMicroMuted(bool microMuted)
{
    if (m_microMuted == microMuted)
        return;

    m_microMuted = microMuted;
    if(currentCall)
        currentCall->setMicrophoneMuted(microMuted);
    emit microMutedChanged(m_microMuted);
}

void CallCore::handleCallStateChanged(const std::shared_ptr<linphone::Call> &call, linphone::Call::State state)
{
    //    qInfo() << "call state:" << (int)state;
    switch (state) {
    case linphone::Call::State::IncomingReceived:
        qInfo() << "IncomingReceived";
        setCall(call);
        break;

    case linphone::Call::State::OutgoingInit:
        qInfo() << "OutgoingInit";
        setCall(call);
        break;

    case linphone::Call::State::End:
    case linphone::Call::State::Error:
//        qInfo() << (int)call->getCallLog()->getStatus();
//        qInfo() << (int)call->getCallLog()->getErrorInfo()->getReason();
        qInfo() << QString::fromStdString(call->getCallLog()->getErrorInfo()->getWarnings());
        break;

    case linphone::Call::State::StreamsRunning:
    {
//        int index = findCallIndex(mList, call);
//        emit callRunning(index, &call->getData<CallModel>("call-model"));
        break;
    }
    default:
        break;
    }

    if(call == currentCall){
        qInfo() << "call state2:" << (int)state;
        setCallState(mapLinphoneCallStateToUi(state));
    }
}

void CallCore::dealCallChanged()
{
    if(!currentCall)
        return;

    qInfo() << "call " << QString::fromStdString(currentCall->getRemoteAddressAsString());
    callAccept();
}

void CallCore::setVideoDevice(int videoDevice)
{
    if(videoDevice < 0 || videoDevice >= m_videoDevices.count())
        videoDevice = 0;
	
	auto core = LinphoneCoreManager::getInstance()->getCore();
	auto device = Utils::appStringToCoreString(m_videoDevices[videoDevice]);
	if(device == core->getVideoDevice())
		return;
	core->setVideoDevice(device);
    m_videoDevice = videoDevice;
	emit videoDeviceChanged(m_videoDevice);
}

void CallCore::setPlaybackDevice(int playbackDevice)
{
	if(playbackDevice < 0 || playbackDevice >= m_playbackDevices.count())
        playbackDevice = 0;
	
	auto core = LinphoneCoreManager::getInstance()->getCore();
	
	std::string devId = Utils::appStringToCoreString(m_playbackDevices[playbackDevice]);
	
	auto list = core->getExtendedAudioDevices();
	auto audioDevice = find_if(list.cbegin(), list.cend(), [&] ( const std::shared_ptr<linphone::AudioDevice> & audioItem) {
	   return audioItem->getId() == devId;
	});
	
	if(audioDevice != list.cend()){

		core->setPlaybackDevice(devId);
		core->setOutputAudioDevice(*audioDevice);
		m_playbackDevice = playbackDevice;
		//更换播放器时需要重新加载音量数值
		reloadPlayerVolume();
		emit playbackDeviceChanged(m_playbackDevice);
	}else
		qWarning() << "Cannot set Playback device. The ID cannot be matched with an existant device : " << m_playbackDevices[playbackDevice];
}

void CallCore::setPlayerVolume(float playerVolume)
{
    //    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_playerVolume, playerVolume))
        return;
    
    m_playerVolume = playerVolume;
    if(currentCall)
        currentCall->setSpeakerVolumeGain(playerVolume);
    emit playerVolumeChanged(m_playerVolume);
}

QString CallCore::generateSavedFilename() const
{
    //    const shared_ptr<linphone::CallLog> callLog(LinphoneCoreManager::getInstance()->getCall()->getCallLog());
    //    return generateSavedFilename(
    //                QString::fromStdString(callLog->getFromAddress()->getUsername()),
    //                QString::fromStdString(callLog->getToAddress()->getUsername())
    //             );
    return QString();
}

QString CallCore::generateSavedFilename(const QString &from, const QString &to)
{
    auto escape = [](const QString &str) {
        constexpr char ReservedCharacters[] = "<>:\"/\\|\\?\\*";
        static QRegularExpression regexp(ReservedCharacters);
        return QString(str).replace(regexp, "");
    };

    return QStringLiteral("%1_%2_%3")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"))
            .arg(escape(from))
            .arg(escape(to));
}
