#include "camera.h"
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QThread>
#include <QTimer>
#include "linphone/linphonecoremanager.h"
#include <linphone++/core.hh>

// =============================================================================

using namespace std;

namespace {
constexpr int MaxFps = 30;
}


// =============================================================================
Camera::Camera (QQuickItem *parent) : QQuickFramebufferObject(parent) {
    // The fbo content must be y-mirrored because the ms rendering is y-inverted.
    setMirrorVertically(true);

    mRefreshTimer = new QTimer(this);
    mRefreshTimer->setInterval(1000 / MaxFps);

    QObject::connect(
                mRefreshTimer, &QTimer::timeout,
                this, &QQuickFramebufferObject::update,
                Qt::DirectConnection
                );

    mRefreshTimer->start();
}

QQuickFramebufferObject::Renderer *Camera::createRenderer () const {
    QQuickFramebufferObject::Renderer * renderer = NULL;
    if(mIsPreview){
        LinphoneCoreManager::getInstance()->getCore()->setNativePreviewWindowId(NULL);// Reset
        renderer=(QQuickFramebufferObject::Renderer *)LinphoneCoreManager::getInstance()->getCore()->getNativePreviewWindowId();
    }else{
        auto call = LinphoneCoreManager::getInstance()->getCallCore()->getCall();
        if(call){
            call->setNativeVideoWindowId(NULL);
            renderer=(QQuickFramebufferObject::Renderer *) call->getNativeVideoWindowId();
        }else{
            LinphoneCoreManager::getInstance()->getCore()->setNativeVideoWindowId(NULL);
            renderer=(QQuickFramebufferObject::Renderer *)LinphoneCoreManager::getInstance()->getCore()->getNativePreviewWindowId();
        }
    }
    if(renderer)
        return renderer;
    else
        return new SafeFramebuffer();
}

bool Camera::getIsPreview () const {
    return mIsPreview;
}

void Camera::setIsPreview (bool status) {
    if (mIsPreview != status) {
        mIsPreview = status;
        update();

        emit isPreviewChanged(status);
    }
}
