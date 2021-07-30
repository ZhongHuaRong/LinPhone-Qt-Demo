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

void CallCore::takeSnapshot()
{
    auto call = LinphoneCoreManager::getInstance()->getCall();
    if(!call)
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
    call->takeVideoSnapshot(Utils::appStringToCoreString(filePath));
}

void CallCore::enableCamera(bool enabled)
{
    auto call = LinphoneCoreManager::getInstance()->getCall();
    if(!call)
        return;
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    if (!core->videoSupported()) {
        qWarning() << QStringLiteral("Unable to update video call property. (Video not supported.)");
        return;
    }

    switch (call->getState()) {
    case linphone::Call::State::Connected:
    case linphone::Call::State::StreamsRunning:
        break;
    default: return;
    }

    shared_ptr<linphone::CallParams> params = core->createCallParams(call);
    params->enableVideo(enabled);

    call->update(params);
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
        LinphoneCoreManager::getInstance()->setCall(call);
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

QString CallCore::generateSavedFilename() const
{
    const shared_ptr<linphone::CallLog> callLog(LinphoneCoreManager::getInstance()->getCall()->getCallLog());
    return generateSavedFilename(
                QString::fromStdString(callLog->getFromAddress()->getUsername()),
                QString::fromStdString(callLog->getToAddress()->getUsername())
                );
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
