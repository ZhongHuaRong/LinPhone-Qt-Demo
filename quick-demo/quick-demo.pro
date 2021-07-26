QT += quick
CONFIG += c++11
DESTDIR = ../Debug

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    linphone/linphonecoremanager.cpp \
    linphone/corehandlers.cpp \
    linphone/linphonesettings.cpp \
    linphone/mediastreamerutils.cpp \
    linphone/utils.cpp \
    linphone/callcore.cpp \
    linphone/accountsettings.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=  $$PWD/src \
                $$PWD/sdk/linux/linphone-sdk/desktop/include \

LIBS +=  -L$$PWD/sdk/linux/linphone-sdk/desktop/lib/ -lz \
                                 -lxml2 \
                                 -llinphone++ \
                                 -llinphone \
                                 -lbctoolbox \
                                 -lortp \
                                 -lmediastreamer \
                                 -lbelr \
                                 -lsqlite3 \
                                 -lbellesip \
                                 -lbv16 \
                                 -lbzrtp \
                                 -lmbedcrypto \
                                 -lmbedtls \
                                 -lmbedx509 \
                                 -lopenh264 \
                                 -lopus \
                                 -lortp \
                                 -lspeex \
                                 -lspeexdsp \
                                 -lsrtp2 \
                                 -lturbojpeg\
                                 -lvpx

HEADERS += \
    linphone/linphonecoremanager.h \
    linphone/corehandlers.h \
    linphone/linphonesettings.h \
    linphone/mediastreamerutils.h \
    linphone/utils.h \
    linphone/callcore.h \
    linphone/accountsettings.h
