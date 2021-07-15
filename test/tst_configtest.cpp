#include <QtTest>
#include <QDebug>
#include <QCommandLineParser>

#include "../desktop-demo/src/config.h"
#include "linphone++/linphone.hh"
#include <memory>
// add necessary includes here

using namespace std;

static inline shared_ptr<linphone::Config> getConfigIfExists (const string &configPath) {
  string factoryPath(qApp->applicationDirPath().toStdString());
  return linphone::Config::newWithFactory(configPath, factoryPath);
}

class ConfigTest : public QObject
{
	Q_OBJECT
	
public:
	ConfigTest();
	~ConfigTest();
	
private slots:
	void test_case1();
	
};

ConfigTest::ConfigTest()
{
	
}

ConfigTest::~ConfigTest()
{
	
}

void ConfigTest::test_case1()
{
	auto config = linphone::Config::newFromBuffer("buffer");
	qInfo() << QDir::currentPath();
	QVERIFY(config);
	config->setString("buffer","test","123");
	qInfo() << QString::fromStdString(config->getString("buffer","test",""));
	QVERIFY(config->getString("buffer","test","") == "123");
}

QTEST_APPLESS_MAIN(ConfigTest)

#include "tst_configtest.moc"
