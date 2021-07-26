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

#ifndef APP_H_
#define APP_H_

#include <memory>
#include <QApplication>
#include <QDebug>

// =============================================================================

class QCommandLineParser;
class QQmlApplicationEngine;
class QQuickWindow;

namespace linphone {
  class Config;
}

class Colors;
class DefaultTranslator;
class Notifier;

class App : public QApplication {
  Q_OBJECT;

  Q_PROPERTY(QString configLocale READ getConfigLocale WRITE setConfigLocale NOTIFY configLocaleChanged);
  Q_PROPERTY(QString locale READ getLocale CONSTANT);
  Q_PROPERTY(QVariantList availableLocales READ getAvailableLocales CONSTANT);

public:
  App (int &argc, char *argv[]);
  ~App ();

  void initContentApp ();
  QStringList cleanParserKeys(QCommandLineParser * parser, QStringList keys);// Get all options from parser and remove the selected keys. Return the result that can be passed to parser process.
  void processArguments(QHash<QString,QString> args);

  QString getCommandArgument ();

  bool setFetchConfig (QCommandLineParser *parser);

  #ifdef Q_OS_MACOS
    bool event (QEvent *event) override;
  #endif // ifdef Q_OS_MACOS

  QQmlApplicationEngine *getEngine () {
    return mEngine;
  }

  Notifier *getNotifier () const {
    return mNotifier;
  }

  QQuickWindow *getMainWindow () const;

  bool hasFocus () const;

  bool isOpened () const {
    return mIsOpened;
  }

  static App *getInstance () {
    return static_cast<App *>(QApplication::instance());
  }

  static constexpr int RestartCode = 1000;

  Q_INVOKABLE void restart () {
    exit(RestartCode);
  }

  Q_INVOKABLE QQuickWindow *getCallsWindow () const;
  Q_INVOKABLE QQuickWindow *getSettingsWindow () const;

  Q_INVOKABLE static void smartShowWindow (QQuickWindow *window);

signals:
  void configLocaleChanged (const QString &locale);

  void opened (bool status);

private:
  void createParser ();

  void registerTypes ();
  void registerSharedTypes ();
  void registerToolTypes ();
  void registerSharedToolTypes ();

  void createNotifier ();

  void initLocale (const std::shared_ptr<linphone::Config> &config);

  QString getConfigLocale () const;
  void setConfigLocale (const QString &locale);

  QString getLocale () const;

  QVariantList getAvailableLocales () const {
    return mAvailableLocales;
  }

  void openAppAfterInit (bool mustBeIconified = false);

  void setOpened (bool status) {
    if (mIsOpened != status) {
      mIsOpened = status;
      emit opened(mIsOpened);
    }
  }
  QVariantList mAvailableLocales;
  QString mLocale;

  QCommandLineParser *mParser = nullptr;

  QQmlApplicationEngine *mEngine = nullptr;

  DefaultTranslator *mTranslator = nullptr;
  DefaultTranslator *mDefaultTranslator = nullptr;
  Notifier *mNotifier = nullptr;

  QQuickWindow *mCallsWindow = nullptr;
  QQuickWindow *mSettingsWindow = nullptr;

  Colors *mColors = nullptr;

  bool mIsOpened = false;
};

#endif // APP_H_
