#ifndef CALLCORE_H
#define CALLCORE_H

#include <linphone++/linphone.hh>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QVariantList>

class CallCore : public QObject
{
    Q_OBJECT
public:
    explicit CallCore(QObject *parent = nullptr);
    ~CallCore();

    Q_INVOKABLE void launchAudioCall (const QString &sipAddress, const QHash<QString, QString> &headers = {}) const;
    Q_INVOKABLE void launchVideoCall (const QString &sipAddress) const;

    Q_INVOKABLE int getRunningCallsNumber () const;

    Q_INVOKABLE void takeSnapshot ();
    Q_INVOKABLE void enableCamera(bool enabled);

    Q_INVOKABLE void terminateAllCalls () const;
    Q_INVOKABLE void terminateCall (const QString& sipAddress) const;

    Q_INVOKABLE void callAccept();
    Q_INVOKABLE void callTerminate();
private slots:
    void handleCallStateChanged (const std::shared_ptr<linphone::Call> &call, linphone::Call::State state);
private:
    QString generateSavedFilename () const;
    static QString generateSavedFilename (const QString &from, const QString &to);
};

#endif // CALLCORE_H
