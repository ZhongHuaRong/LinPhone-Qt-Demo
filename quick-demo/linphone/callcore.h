#ifndef CALLCORE_H
#define CALLCORE_H

#include <linphone++/linphone.hh>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QVariantList>

/**
 * @brief The CallCore class
 * 该类负责通话相关操作
 * 
 * 拨打,接听和通话状态
 */
class CallCore : public QObject
{
    Q_OBJECT
public:
    enum CallState {
        CallStateUnknown,			//未知状态
        CallStateReadying,			//通话准备中(连接服务)
        CallStatePausing,			//通话暂停
        CallStateRunning,			//正在通话
        CallStateEnd,				//通话结束
    };
    Q_ENUMS(CallState)
public:
    Q_PROPERTY(std::shared_ptr<linphone::Call> call READ getCall NOTIFY callChanged)
    Q_PROPERTY(CallState callState READ getCallState NOTIFY callStateChanged)
    Q_PROPERTY(bool microMuted READ microMuted WRITE setMicroMuted NOTIFY microMutedChanged)
    Q_PROPERTY(float playerVolume READ playerVolume WRITE setPlayerVolume NOTIFY playerVolumeChanged)
    
    Q_PROPERTY(QStringList videoDevices READ getVideoDevices NOTIFY videoDevicesChanged)
    Q_PROPERTY(int videoDevice READ getVideoDevice WRITE setVideoDevice NOTIFY videoDeviceChanged)
	
	Q_PROPERTY(QStringList playbackDevices READ getPlaybackDevices NOTIFY playbackDevicesChanged)
    Q_PROPERTY(int playbackDevice READ getPlaybackDevice WRITE setPlaybackDevice NOTIFY playbackDeviceChanged)

public:
    explicit CallCore(QObject *parent = nullptr);
    ~CallCore();

    Q_INVOKABLE void launchAudioCall (const QString &sipAddress, const QHash<QString, QString> &headers = {}) const;
    Q_INVOKABLE void launchVideoCall (const QString &sipAddress) const;

    Q_INVOKABLE void takeSnapshot ();
    Q_INVOKABLE void enableCamera(bool enabled);

    Q_INVOKABLE void terminateAllCalls () const;
    Q_INVOKABLE void terminateCall (const QString& sipAddress) const;

    Q_INVOKABLE void callAccept();
    Q_INVOKABLE void callTerminate();
    
    Q_INVOKABLE void reloadCamera();
	Q_INVOKABLE void reloadSoundDevice();
	Q_INVOKABLE void reloadPlayerVolume();


    inline std::shared_ptr<linphone::Call> getCall() const{
        return currentCall;
    }

    inline bool microMuted() const{
        return m_microMuted;
    }
    
    inline float playerVolume() const{
        return m_playerVolume;
    }
    
    inline QStringList getVideoDevices() const{
        return m_videoDevices;
    }
    
    inline int getVideoDevice() const{
        return m_videoDevice;
    }
    
	inline QStringList getPlaybackDevices() const{
		return m_playbackDevices;
	}
	
	inline int getPlaybackDevice() const{
        return m_playbackDevice;
    }
public slots:
    void setCall(std::shared_ptr<linphone::Call> c);

    inline void setCallState(CallState state){
        if(m_callstate == state)
            return;
        m_callstate = state;
        emit callStateChanged();
    }
    void setMicroMuted(bool microMuted);
    void setPlayerVolume(float playerVolume);
    void setVideoDevice(int videoDevice);
	void setPlaybackDevice(int playbackDevice);

    void dealCallChanged();
signals:
    void callStateChanged();
    void callChanged();
    void microMutedChanged(bool microMuted);
    void playerVolumeChanged(float playerVolume);
    void videoDevicesChanged(QStringList devices);
    void videoDeviceChanged(int device);
	
	void playbackDevicesChanged(QStringList devices);
    void playbackDeviceChanged(int device);
private slots:
    void handleCallStateChanged (const std::shared_ptr<linphone::Call> &call, linphone::Call::State state);
private:
    inline CallState getCallState() const {
        return m_callstate;
    }
	
	CallCore::CallState mapLinphoneCallStateToUi (linphone::Call::State state);

    QString generateSavedFilename () const;
    static QString generateSavedFilename (const QString &from, const QString &to);

    std::shared_ptr<linphone::Call> currentCall;
    CallState m_callstate{CallStateUnknown};
    bool m_microMuted{false};
    float m_playerVolume{0.f};
    QStringList m_videoDevices;
    int m_videoDevice{0};
	QStringList m_playbackDevices;
    int m_playbackDevice{0};
	bool mPausedByRemote{false};
};

#endif // CALLCORE_H
