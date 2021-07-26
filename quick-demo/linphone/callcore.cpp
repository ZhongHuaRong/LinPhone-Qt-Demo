#include "callcore.h"
#include "linphonecoremanager.h"
#include "utils.h"
#include "corehandlers.h"
#include <QDebug>

using namespace std;

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

int CallCore::getRunningCallsNumber () const {
    return LinphoneCoreManager::getInstance()->getCore()->getCallsNb();
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
    auto call = LinphoneCoreManager::getInstance()->getCall();
    if(call)
        call->accept();
}

void CallCore::callTerminate(){
    auto call = LinphoneCoreManager::getInstance()->getCall();
    if(call)
        call->terminate();
}

void CallCore::handleCallStateChanged(const std::shared_ptr<linphone::Call> &call, linphone::Call::State state)
{
    switch (state) {
    case linphone::Call::State::IncomingReceived:
//        addCall(call);
//        joinConference(call);
        qInfo() << "IncomingReceived";
//        callCreate(call);
        LinphoneCoreManager::getInstance()->setCall(call);
        break;

    case linphone::Call::State::OutgoingInit:
//        addCall(call);
        qInfo() << "OutgoingInit";
        break;

    case linphone::Call::State::End:
    case linphone::Call::State::Error:
//        if (call->getCallLog()->getStatus() == linphone::Call::Status::Missed)
//            emit callMissed(&call->getData<CallModel>("call-model"));
//        removeCall(call);
        break;

    case linphone::Call::State::StreamsRunning:
    {
//        int index = findCallIndex(mList, call);
//        emit callRunning(index, &call->getData<CallModel>("call-model"));
    }
    break;
    default:
        break;
    }
}
