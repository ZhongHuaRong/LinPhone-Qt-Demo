#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFramebufferObject>
#include <memory>

#include <QQuickFramebufferObject>
#include <mediastreamer2/msogl.h>

class CallModel;

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
