#include "accountsettings.h"
#include <QtDebug>
#include "linphonecoremanager.h"
#include "corehandlers.h"
#include "utils.h"
#include "linphonesettings.h"


// =============================================================================

using namespace std;

static inline AccountSettings::RegistrationState mapLinphoneRegistrationStateToUi (linphone::RegistrationState state) {
    switch (state) {
    case linphone::RegistrationState::None:
    case linphone::RegistrationState::Cleared:
    case linphone::RegistrationState::Failed:
        return AccountSettings::RegistrationStateNotRegistered;

    case linphone::RegistrationState::Progress:
        return AccountSettings::RegistrationStateInProgress;

    case linphone::RegistrationState::Ok:
        break;
    }

    return AccountSettings::RegistrationStateRegistered;
}

// -----------------------------------------------------------------------------

AccountSettings::AccountSettings (QObject *parent) : QObject(parent) {
    auto coreManager = LinphoneCoreManager::getInstance();
    QObject::connect(
                coreManager->getHandlers().get(), &CoreHandlers::registrationStateChanged,
                this, &AccountSettings::handleRegistrationStateChanged
                );
}

// -----------------------------------------------------------------------------

shared_ptr<const linphone::Address> AccountSettings::getUsedSipAddress () const {
    auto core = LinphoneCoreManager::getInstance()->getCore();
    auto proxyConfig = core->getDefaultProxyConfig();

    return proxyConfig?proxyConfig->getIdentityAddress():core->createPrimaryContactParsed();
}

void AccountSettings::setUsedSipAddress (const shared_ptr<const linphone::Address> &address) {
    auto core = LinphoneCoreManager::getInstance()->getCore();
    auto proxyConfig = core->getDefaultProxyConfig();

    proxyConfig ? proxyConfig->setIdentityAddress(address) : core->setPrimaryContact(address->asString());
}

QString AccountSettings::getUsedSipAddressAsStringUriOnly () const {
    return Utils::coreStringToAppString(getUsedSipAddress()->asStringUriOnly());
}

QString AccountSettings::getUsedSipAddressAsString () const {
    return QString::fromStdString(getUsedSipAddress()->asString());
}
// -----------------------------------------------------------------------------

bool AccountSettings::addOrUpdateProxyConfig (const shared_ptr<linphone::ProxyConfig> &proxyConfig) {
    Q_CHECK_PTR(proxyConfig);

    auto coreManager = LinphoneCoreManager::getInstance();
    auto core = coreManager->getCore();

    auto proxyConfigs = core->getProxyConfigList();
    if (find(proxyConfigs.cbegin(), proxyConfigs.cend(), proxyConfig) != proxyConfigs.cend()) {
        if (proxyConfig->done() == -1) {
            qWarning() << QStringLiteral("Unable to update proxy config: `%1`.")
                          .arg(QString::fromStdString(proxyConfig->getIdentityAddress()->asString()));
            return false;
        }
//        coreManager->getSettingsModel()->configureRlsUri();
    } else {
        if (core->addProxyConfig(proxyConfig) == -1) {
            qWarning() << QStringLiteral("Unable to add proxy config: `%1`.")
                          .arg(QString::fromStdString(proxyConfig->getIdentityAddress()->asString()));
            return false;
        }
//        coreManager->getSettingsModel()->configureRlsUri(proxyConfig);
    }
    emit accountSettingsUpdated();

    return true;
}

