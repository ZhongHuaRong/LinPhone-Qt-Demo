#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "linphone/linphonecoremanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication app(argc, argv);

    LinphoneCoreManager::init(nullptr,app.applicationDirPath() + "/info");

    QQmlApplicationEngine engine;
    auto rootContext = engine.rootContext();
    rootContext->setContextProperty("core", LinphoneCoreManager::getInstance());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
