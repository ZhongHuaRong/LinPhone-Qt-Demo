﻿#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QString>
#include <QLocale>
#include <QImage>

#include <linphone++/address.hh>


// =============================================================================

/*
 * Define telling g++ that a 'break' statement has been deliberately omitted
 * in switch block.
 */
#ifndef UTILS_NO_BREAK
  #if defined(__GNUC__) && __GNUC__ >= 7
    #define UTILS_NO_BREAK __attribute__((fallthrough))
  #else
    #define UTILS_NO_BREAK
  #endif // if defined(__GNUC__) && __GNUC__ >= 7
#endif // ifndef UTILS_NO_BREAK

namespace Utils {
  inline QString coreStringToAppString (const std::string &str) {
    return QString::fromLocal8Bit(str.c_str(), int(str.size()));
  }

  inline std::string appStringToCoreString (const QString &str) {
    return qPrintable(str);
  }

  // Reverse function of strstr.
  char *rstrstr (const char *a, const char *b);
  // Returns the same path given in parameter if `filePath` exists.
  // Otherwise returns a safe path with a unique number before the extension.
  QString getSafeFilePath (const QString &filePath, bool *soFarSoGood = nullptr);
  std::shared_ptr<linphone::Address> getMatchingLocalAddress(std::shared_ptr<linphone::Address> p_localAddress);
  QString cleanSipAddress (const QString &sipAddress);// Return at most : sip:username@domain

  // Connect once to a member function.
  template<typename Func1, typename Func2>
  static inline QMetaObject::Connection connectOnce (
    typename QtPrivate::FunctionPointer<Func1>::Object *sender,
    Func1 signal,
    typename QtPrivate::FunctionPointer<Func2>::Object *receiver,
    Func2 slot
  ) {
    QMetaObject::Connection connection = QObject::connect(sender, signal, receiver, slot);
    QMetaObject::Connection *deleter = new QMetaObject::Connection();

    *deleter = QObject::connect(sender, signal, [connection, deleter] {
      QObject::disconnect(connection);
      QObject::disconnect(*deleter);
      delete deleter;
    });

    return connection;
  }

  // Connect once to a function.
  template<typename Func1, typename Func2>
  static inline QMetaObject::Connection connectOnce (
    typename QtPrivate::FunctionPointer<Func1>::Object *sender,
    Func1 signal,
    const QObject *receiver,
    Func2 slot
  ) {
    QMetaObject::Connection connection = QObject::connect(sender, signal, receiver, slot);
    QMetaObject::Connection *deleter = new QMetaObject::Connection();

    *deleter = QObject::connect(sender, signal, [connection, deleter] {
      QObject::disconnect(connection);
      QObject::disconnect(*deleter);
      delete deleter;
    });

    return connection;
  }
  QString getCountryName(const QLocale::Country& country);
  void copyDir(QString from, QString to);// Copy a folder recursively without erasing old file
}


#endif // UTILS_H