QVariantMap AccountSettings::getProxyConfigDescription (const shared_ptr<linphone::ProxyConfig> &proxyConfig) {
    Q_CHECK_PTR(proxyConfig);

    QVariantMap map;

    {
        const shared_ptr<const linphone::Address> address = proxyConfig->getIdentityAddress();
        map["sipAddress"] = address
                ? QString::fromStdString(proxyConfig->getIdentityAddress()->asString())
                : QString("");
    }
    map["serverAddress"] = Utils::coreStringToAppString(proxyConfig->getServerAddr());
    map["registrationDuration"] = proxyConfig->getPublishExpires();

    if( map["serverAddress"].toString().toUpper().contains("TRANSPORT="))// transport has been specified : let the RFC select the transport
        map["transport"] = Utils::coreStringToAppString(proxyConfig->getTransport());
    else// Set to TLS as default
        map["transport"] = "tls";
    if( proxyConfig->getRoutes().size() > 0)
        map["route"] = Utils::coreStringToAppString(proxyConfig->getRoutes().front());
    else
        map["route"] = "";
    map["contactParams"] = Utils::coreStringToAppString(proxyConfig->getContactParameters());
    map["avpfInterval"] = proxyConfig->getAvpfRrInterval();
    map["registerEnabled"] = proxyConfig->registerEnabled();
    map["publishPresence"] = proxyConfig->publishEnabled();
    map["avpfEnabled"] = proxyConfig->getAvpfMode() == linphone::AVPFMode::Enabled;
    map["registrationState"] = mapLinphoneRegistrationStateToUi(proxyConfig->getState());

    shared_ptr<linphone::NatPolicy> natPolicy = proxyConfig->getNatPolicy();
    bool createdNat = !natPolicy;
    if (createdNat)
        natPolicy = proxyConfig->getCore()->createNatPolicy();
    map["iceEnabled"] = natPolicy->iceEnabled();
    map["turnEnabled"] = natPolicy->turnEnabled();

    const string &turnUser(natPolicy->getStunServerUsername());
    const string &stunServer(natPolicy->getStunServer());

    map["turnUser"] = Utils::coreStringToAppString(turnUser);
    map["stunServer"] = Utils::coreStringToAppString(stunServer);

    if (createdNat)
        proxyConfig->setNatPolicy(natPolicy);

    shared_ptr<const linphone::AuthInfo> authInfo = LinphoneCoreManager::getInstance()->getCore()->findAuthInfo(
                "", turnUser, stunServer
                );
    map["turnPassword"] = authInfo ? Utils::coreStringToAppString(authInfo->getPassword()) : QString("");

    return map;
}

void AccountSettings::setDefaultProxyConfig (const shared_ptr<linphone::ProxyConfig> &proxyConfig) {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    if (core->getDefaultProxyConfig() != proxyConfig) {
        core->setDefaultProxyConfig(proxyConfig);
        emit accountSettingsUpdated();
    }
}

void AccountSettings::setDefaultProxyConfigFromSipAddress (const QString &sipAddress) {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    if (Utils::coreStringToAppString(core->createPrimaryContactParsed()->asStringUriOnly()) == sipAddress) {
        setDefaultProxyConfig(nullptr);
        return;
    }

    for (const auto &proxyConfig : core->getProxyConfigList())
        if (Utils::coreStringToAppString(proxyConfig->getIdentityAddress()->asStringUriOnly()) == sipAddress) {
            setDefaultProxyConfig(proxyConfig);
            return;
        }

    qWarning() << "Unable to set default proxy config from:" << sipAddress;
}
void AccountSettings::removeProxyConfig (const shared_ptr<linphone::ProxyConfig> &proxyConfig) {
    if(!proxyConfig){
        auto coreManager = LinphoneCoreManager::getInstance();
        std::list<std::shared_ptr<linphone::ProxyConfig>> allProxies = coreManager->getCore()->getProxyConfigList();

        for(auto i: allProxies){
            std::shared_ptr<const linphone::Address> proxyAddress = i->getIdentityAddress();
            qInfo() << Utils::coreStringToAppString(proxyAddress->asString());
            coreManager->getCore()->removeProxyConfig(i);// Remove first to avoid requesting password when deleting it
            if(i->findAuthInfo())
                coreManager->getCore()->removeAuthInfo(i->findAuthInfo());// Remove passwords
        }

        emit accountSettingsUpdated();
        return;
    }
    Q_CHECK_PTR(proxyConfig);

    auto coreManager = LinphoneCoreManager::getInstance();
    std::list<std::shared_ptr<linphone::ProxyConfig>> allProxies = coreManager->getCore()->getProxyConfigList();
    std::shared_ptr<const linphone::Address> proxyAddress = proxyConfig->getIdentityAddress();

    coreManager->getCore()->removeProxyConfig(proxyConfig);// Remove first to avoid requesting password when deleting it
    if(proxyConfig->findAuthInfo())
        coreManager->getCore()->removeAuthInfo(proxyConfig->findAuthInfo());// Remove passwords

    coreManager->getSettingsModel()->configureRlsUri();

    emit accountSettingsUpdated();
}

