#ifndef PROXYUSER_H
#define PROXYUSER_H

#include <linphone++/linphone.hh>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QVariantList>

class AccountSettings: public QObject
{
    Q_OBJECT
public:
    // proxy config.
    Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY accountSettingsUpdated)
    Q_PROPERTY(QString sipAddress READ getUsedSipAddressAsStringUriOnly NOTIFY accountSettingsUpdated)
    Q_PROPERTY(QString fullSipAddress READ getUsedSipAddressAsString)
    Q_PROPERTY(RegistrationState registrationState READ getRegistrationState NOTIFY accountSettingsUpdated)

    enum RegistrationState {
      RegistrationStateRegistered,
      RegistrationStateNotRegistered,
      RegistrationStateInProgress,
      RegistrationStateNoProxy,
    };
    Q_ENUMS(RegistrationState)
public:
    explicit AccountSettings (QObject *parent = nullptr);


    std::shared_ptr<const linphone::Address> getUsedSipAddress () const;
    void setUsedSipAddress (const std::shared_ptr<const linphone::Address> &address);

    QString getUsedSipAddressAsStringUriOnly () const;
    QString getUsedSipAddressAsString () const;

    bool addOrUpdateProxyConfig (const std::shared_ptr<linphone::ProxyConfig> &proxyConfig);

    Q_INVOKABLE QVariantMap getProxyConfigDescription (const std::shared_ptr<linphone::ProxyConfig> &proxyConfig);

    Q_INVOKABLE void setDefaultProxyConfig (const std::shared_ptr<linphone::ProxyConfig> &proxyConfig = nullptr);
    Q_INVOKABLE void setDefaultProxyConfigFromSipAddress (const QString &sipAddress);

    Q_INVOKABLE void addSipAccount(const QVariantMap &data);
    Q_INVOKABLE bool addOrUpdateProxyConfig (const std::shared_ptr<linphone::ProxyConfig> &proxyConfig, const QVariantMap &data);
    Q_INVOKABLE void removeProxyConfig (const std::shared_ptr<linphone::ProxyConfig> &proxyConfig);

    Q_INVOKABLE std::shared_ptr<linphone::ProxyConfig> createProxyConfig ();

    Q_INVOKABLE void addAuthInfo (
      const std::shared_ptr<linphone::AuthInfo> &authInfo,
      const QString &password,
      const QString &userId
    );

    Q_INVOKABLE void eraseAllPasswords ();

  signals:
    void accountSettingsUpdated ();
    void publishPresenceChanged();

  private:
    QString getUsername () const;
    void setUsername (const QString &username);

    RegistrationState getRegistrationState () const;

    // ---------------------------------------------------------------------------

    QString getPrimaryUsername () const;
    void setPrimaryUsername (const QString &username);

    QString getPrimaryDisplayName () const;
    void setPrimaryDisplayName (const QString &displayName);

    QString getPrimarySipAddress () const;

    // ---------------------------------------------------------------------------

    QVariantList getAccounts () const;

    // ---------------------------------------------------------------------------

    void handleRegistrationStateChanged (
      const std::shared_ptr<linphone::ProxyConfig> &proxyConfig,
      linphone::RegistrationState state
    );

    friend class LinphoneCoreManager;
};

Q_DECLARE_METATYPE(std::shared_ptr<linphone::ProxyConfig>);

#endif // PROXYUSER_H
