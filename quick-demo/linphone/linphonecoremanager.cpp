#include "linphonecoremanager.h"
#include "utils.h"
#include <QCoreApplication>
#include <QDebug>
#include <QLibrary>
#include <QQmlEngine>
#include "camera.h"
#include <linphone++/config.hh>
#include <linphone++/core.hh>
#include <linphone++/factory.hh>
#include <linphone++/payload_type.hh>

LinphoneCoreManager *LinphoneCoreManager::mInstance{nullptr};


LinphoneCoreManager *LinphoneCoreManager::getInstance () noexcept{
   return mInstance;
}

LinphoneCoreManager::LinphoneCoreManager(QObject *parent, const QString &configPath):
    QObject(parent), mHandlers(std::make_shared<CoreHandlers>(this))
{

    qRegisterMetaType<std::shared_ptr<linphone::ProxyConfig>>();
    qmlRegisterType<LinphoneCoreManager>("an.qt.im", 1, 0, "LinphoneCoreManager");
    qmlRegisterType<AccountSettings>("an.qt.im", 1, 0, "AccountSettings");
    qmlRegisterType<CallCore>("an.qt.im", 1, 0, "CallCore");
    qmlRegisterType<Camera>("an.qt.im", 1, 0, "Camera");

    mCore = nullptr;
    CoreHandlers *coreHandlers = mHandlers.get();
    QObject::connect(coreHandlers, &CoreHandlers::coreStarting, this, &LinphoneCoreManager::startIterate, Qt::QueuedConnection);
    QObject::connect(coreHandlers, &CoreHandlers::coreStarted, this, &LinphoneCoreManager::initCoreManager, Qt::QueuedConnection);
    QObject::connect(coreHandlers, &CoreHandlers::coreStopped, this, &LinphoneCoreManager::stopIterate, Qt::QueuedConnection);
    QTimer::singleShot(100, [this, configPath](){// Delay the creation in order to have the CoreManager instance set before
        createLinphoneCore(configPath);
    });
}

LinphoneCoreManager::~LinphoneCoreManager()
{
    mCore->removeListener(mHandlers);
    mHandlers = nullptr;// Ordering Call destructor just to be sure (removeListener should be enough)
    mCore = nullptr;
}

void LinphoneCoreManager::initCoreManager()
{
    accountSettings = new AccountSettings(this);
    callcore = new CallCore(this);
    qInfo() << QStringLiteral("CoreManager initialized");
	callcore->reloadSoundDevice();
    emit coreManagerInitialized();
}

void LinphoneCoreManager::startIterate()
{
    mCbsTimer = new QTimer(this);
    mCbsTimer->setInterval(20);
    QObject::connect(mCbsTimer, &QTimer::timeout, this, &LinphoneCoreManager::iterate);
    qInfo() << QStringLiteral("Start iterate");
    mCbsTimer->start();
}

void LinphoneCoreManager::stopIterate()
{
    qInfo() << QStringLiteral("Stop iterate");
    mCbsTimer->stop();
    mCbsTimer->deleteLater();// allow the timer to continue its stuff
    mCbsTimer = nullptr;
}

void LinphoneCoreManager::iterate () {
    lockVideoRender();
    if(mCore)
        mCore->iterate();
    unlockVideoRender();
}

