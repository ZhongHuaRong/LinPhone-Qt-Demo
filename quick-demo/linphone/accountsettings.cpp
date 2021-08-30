#include "accountsettings.h"
#include <QtDebug>
#include "linphonecoremanager.h"
#include "corehandlers.h"
#include "utils.h"


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

void AccountSettings::removeProxyConfig () {
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
}

void AccountSettings::addSipAccount(const QVariantMap &data)
{
    qInfo() << data;
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

    auto proxyConfigs = core->getProxyConfigList();
    if (find(proxyConfigs.cbegin(), proxyConfigs.cend(), proxyConfig) != proxyConfigs.cend()) {
        if (proxyConfig->done() == -1) {
            qWarning() << QStringLiteral("Unable to update proxy config: `%1`.")
                          .arg(QString::fromStdString(proxyConfig->getIdentityAddress()->asString()));
            return;
        }
    } else {
        if (core->addProxyConfig(proxyConfig) == -1) {
            qWarning() << QStringLiteral("Unable to add proxy config: `%1`.")
                          .arg(QString::fromStdString(proxyConfig->getIdentityAddress()->asString()));
            return;
        }
    }

    core->setDefaultProxyConfig(proxyConfig);
}

// -----------------------------------------------------------------------------

AccountSettings::RegistrationState AccountSettings::getRegistrationState () const {
    shared_ptr<linphone::ProxyConfig> proxyConfig = LinphoneCoreManager::getInstance()->getCore()->getDefaultProxyConfig();
//    qInfo() << "getRegistrationState" << (int)proxyConfig->getState() << QString::fromStdString(proxyConfig->getServerAddr());
    return proxyConfig ? mapLinphoneRegistrationStateToUi(proxyConfig->getState()) : RegistrationStateNoProxy;
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
    Q_UNUSED(core)
    qInfo() << "account state";
    if(proxy != LinphoneCoreManager::getInstance()->getCore()->getDefaultProxyConfig())
        return;
    if(proxy)
        qInfo() << (int)proxy->getState();
    emit accountSettingsUpdated();
}