void AccountSettings::addSipAccount(const QVariantMap &data)
{
    auto coreManager = LinphoneCoreManager::getInstance();

    shared_ptr<linphone::Factory> factory = linphone::Factory::get();
    shared_ptr<linphone::Core> core = coreManager->getCore();
    shared_ptr<linphone::ProxyConfig> proxyConfig = core->createProxyConfig();

    proxyConfig->setServerAddr(Utils::appStringToCoreString(data["serverAddress"].toString()));

    // Sip Address.
    shared_ptr<linphone::Address> address = factory->createAddress(data["sipAddress"].toString().toStdString());
    address->setDisplayName(data["displayName"].toString().toStdString());
    proxyConfig->setIdentityAddress(address);

    // AuthInfo.
    core->addAuthInfo(
      factory->createAuthInfo(
        address->getUsername(), // Username.
        "", // User ID.
        Utils::appStringToCoreString(data["password"].toString()), // Password.
        "", // HA1.
        "", // Realm.
        address->getDomain() // Domain.
      )
    );
    coreManager->getAccountSettingsModel()->addOrUpdateProxyConfig(proxyConfig);
}

bool AccountSettings::addOrUpdateProxyConfig (
        const shared_ptr<linphone::ProxyConfig> &proxyConfig,
        const QVariantMap &data
        ) {
    Q_CHECK_PTR(proxyConfig);
    bool newPublishPresence = false;

    proxyConfig->edit();

    QString literal = data["sipAddress"].toString();

    // Sip address.
    {
        shared_ptr<linphone::Address> address = linphone::Factory::get()->createAddress(literal.toStdString());
        if (!address) {
            qWarning() << QStringLiteral("Unable to create sip address object from: `%1`.").arg(literal);
            return false;
        }

        if (proxyConfig->setIdentityAddress(address)) {
            qWarning() << QStringLiteral("Unable to set identity address: `%1`.")
                          .arg(Utils::coreStringToAppString(address->asStringUriOnly()));
            return false;
        }
    }

    // Server address.
    {
        QString serverAddress = data["serverAddress"].toString();

        if (proxyConfig->setServerAddr(Utils::appStringToCoreString(serverAddress))) {
            qWarning() << QStringLiteral("Unable to add server address: `%1`.").arg(serverAddress);
            return false;
        }
    }

    proxyConfig->setPublishExpires(data["registrationDuration"].toInt());
    proxyConfig->setRoute(Utils::appStringToCoreString(data["route"].toString()));
    proxyConfig->setContactParameters(Utils::appStringToCoreString(data["contactParams"].toString()));
    proxyConfig->setAvpfRrInterval(uint8_t(data["avpfInterval"].toInt()));
    proxyConfig->enableRegister(data["registerEnabled"].toBool());
    newPublishPresence = proxyConfig->publishEnabled() != data["publishPresence"].toBool();
    proxyConfig->enablePublish(data["publishPresence"].toBool());
    proxyConfig->setAvpfMode(data["avpfEnabled"].toBool()
            ? linphone::AVPFMode::Enabled
            : linphone::AVPFMode::Default
              );

    shared_ptr<linphone::NatPolicy> natPolicy = proxyConfig->getNatPolicy();
    bool createdNat = !natPolicy;
    if (createdNat)
        natPolicy = proxyConfig->getCore()->createNatPolicy();
    natPolicy->enableIce(data["iceEnabled"].toBool());
    natPolicy->enableStun(data["iceEnabled"].toBool());

    const string turnUser(Utils::appStringToCoreString(data["turnUser"].toString()));
    const string stunServer(Utils::appStringToCoreString(data["stunServer"].toString()));

    natPolicy->enableTurn(data["turnEnabled"].toBool());
    natPolicy->setStunServerUsername(turnUser);
    natPolicy->setStunServer(stunServer);

    if( createdNat)
        proxyConfig->setNatPolicy(natPolicy);

    shared_ptr<linphone::Core> core(proxyConfig->getCore());
    shared_ptr<const linphone::AuthInfo> authInfo(core->findAuthInfo("", turnUser, stunServer));
    if (authInfo) {
        shared_ptr<linphone::AuthInfo> clonedAuthInfo(authInfo->clone());
        clonedAuthInfo->setUserid(turnUser);
        clonedAuthInfo->setUsername(turnUser);
        clonedAuthInfo->setPassword(Utils::appStringToCoreString(data["turnPassword"].toString()));

        core->addAuthInfo(clonedAuthInfo);
        core->removeAuthInfo(authInfo);
    } else
        core->addAuthInfo(linphone::Factory::get()->createAuthInfo(
                              turnUser,
                              turnUser,
                              Utils::appStringToCoreString(data["turnPassword"].toString()),
                          "",
                          stunServer,
                          ""
                          ));
    if( newPublishPresence)
        emit publishPresenceChanged();
    return addOrUpdateProxyConfig(proxyConfig);
}

