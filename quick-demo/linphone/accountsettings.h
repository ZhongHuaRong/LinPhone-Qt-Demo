#ifndef PROXYUSER_H
#define PROXYUSER_H

#include <linphone++/linphone.hh>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QVariantList>

/**
 * @brief The AccountSettings class
 * 该类作用于账号设置相关的操作
 */
class AccountSettings: public QObject
{
    Q_OBJECT
public:
    // proxy config.
    Q_PROPERTY(RegistrationState registrationState READ getRegistrationState NOTIFY accountSettingsUpdated)

    enum RegistrationState {
      RegistrationStateRegistered,				//已登录
      RegistrationStateNotRegistered,			//未登录
      RegistrationStateInProgress,				//正在登录
      RegistrationStateNoProxy,					//未设置账号
    };
    Q_ENUMS(RegistrationState)
public:
    explicit AccountSettings (QObject *parent = nullptr);

	/**
	 * @brief addSipAccount
	 * 添加一个Sip账号
	 * @param data			账号相关信息
	 * sipAddress: 自己地址,
	 * serverAddress: 服务地址,
	 * transport: 协议,"TCP",
	 * password:密码
	 */
    Q_INVOKABLE void addSipAccount(const QVariantMap &data);
	
	/**
	 * @brief removeProxyConfig
	 * 移除所有账号信息
	 * @note 每次重新登录时,都会自动登录上次关闭时的状态,需要调用该接口移除所有账号
	 */
    Q_INVOKABLE void removeProxyConfig ();

  signals:
    void accountSettingsUpdated ();

  private:
    RegistrationState getRegistrationState () const;

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
