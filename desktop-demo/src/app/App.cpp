/*
 * Copyright (c) 2010-2020 Belledonne Communications SARL.
 *
 * This file is part of linphone-desktop
 * (see https://www.linphone.org).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "App.hpp"

#ifdef Q_OS_WIN
  #include <QSettings>
#endif // ifdef Q_OS_WIN
#include <QCommandLineParser>
#include <QDir>
#include <QFileSelector>
#include <QLibraryInfo>
#include <QMenu>
#include <QQmlApplicationEngine>
#include <QQmlFileSelector>
#include <QQmlContext>
#include <QQuickWindow>
#include <QTimer>

#include "config.h"
#include "components/Components.hpp"
#include "paths/Paths.hpp"
#include "providers/AvatarProvider.hpp"
#include "providers/ImageProvider.hpp"
#include "providers/ExternalImageProvider.hpp"
#include "providers/ThumbnailProvider.hpp"
#include "translator/DefaultTranslator.hpp"
#include "utils/LinphoneUtils.hpp"
#include "utils/Utils.hpp"
#include "components/other/desktop-tools/DesktopTools.hpp"

// =============================================================================

using namespace std;

namespace {
  constexpr char DefaultLocale[] = "zh_CN";

  constexpr char LanguagePath[] = ":/languages/";

  // The main windows of Linphone desktop.
  constexpr char QmlViewMainWindow[] = "qrc:/ui/views/App/Main/MainWindow.qml";
  constexpr char QmlViewCallsWindow[] = "qrc:/ui/views/App/Calls/CallsWindow.qml";
  constexpr char QmlViewSettingsWindow[] = "qrc:/ui/views/App/Settings/SettingsWindow.qml";

  #ifdef ENABLE_UPDATE_CHECK
    constexpr int VersionUpdateCheckInterval = 86400000; // 24 hours in milliseconds.
  #endif // ifdef ENABLE_UPDATE_CHECK

  constexpr char MainQmlUri[] = "Linphone";

  constexpr char AttachVirtualWindowMethodName[] = "attachVirtualWindow";
  constexpr char AboutPath[] = "qrc:/ui/views/App/Main/Dialogs/About.qml";

  constexpr char AssistantViewName[] = "Assistant";

  #ifdef Q_OS_LINUX
    const QString AutoStartDirectory(QDir::homePath().append(QStringLiteral("/.config/autostart/")));
  #elif defined(Q_OS_MACOS)
    const QString OsascriptExecutable(QStringLiteral("osascript"));
  #else
    const QString AutoStartSettingsFilePath(
      QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")
    );
  #endif // ifdef Q_OS_LINUX
}

// -----------------------------------------------------------------------------

#ifdef Q_OS_LINUX
  static inline bool autoStartEnabled () {
    return QDir(AutoStartDirectory).exists() && QFile(AutoStartDirectory + EXECUTABLE_NAME ".desktop").exists();
  }
#elif defined(Q_OS_MACOS)
  static inline QString getMacOsBundlePath () {
    QDir dir(QCoreApplication::applicationDirPath());
    if (dir.dirName() != QLatin1String("MacOS"))
      return QString();

    dir.cdUp();
    dir.cdUp();

    QString path(dir.path());
    if (path.length() > 0 && path.right(1) == "/")
      path.chop(1);
    return path;
  }

  static inline QString getMacOsBundleName () {
    return QFileInfo(getMacOsBundlePath()).baseName();
  }

  static inline bool autoStartEnabled () {
    const QByteArray expectedWord(getMacOsBundleName().toUtf8());
    if (expectedWord.isEmpty()) {
      qInfo() << QStringLiteral("Application is not installed. Autostart unavailable.");
      return false;
    }

    QProcess process;
    process.start(OsascriptExecutable, { "-e", "tell application \"System Events\" to get the name of every login item" });
    if (!process.waitForFinished()) {
      qWarning() << QStringLiteral("Unable to execute properly: `%1` (%2).").arg(OsascriptExecutable).arg(process.errorString());
      return false;
    }

    // TODO: Move in utils?
    const QByteArray buf(process.readAll());
    for (const char *p = buf.data(), *word = p, *end = p + buf.length(); p <= end; ++p) {
      switch (*p) {
        case ' ':
        case '\r':
        case '\n':
        case '\t':
        case '\0':
          if (word != p) {
            if (!strncmp(word, expectedWord, size_t(p - word)))
              return true;
            word = p + 1;
          }
        default:
          break;
      }
    }

    return false;
  }
#else
  static inline bool autoStartEnabled () {
    return QSettings(AutoStartSettingsFilePath, QSettings::NativeFormat).value(EXECUTABLE_NAME).isValid();
  }
#endif // ifdef Q_OS_LINUX

// -----------------------------------------------------------------------------

static inline bool installLocale (App &app, QTranslator &translator, const QLocale &locale) {
  return translator.load(locale, LanguagePath) && app.installTranslator(&translator);
}

static inline string getConfigPathIfExists (const QCommandLineParser &parser) {
  QString filePath = parser.value("config");
  string configPath;
  if(!QUrl(filePath).isRelative()){
        configPath = Utils::appStringToCoreString(FileDownloader::synchronousDownload(filePath, Utils::coreStringToAppString(Paths::getConfigDirPath(false)), true));
  }
  if( configPath == "")
    configPath = Paths::getConfigFilePath(filePath, false);
  if( configPath == "" )
    configPath = Paths::getConfigFilePath("", false);
  return configPath;
}

static inline shared_ptr<linphone::Config> getConfigIfExists (const string &configPath) {
  string factoryPath(Paths::getFactoryConfigFilePath());
  if (!Paths::filePathExists(factoryPath))
    factoryPath.clear();

  return linphone::Config::newWithFactory(configPath, factoryPath);
}
bool App::setFetchConfig (QCommandLineParser *parser) {
  bool fetched = false;
  QString filePath = parser->value("fetch-config");
  if( !filePath.isEmpty()){
    if(QUrl(filePath).isRelative()){// this is a file path
        filePath = Utils::coreStringToAppString(Paths::getConfigFilePath(filePath, false));
        if(!filePath.isEmpty())
            filePath = "file://"+filePath;
    }
    if(!filePath.isEmpty()){
      auto instance = CoreManager::getInstance();
      if(instance){
        auto core = instance->getCore();
        if(core){
            filePath.replace('\\','/');
            if(core->setProvisioningUri(Utils::appStringToCoreString(filePath)) == 0){
              parser->process(cleanParserKeys(parser, QStringList("fetch-config")));// Remove this parameter from the parser
              fetched = true;
            }else
              fetched = false;
        }
      }
    }
    if(!fetched){
      qWarning() <<"Remote provisionning cannot be retrieved. Command have beend cleaned";
      createParser();
    }
  }
  return fetched;
}
// -----------------------------------------------------------------------------

App::App (int &argc, char *argv[]) :
   QApplication(argc, argv) {

  setWindowIcon(QIcon(LinphoneUtils::WindowIconPath));

  createParser();
  mParser->process(*this);

  // Initialize logger.
  shared_ptr<linphone::Config> config = getConfigIfExists(getConfigPathIfExists(*mParser));

  // List available locales.
  for (const auto &locale : QDir(LanguagePath).entryList())
    mAvailableLocales << QLocale(locale);

  // Init locale.
  mTranslator = new DefaultTranslator(this);
  mDefaultTranslator = new DefaultTranslator(this);
  initLocale(config);

  qInfo() << QStringLiteral("Starting " APPLICATION_NAME " (bin: " EXECUTABLE_NAME ")");
  qInfo() << QStringLiteral("Use locale: %1").arg(mLocale);
}

App::~App () {
  qInfo() << QStringLiteral("Destroying app...");
  delete mEngine;
  delete mParser;
}

// -----------------------------------------------------------------------------

QStringList App::cleanParserKeys(QCommandLineParser * parser, QStringList keys){
  QStringList oldArguments = parser->optionNames();
  QStringList parameters;
  parameters << "dummy";
  for(int i = 0 ; i < oldArguments.size() ; ++i){
    if( !keys.contains(oldArguments[i])){
      if( mParser->value(oldArguments[i]).isEmpty())
        parameters << "--"+oldArguments[i];
      else
        parameters << "--"+oldArguments[i]+"="+parser->value(oldArguments[i]);
    }
  }
  return parameters;
}

void App::processArguments(QHash<QString,QString> args){
  QList<QString> keys = args.keys();
  QStringList parameters = cleanParserKeys(mParser, keys);
  for(auto i = keys.begin() ; i != keys.end() ; ++i){
    parameters << "--"+(*i)+"="+args.value(*i);
  }
  mParser->process(parameters);
}

static QQuickWindow *createSubWindow (QQmlApplicationEngine *engine, const char *path) {
  qInfo() << QStringLiteral("Creating subwindow: `%1`.").arg(path);

  QQmlComponent component(engine, QUrl(path));
  if (component.isError()) {
    qWarning() << component.errors();
    abort();
  }
  qInfo() << QStringLiteral("Subwindow status: `%1`.").arg(component.status());

  QObject *object = component.create();
  Q_ASSERT(object);

  QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
  object->setParent(engine);

  return qobject_cast<QQuickWindow *>(object);
}

// -----------------------------------------------------------------------------

void App::initContentApp () {
  std::string configPath;
  shared_ptr<linphone::Config> config;
  bool mustBeIconified = false;
  bool needRestart = true;

  // Destroy qml components and linphone core if necessary.
  if (mEngine) {
    needRestart = false;
    setFetchConfig(mParser);
    setOpened(false);
    qInfo() << QStringLiteral("Restarting app...");
    delete mEngine;

    mNotifier = nullptr;
    //
    CoreManager::uninit();
    removeTranslator(mTranslator);
    removeTranslator(mDefaultTranslator);
    delete mTranslator;
    delete mDefaultTranslator;
    mTranslator = new DefaultTranslator(this);
    mDefaultTranslator = new DefaultTranslator(this);
    configPath = getConfigPathIfExists(*mParser);
    config = getConfigIfExists(configPath);
    initLocale(config);
  } else {
    configPath = getConfigPathIfExists(*mParser);
    qDebug() << mParser->value("config") << configPath.c_str();
    config = getConfigIfExists(configPath);
    // Update and download codecs.
    VideoCodecsModel::updateCodecs();
    VideoCodecsModel::downloadUpdatableCodecs(this);

    #ifndef Q_OS_MACOS
      mustBeIconified = mParser->isSet("iconified");
    #endif // ifndef Q_OS_MACOS

  }

  // Init core.
  CoreManager::init(this, Utils::coreStringToAppString(configPath));


  // Init engine content.
  mEngine = new QQmlApplicationEngine();

  // Provide `+custom` folders for custom components and `5.9` for old components.
  {
    QStringList selectors("custom");
    const QVersionNumber &version = QLibraryInfo::version();
    if (version.majorVersion() == 5 && version.minorVersion() == 9)
      selectors.push_back("5.9");
    (new QQmlFileSelector(mEngine, mEngine))->setExtraSelectors(selectors);
  }
  qInfo() << QStringLiteral("Activated selectors:") << QQmlFileSelector::get(mEngine)->selector()->allSelectors();

  // Set modules paths.
  mEngine->addImportPath(":/ui/modules");
  mEngine->addImportPath(":/ui/scripts");
  mEngine->addImportPath(":/ui/views");

  // Provide avatars/thumbnails providers.
  mEngine->addImageProvider(AvatarProvider::ProviderId, new AvatarProvider());
  mEngine->addImageProvider(ImageProvider::ProviderId, new ImageProvider());
  mEngine->addImageProvider(ExternalImageProvider::ProviderId, new ExternalImageProvider());
  mEngine->addImageProvider(ThumbnailProvider::ProviderId, new ThumbnailProvider());

  mEngine->rootContext()->setContextProperty("applicationUrl", APPLICATION_URL);

  registerTypes();
  registerSharedTypes();
  registerToolTypes();
  registerSharedToolTypes();

  // Enable notifications.
  mNotifier = new Notifier(mEngine);

  // Load main view.
  qInfo() << QStringLiteral("Loading main view...");
  mEngine->load(QUrl(QmlViewMainWindow));
  if (mEngine->rootObjects().isEmpty())
    qFatal("Unable to open main window.");

  QObject::connect(
    CoreManager::getInstance(),
    &CoreManager::coreManagerInitialized, CoreManager::getInstance(),
    [this, mustBeIconified]() mutable {
      if(CoreManager::getInstance()->started())
        openAppAfterInit(mustBeIconified);
    }
  );
}

// -----------------------------------------------------------------------------

QString App::getCommandArgument () {
  const QStringList &arguments = mParser->positionalArguments();
  return arguments.empty() ? QString("") : arguments[0];
}

// -----------------------------------------------------------------------------

QQuickWindow *App::getCallsWindow () const {
  if (CoreManager::getInstance()->getCore()->getConfig()->getInt(
    SettingsModel::UiSection, "disable_calls_window", 0
  ))
    return nullptr;

  return mCallsWindow;
}

QQuickWindow *App::getMainWindow () const {
  return qobject_cast<QQuickWindow *>(
    const_cast<QQmlApplicationEngine *>(mEngine)->rootObjects().at(0)
  );
}

QQuickWindow *App::getSettingsWindow () const {
  return mSettingsWindow;
}

// -----------------------------------------------------------------------------

void App::smartShowWindow (QQuickWindow *window) {
  if (!window)
    return;
  window->setVisible(true);
// Force show, maybe redundant with setVisible
  if(window->visibility() == QWindow::Maximized)// Avoid to change visibility mode
    window->showMaximized();
  else
    window->show();
  window->raise();// Raise ensure to get focus on Mac
  window->requestActivate();
}

// -----------------------------------------------------------------------------
bool App::hasFocus () const {
  return getMainWindow()->isActive() || (mCallsWindow && mCallsWindow->isActive());
}
// -----------------------------------------------------------------------------

void App::createParser () {
  delete mParser;

  mParser = new QCommandLineParser();
  mParser->setApplicationDescription(tr("applicationDescription"));
  mParser->addPositionalArgument("command", tr("commandLineDescription").replace("%1", APPLICATION_NAME), "[command]");
  mParser->addOptions({
    { { "h", "help" }, tr("commandLineOptionHelp") },
    { "cli-help", tr("commandLineOptionCliHelp").replace("%1", APPLICATION_NAME) },
    { { "v", "version" }, tr("commandLineOptionVersion") },
    { "config", tr("commandLineOptionConfig").replace("%1", EXECUTABLE_NAME), tr("commandLineOptionConfigArg") },
    { "fetch-config", tr("commandLineOptionFetchConfig").replace("%1", EXECUTABLE_NAME), tr("commandLineOptionFetchConfigArg") },
    { { "c", "call" }, tr("commandLineOptionCall").replace("%1", EXECUTABLE_NAME),tr("commandLineOptionCallArg") },
    #ifndef Q_OS_MACOS
      { "iconified", tr("commandLineOptionIconified") },
    #endif // ifndef Q_OS_MACOS
    { { "V", "verbose" }, tr("commandLineOptionVerbose") }
  });
}

// -----------------------------------------------------------------------------

template<typename T, T *(*function)()>
static QObject *makeSharedSingleton (QQmlEngine *, QJSEngine *) {
  QObject *object = (*function)();
  QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
  return object;
}

template<typename T, T *(*function)(void)>
static inline void registerSharedSingletonType (const char *name) {
  qmlRegisterSingletonType<T>(MainQmlUri, 1, 0, name, makeSharedSingleton<T, function>);
}

template<typename T, T *(CoreManager::*function)() const>
static QObject *makeSharedSingleton (QQmlEngine *, QJSEngine *) {
  QObject *object = (CoreManager::getInstance()->*function)();
  QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
  return object;
}

template<typename T, T *(CoreManager::*function)() const>
static inline void registerSharedSingletonType (const char *name) {
  qmlRegisterSingletonType<T>(MainQmlUri, 1, 0, name, makeSharedSingleton<T, function>);
}

template<typename T>
static inline void registerUncreatableType (const char *name) {
  qmlRegisterUncreatableType<T>(MainQmlUri, 1, 0, name, QLatin1String("Uncreatable"));
}

template<typename T>
static inline void registerSingletonType (const char *name) {
  qmlRegisterSingletonType<T>(MainQmlUri, 1, 0, name, [](QQmlEngine *engine, QJSEngine *) -> QObject *{
    return new T(engine);
  });
}

template<typename T>
static inline void registerType (const char *name) {
  qmlRegisterType<T>(MainQmlUri, 1, 0, name);
}

template<typename T>
static inline void registerToolType (const char *name) {
  qmlRegisterSingletonType<T>(name, 1, 0, name, [](QQmlEngine *engine, QJSEngine *) -> QObject *{
    return new T(engine);
  });
}

template<typename T, typename Owner, T *(Owner::*function)() const>
static QObject *makeSharedTool (QQmlEngine *, QJSEngine *) {
  QObject *object = (Owner::getInstance()->*function)();
  QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
  return object;
}

template<typename T, typename Owner, T *(Owner::*function)() const>
static inline void registerSharedToolType (const char *name) {
  qmlRegisterSingletonType<T>(name, 1, 0, name, makeSharedTool<T, Owner, function>);
}

void App::registerTypes () {
  qInfo() << QStringLiteral("Registering types...");

  qRegisterMetaType<shared_ptr<linphone::ProxyConfig>>();
  qRegisterMetaType<shared_ptr<linphone::SearchResult>>();
  qRegisterMetaType<std::list<std::shared_ptr<linphone::SearchResult> > >();

  registerType<AssistantModel>("AssistantModel");
  registerType<AuthenticationNotifier>("AuthenticationNotifier");
  registerType<CallsListProxyModel>("CallsListProxyModel");
  registerType<Camera>("Camera");
  registerType<CameraPreview>("CameraPreview");
  registerType<ConferenceHelperModel>("ConferenceHelperModel");
  registerType<ConferenceModel>("ConferenceModel");
  registerType<ContactsListProxyModel>("ContactsListProxyModel");
  registerType<ContactsImporterListProxyModel>("ContactsImporterListProxyModel");
  registerType<FileDownloader>("FileDownloader");
  registerType<FileExtractor>("FileExtractor");
  registerType<LdapProxyModel>("LdapProxyModel");
  registerType<SipAddressesProxyModel>("SipAddressesProxyModel");
  registerType<SearchSipAddressesModel>("SearchSipAddressesModel");
  
  registerType<SoundPlayer>("SoundPlayer");
  registerType<TelephoneNumbersModel>("TelephoneNumbersModel");

  registerSingletonType<AudioCodecsModel>("AudioCodecsModel");
  registerSingletonType<OwnPresenceModel>("OwnPresenceModel");
  registerSingletonType<Presence>("Presence");
  registerSingletonType<TimelineModel>("TimelineModel");
  registerSingletonType<UrlHandlers>("UrlHandlers");
  registerSingletonType<VideoCodecsModel>("VideoCodecsModel");

  registerUncreatableType<CallModel>("CallModel");
  registerUncreatableType<ConferenceHelperModel::ConferenceAddModel>("ConferenceAddModel");
  registerUncreatableType<ContactModel>("ContactModel");
  registerUncreatableType<ContactsImporterModel>("ContactsImporterModel");
  registerUncreatableType<LdapModel>("LdapModel");
  registerUncreatableType<SipAddressObserver>("SipAddressObserver");
  registerUncreatableType<VcardModel>("VcardModel");
}

void App::registerSharedTypes () {
  qInfo() << QStringLiteral("Registering shared types...");

  registerSharedSingletonType<App, &App::getInstance>("App");
  registerSharedSingletonType<CoreManager, &CoreManager::getInstance>("CoreManager");
  registerSharedSingletonType<SettingsModel, &CoreManager::getSettingsModel>("SettingsModel");
  registerSharedSingletonType<AccountSettingsModel, &CoreManager::getAccountSettingsModel>("AccountSettingsModel");
  registerSharedSingletonType<SipAddressesModel, &CoreManager::getSipAddressesModel>("SipAddressesModel");
  registerSharedSingletonType<CallsListModel, &CoreManager::getCallsListModel>("CallsListModel");
  registerSharedSingletonType<ContactsListModel, &CoreManager::getContactsListModel>("ContactsListModel");
  registerSharedSingletonType<ContactsImporterListModel, &CoreManager::getContactsImporterListModel>("ContactsImporterListModel");
  registerSharedSingletonType<LdapListModel, &CoreManager::getLdapListModel>("LdapListModel");
}

void App::registerToolTypes () {
  qInfo() << QStringLiteral("Registering tool types...");

  registerToolType<Clipboard>("Clipboard");
  registerToolType<DesktopTools>("DesktopTools");
  registerToolType<TextToSpeech>("TextToSpeech");
  registerToolType<Units>("Units");
  registerToolType<ContactsImporterPluginsManager>("ContactsImporterPluginsManager");
}

void App::registerSharedToolTypes () {
  qInfo() << QStringLiteral("Registering shared tool types...");

  registerToolType<Colors>("Colors");
//  registerSharedToolType<Colors, App, &App::getColors>("Colors");
}

// -----------------------------------------------------------------------------

void App::initLocale (const shared_ptr<linphone::Config> &config) {
  // Try to use preferred locale.
  QString locale;

  // Use english. This default translator is used if there are no found translations in others loads
  mLocale = DefaultLocale;

  if (!installLocale(*this, *mDefaultTranslator, QLocale(mLocale)))
    qFatal("Unable to install default translator.");

  if (config)
    locale = Utils::coreStringToAppString(config->getString(SettingsModel::UiSection, "locale", ""));

  if (!locale.isEmpty() && installLocale(*this, *mTranslator, QLocale(locale))) {
    mLocale = locale;
    return;
  }

  // Try to use system locale.
  QLocale sysLocale = QLocale::system();
  if (installLocale(*this, *mTranslator, sysLocale)) {
    mLocale = sysLocale.name();
    return;
  }

}

QString App::getConfigLocale () const {
  return Utils::coreStringToAppString(
    CoreManager::getInstance()->getCore()->getConfig()->getString(
      SettingsModel::UiSection, "locale", ""
    )
  );
}

void App::setConfigLocale (const QString &locale) {
  CoreManager::getInstance()->getCore()->getConfig()->setString(
    SettingsModel::UiSection, "locale", Utils::appStringToCoreString(locale)
  );

  emit configLocaleChanged(locale);
}

QString App::getLocale () const {
  return mLocale;
}

// -----------------------------------------------------------------------------

void App::openAppAfterInit (bool mustBeIconified) {
  qInfo() << QStringLiteral("Open " APPLICATION_NAME " app.");
  auto coreManager = CoreManager::getInstance();
  // Create other windows.
  mCallsWindow = createSubWindow(mEngine, QmlViewCallsWindow);
  mSettingsWindow = createSubWindow(mEngine, QmlViewSettingsWindow);
  QObject::connect(mSettingsWindow, &QWindow::visibilityChanged, this, [coreManager](QWindow::Visibility visibility) {
    if (visibility == QWindow::Hidden) {
      qInfo() << QStringLiteral("Update nat policy.");
      shared_ptr<linphone::Core> core = coreManager->getCore();
      core->setNatPolicy(core->getNatPolicy());
    }
  });

  QQuickWindow *mainWindow = getMainWindow();

  // Display Assistant if it does not exist proxy config.
  if (coreManager->getCore()->getProxyConfigList().empty())
    QMetaObject::invokeMethod(mainWindow, "setView", Q_ARG(QVariant, AssistantViewName), Q_ARG(QVariant, QString("")));

  if(setFetchConfig(mParser))
        restart();
  else{
// Launch call if wanted and clean parser
      if( mParser->isSet("call") && coreManager->isLastRemoteProvisioningGood()){
        QString sipAddress = mParser->value("call");
        mParser->parse(cleanParserKeys(mParser, QStringList("call")));// Clean call from parser
        if(coreManager->started()){
          coreManager->getCallsListModel()->launchAudioCall(sipAddress);
        }else{
          QObject * context = new QObject();
          QObject::connect(CoreManager::getInstance(), &CoreManager::coreManagerInitialized,context,
          [sipAddress,coreManager, context]() mutable {
            if(context){
              delete context;
              context = nullptr;
              coreManager->getCallsListModel()->launchAudioCall(sipAddress);
            }
          });
        }
      }
#ifndef __APPLE__
      if (!mustBeIconified)
        smartShowWindow(mainWindow);
#else
      Q_UNUSED(mustBeIconified);
      smartShowWindow(mainWindow);
#endif
      setOpened(true);
  }
}