void LinphoneCoreManager::createLinphoneCore (const QString &configPath) {
  qInfo() << QStringLiteral("Launch async core creation.");


  std::shared_ptr<linphone::Factory> factory = linphone::Factory::get();
//  qInfo() << qApp->applicationDirPath();
//  qInfo() << QString::fromStdString(factory->getDataResourcesDir());
//  qInfo() << QString::fromStdString(factory->getImageResourcesDir());
//  qInfo() << QString::fromStdString(factory->getRingResourcesDir());
//  qInfo() << QString::fromStdString(factory->getSoundResourcesDir());
//  qInfo() << QString::fromStdString(factory->getTopResourcesDir());
  factory->setMspluginsDir(Utils::appStringToCoreString(qApp->applicationDirPath() + "/plugins/mediastreamer"));
  factory->setImageResourcesDir(Utils::appStringToCoreString(qApp->applicationDirPath() + "/images"));
  // Migration of configuration and database files from GTK version of Linphone.

  qInfo() << "configPath:" << configPath;
  //先删除原来配置信息
//  QFile::remove(configPath);
  mCore = linphone::Factory::get()->createCore(
    Utils::appStringToCoreString(configPath),
    Utils::appStringToCoreString(configPath),
    nullptr
  );
  mCore->addListener(mHandlers);
  qInfo() << Utils::coreStringToAppString(mCore->getDefaultVideoDisplayFilter());
  auto codecs = mCore->getVideoPayloadTypes();
  for(auto codec:codecs){
      QVariantMap map;

      map["bitrate"] = codec->getNormalBitrate();
      map["channels"] = codec->getChannels();
      map["clockRate"] = codec->getClockRate();
      map["description"] = Utils::coreStringToAppString(codec->getDescription());
      map["enabled"] = codec->enabled();
      map["encoderDescription"] = Utils::coreStringToAppString(codec->getEncoderDescription());
      map["isUsable"] = codec->isUsable(); // TODO: Notify in UI when unusable.
      map["isVbr"] = codec->isVbr();
      map["mime"] = Utils::coreStringToAppString(codec->getMimeType());
      map["number"] = codec->getNumber();
      map["recvFmtp"] = Utils::coreStringToAppString(codec->getRecvFmtp());
//      qInfo() << map;
  }
  codecs.push_back(codecs.front());
  codecs.pop_front();
  mCore->setVideoPayloadTypes(codecs);
  
//  auto audios = mCore->getAudioPayloadTypes();
//  for(auto codec:audios){
//	  QVariantMap map;

//	  codec->enable(true);
//      map["bitrate"] = codec->getNormalBitrate();
//      map["channels"] = codec->getChannels();
//      map["clockRate"] = codec->getClockRate();
//      map["description"] = Utils::coreStringToAppString(codec->getDescription());
//      map["enabled"] = codec->enabled();
//      map["encoderDescription"] = Utils::coreStringToAppString(codec->getEncoderDescription());
//      map["isUsable"] = codec->isUsable(); // TODO: Notify in UI when unusable.
//      map["isVbr"] = codec->isVbr();
//      map["mime"] = Utils::coreStringToAppString(codec->getMimeType());
//      map["number"] = codec->getNumber();
//      map["recvFmtp"] = Utils::coreStringToAppString(codec->getRecvFmtp());
//      qInfo() << map;
//  }
//  audios.push_back(codecs.front());
//  audios.pop_front();
//  mCore->setAudioPayloadTypes(audios);
  
  mCore->setVideoDisplayFilter("MSQOGL");
  mCore->usePreviewWindow(true);
//  mCore->enableVideoPreview(true);
  mCore->setUserAgent(
    Utils::appStringToCoreString(
      QStringLiteral("demo Desktop/%1 (%2, Qt %3) LinphoneCore")
        .arg(QCoreApplication::applicationVersion())
        .arg(QSysInfo::prettyProductName())
        .arg(qVersion())
    ),
    mCore->getVersion()
  );
  // Force capture/display.
  // Useful if the app was built without video support.
  // (The capture/display attributes are reset by the core in this case.)
  if (mCore->videoSupported()) {
    std::shared_ptr<linphone::Config> config = mCore->getConfig();
    config->setInt("video", "capture", 1);
    config->setInt("video", "display", 1);
  }

  mCore->start();
}

void LinphoneCoreManager::reset()
{
	callcore->terminateAllCalls();
	callcore->setCall(nullptr);
    accountSettings->removeProxyConfig();
}

// -----------------------------------------------------------------------------

void LinphoneCoreManager::init (QObject *parent, const QString &configPath) {
  if (mInstance)
    return;
  mInstance = new LinphoneCoreManager(parent, configPath);
}

void LinphoneCoreManager::uninit () {
  if (mInstance) {
    connect(mInstance, &QObject::destroyed, []()mutable{
        mInstance = nullptr;
        qInfo() << "Core is correctly destroyed";
    });
    QObject::connect(mInstance->getHandlers().get(), &CoreHandlers::coreStopped, mInstance, &QObject::deleteLater); // Delete data only when the core is Off

    mInstance->lockVideoRender();// Stop do iterations. We have to protect GUI.
    mInstance->mCore->stop();
    mInstance->unlockVideoRender();
    mInstance = nullptr;
//    QTest::qWaitFor([&]() {return mInstance == nullptr;},10000);
    if( mInstance){
        qWarning() << "Core couldn't destroy in time. It may lead to have multiple session of Core";
        mInstance = nullptr;
    }
  }
}
