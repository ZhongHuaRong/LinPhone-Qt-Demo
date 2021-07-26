#ifndef LINPHONECOREMANAGER_H
#define LINPHONECOREMANAGER_H

#include <QObject>
#include <QMutex>
#include <QTimer>

#include "linphone++/linphone.hh"
#include "corehandlers.h"
#include "accountsettings.h"
#include "linphonesettings.h"
#include "callcore.h"

class LinphoneCoreManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(LinphoneSettings* settings READ getSettingsModel CONSTANT)
    Q_PROPERTY(AccountSettings* accountSettings READ getAccountSettingsModel CONSTANT)
    Q_PROPERTY(CallCore* callcore READ getCallCore CONSTANT)
    Q_PROPERTY(std::shared_ptr<linphone::Call> call READ getCall NOTIFY callChanged)
public:
    LinphoneCoreManager (){}
    ~LinphoneCoreManager ();
    static LinphoneCoreManager *getInstance () noexcept;

    inline std::shared_ptr<linphone::Core> getCore()  noexcept{
      return mCore;
    }

    inline std::shared_ptr<CoreHandlers> getHandlers () {
      return mHandlers;
    }

    inline LinphoneSettings *getSettingsModel () const {
      return settings;
    }

    inline AccountSettings *getAccountSettingsModel () const {
      return accountSettings;
    }

    inline CallCore *getCallCore() const {
        return callcore;
    }

    inline std::shared_ptr<linphone::Call> getCall() const{
        return currentCall;
    }

    inline void setCall(std::shared_ptr<linphone::Call> c) {
        currentCall = c;
        emit callChanged();
    }

    // ---------------------------------------------------------------------------
    // Video render lock.
    // ---------------------------------------------------------------------------
    inline void lockVideoRender () {
      mMutexVideoRender.lock();
    }

    inline void unlockVideoRender () {
      mMutexVideoRender.unlock();
    }

    // ---------------------------------------------------------------------------
    // Initialization.
    // ---------------------------------------------------------------------------

    static void init (QObject *parent, const QString &configPath);
    static void uninit ();
public slots:
    void initCoreManager();
    void startIterate();
    void stopIterate();
    void createLinphoneCore (const QString &configPath);// In order to delay creation
signals:
    void coreManagerInitialized ();
    void callChanged();
private:
  LinphoneCoreManager (QObject *parent, const QString &configPath);
  void iterate ();
private:
    std::shared_ptr<linphone::Core> mCore;
    std::shared_ptr<CoreHandlers> mHandlers;


    QTimer *mCbsTimer = nullptr;

    LinphoneSettings *settings{nullptr};
    AccountSettings *accountSettings{nullptr};
    CallCore *callcore{nullptr};

    std::shared_ptr<linphone::Call> currentCall;

    QMutex mMutexVideoRender;

    static LinphoneCoreManager *mInstance;
};

#endif // LINPHONECOREMANAGER_H
