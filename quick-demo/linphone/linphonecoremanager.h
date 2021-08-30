#ifndef LINPHONECOREMANAGER_H
#define LINPHONECOREMANAGER_H

#include <QObject>
#include <QMutex>
#include <QTimer>

#include "linphone++/linphone.hh"
#include "corehandlers.h"
#include "accountsettings.h"
#include "callcore.h"

class LinphoneCoreManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AccountSettings* accountSettings READ getAccountSettingsModel CONSTANT)
    Q_PROPERTY(CallCore* callcore READ getCallCore CONSTANT)
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

    inline AccountSettings *getAccountSettingsModel () const {
      return accountSettings;
    }

    inline CallCore *getCallCore() const {
        return callcore;
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

    void reset();
signals:
    void coreManagerInitialized ();

private:
  LinphoneCoreManager (QObject *parent, const QString &configPath);
  void iterate ();
private:
    std::shared_ptr<linphone::Core> mCore;
    std::shared_ptr<CoreHandlers> mHandlers;


    QTimer *mCbsTimer = nullptr;

    AccountSettings *accountSettings{nullptr};
    CallCore *callcore{nullptr};

    QMutex mMutexVideoRender;

    static LinphoneCoreManager *mInstance;
};

#endif // LINPHONECOREMANAGER_H
