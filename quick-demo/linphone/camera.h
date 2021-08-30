#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFramebufferObject>
#include <memory>

#include <QQuickFramebufferObject>
#include <mediastreamer2/msogl.h>

class CallModel;

/**
 * @brief The Camera class
 * 摄像头,用于显示对方画面和自己画面
 * 
 * isPreview为true时显示自己摄像头,false时显示对方摄像头
 * @note 该对象的生命周期必须在CallCore的call生命周期内
 * @warning 需要注意一个问题,在通话未开始之前生成该对象实例,则不会显示画面,且在通话接通后也不会有画面
 *          要在call的生命周期内删除,否则会崩溃
 */
class Camera : public QQuickFramebufferObject {
  Q_OBJECT
  Q_PROPERTY(bool isPreview READ getIsPreview WRITE setIsPreview NOTIFY isPreviewChanged)
public:
  Camera (QQuickItem *parent = Q_NULLPTR);

  QQuickFramebufferObject::Renderer *createRenderer () const override;

signals:
  void isPreviewChanged (bool isPreview);

private:

  bool getIsPreview () const;
  void setIsPreview (bool status);

  bool mIsPreview = false;
  QTimer *mRefreshTimer = nullptr;
};

class SafeFramebuffer : public QQuickFramebufferObject::Renderer{
public:
    SafeFramebuffer(){}
    QOpenGLFramebufferObject *createFramebufferObject (const QSize &size) override{
        return new QOpenGLFramebufferObject(size);
    }
    void render () override{}
    void synchronize (QQuickFramebufferObject *item) override{}
};


#endif // CAMERA_H