shared_ptr<linphone::ProxyConfig> AccountSettings::createProxyConfig () {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();

//    core->getConfig()->loadFromXmlFile(
//                Paths::getAssistantConfigDirPath() + "create-app-sip-account.rc"
//                );

    return core->createProxyConfig();
}

void AccountSettings::addAuthInfo (
        const shared_ptr<linphone::AuthInfo> &authInfo,
        const QString &password,
        const QString &userId
        ) {
    authInfo->setPassword(Utils::appStringToCoreString(password));
    authInfo->setUserid(Utils::appStringToCoreString(userId));

    LinphoneCoreManager::getInstance()->getCore()->addAuthInfo(authInfo);
}

void AccountSettings::eraseAllPasswords () {
    LinphoneCoreManager::getInstance()->getCore()->clearAllAuthInfo();
}

// -----------------------------------------------------------------------------

QString AccountSettings::getUsername () const {
    shared_ptr<const linphone::Address> address = getUsedSipAddress();
    const string displayName = address->getDisplayName();

    return QString::fromStdString(
                displayName.empty() ? address->getUsername() : displayName
                                      );
}

void AccountSettings::setUsername (const QString &username) {
    shared_ptr<const linphone::Address> address = getUsedSipAddress();
    shared_ptr<linphone::Address> newAddress = address->clone();

    if (newAddress->setDisplayName(username.toStdString())) {
        qWarning() << QStringLiteral("Unable to set displayName on sip address: `%1`.")
                      .arg(Utils::coreStringToAppString(newAddress->asStringUriOnly()));
    } else {
        setUsedSipAddress(newAddress);
    }

    emit accountSettingsUpdated();
}

AccountSettings::RegistrationState AccountSettings::getRegistrationState () const {
    shared_ptr<linphone::ProxyConfig> proxyConfig = LinphoneCoreManager::getInstance()->getCore()->getDefaultProxyConfig();
    return proxyConfig ? mapLinphoneRegistrationStateToUi(proxyConfig->getState()) : RegistrationStateNoProxy;
}

// -----------------------------------------------------------------------------

QString AccountSettings::getPrimaryUsername () const {
    return QString::fromStdString(
                LinphoneCoreManager::getInstance()->getCore()->createPrimaryContactParsed()->getUsername()
                );
}

void AccountSettings::setPrimaryUsername (const QString &username) {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    shared_ptr<linphone::Address> primary = core->createPrimaryContactParsed();

    primary->setUsername(Utils::appStringToCoreString(
                             username.isEmpty() ? "1111" : username
                                                  ));
    core->setPrimaryContact(primary->asString());

    emit accountSettingsUpdated();
}

QString AccountSettings::getPrimaryDisplayName () const {
    return QString::fromStdString(LinphoneCoreManager::getInstance()->getCore()->createPrimaryContactParsed()->getDisplayName());
}

void AccountSettings::setPrimaryDisplayName (const QString &displayName) {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    shared_ptr<linphone::Address> primary = core->createPrimaryContactParsed();

    primary->setDisplayName(displayName.toStdString());
    core->setPrimaryContact(primary->asString());

    emit accountSettingsUpdated();
}

QString AccountSettings::getPrimarySipAddress () const {
    return QString::fromStdString(
                LinphoneCoreManager::getInstance()->getCore()->createPrimaryContactParsed()->asString()
                );
}

// -----------------------------------------------------------------------------

QVariantList AccountSettings::getAccounts () const {
    shared_ptr<linphone::Core> core = LinphoneCoreManager::getInstance()->getCore();
    QVariantList accounts;

    for (const auto &proxyConfig : core->getProxyConfigList()) {
        QVariantMap account;
        account["sipAddress"] = Utils::coreStringToAppString(proxyConfig->getIdentityAddress()->asStringUriOnly());
        account["fullSipAddress"] = QString::fromStdString(proxyConfig->getIdentityAddress()->asString());
        account["proxyConfig"].setValue(proxyConfig);
        account["unreadMessageCount"] = proxyConfig->getUnreadChatMessageCount();
        account["missedCallCount"] = 0;
        accounts << account;
    }

    return accounts;
}

// -----------------------------------------------------------------------------

void AccountSettings::handleRegistrationStateChanged (
        const shared_ptr<linphone::ProxyConfig> & proxy,
        linphone::RegistrationState core
        ) {
    Q_UNUSED(proxy)
    Q_UNUSED(core)
    emit accountSettingsUpdated();
}
