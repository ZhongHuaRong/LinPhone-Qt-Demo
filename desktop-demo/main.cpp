#include "app/AppController.hpp"
#include <qloggingcategory.h>
#ifdef QT_QML_DEBUG
#include <QQmlDebuggingEnabler>
#endif
// =============================================================================

int main (int argc, char *argv[]) {
  AppController controller(argc, argv);
#ifdef QT_QML_DEBUG
  QQmlDebuggingEnabler enabler;
#endif
  //QLoggingCategory::setFilterRules("*.debug=true;qml=false");
  App *app = controller.getApp();
  if (app->isSecondary())
  {
	  qInfo() << QStringLiteral("Running secondary app success. Kill it now.");
	  return EXIT_SUCCESS;
  }

  qInfo() << QStringLiteral("Running app...");

  int ret;
  do {
    app->initContentApp();
    ret = app->exec();
  } while (ret == App::RestartCode);
  return ret;
}
