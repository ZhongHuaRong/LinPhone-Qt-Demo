#include "app/AppController.hpp"
#include "components/other/desktop-tools/DesktopTools.hpp"
#include "config.h"
#include "gitversion.h"
#include <qloggingcategory.h>
#ifdef QT_QML_DEBUG
#include <QQmlDebuggingEnabler>
#endif
// =============================================================================

constexpr char DefaultFont[] = "Noto Sans";
int main (int argc, char *argv[]) {
#ifdef QT_QML_DEBUG
  QQmlDebuggingEnabler enabler;
#endif
  //QLoggingCategory::setFilterRules("*.debug=true;qml=false");
  DesktopTools::init();
  // Disable QML cache. Avoid malformed cache.
  qputenv("QML_DISABLE_DISK_CACHE", "true");
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  // Useful to share camera on Fullscreen (other context)
  QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  // Do not use APPLICATION_NAME here.
  // The EXECUTABLE_NAME will be used in qt standard paths. It's our goal.
  QCoreApplication::setApplicationName(EXECUTABLE_NAME);
  QApplication::setOrganizationDomain(EXECUTABLE_NAME);
  QCoreApplication::setApplicationVersion(LINPHONE_QT_GIT_VERSION);

  auto mApp = new App(argc, argv);
  mApp->initContentApp();

  qInfo() << QStringLiteral("Running app...");
  return mApp->exec();
}
