TEMPLATE = app
TARGET = Linphone-demo
DESTDIR = ../Debug


QT += quick concurrent testlib widgets svg dbus quickcontrols2 core

CONFIG += c++11

DEFINES += WIN32_LEAN_AND_MEAN

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/app/App.cpp \
        src/app/paths/Paths.cpp \
        src/app/providers/AvatarProvider.cpp \
        src/app/providers/ExternalImageProvider.cpp \
        src/app/providers/ImageProvider.cpp \
        src/app/providers/ThumbnailProvider.cpp \
        src/app/translator/DefaultTranslator.cpp \
        src/components/assistant/AssistantModel.cpp \
        src/components/authentication/AuthenticationNotifier.cpp \
        src/components/call/CallModel.cpp \
        src/components/calls/CallsListModel.cpp \
        src/components/calls/CallsListProxyModel.cpp \
        src/components/camera/Camera.cpp \
        src/components/camera/CameraPreview.cpp \
        src/components/codecs/AbstractCodecsModel.cpp \
        src/components/codecs/AudioCodecsModel.cpp \
        src/components/codecs/VideoCodecsModel.cpp \
        src/components/conference/ConferenceAddModel.cpp \
        src/components/conference/ConferenceHelperModel.cpp \
        src/components/conference/ConferenceModel.cpp \
        src/components/contact/ContactModel.cpp \
        src/components/contact/VcardModel.cpp \
        src/components/contacts/ContactsImporterListModel.cpp \
        src/components/contacts/ContactsImporterListProxyModel.cpp \
        src/components/contacts/ContactsImporterModel.cpp \
        src/components/contacts/ContactsImporterPluginsManager.cpp \
        src/components/contacts/ContactsListModel.cpp \
        src/components/contacts/ContactsListProxyModel.cpp \
        src/components/core/CoreHandlers.cpp \
        src/components/core/CoreManager.cpp \
        src/components/file/FileDownloader.cpp \
        src/components/file/FileExtractor.cpp \
        src/components/ldap/LdapListModel.cpp \
        src/components/ldap/LdapModel.cpp \
        src/components/ldap/LdapProxyModel.cpp \
        src/components/notifier/Notifier.cpp \
        src/components/other/clipboard/Clipboard.cpp \
        src/components/other/colors/Colors.cpp \
        src/components/other/text-to-speech/TextToSpeech.cpp \
        src/components/other/units/Units.cpp \
        src/components/presence/OwnPresenceModel.cpp \
        src/components/presence/Presence.cpp \
        src/components/search/SearchHandler.cpp \
        src/components/settings/AccountSettingsModel.cpp \
        src/components/settings/SettingsModel.cpp \
        src/components/sip-addresses/SearchSipAddressesModel.cpp \
        src/components/sip-addresses/SipAddressObserver.cpp \
        src/components/sip-addresses/SipAddressesModel.cpp \
        src/components/sip-addresses/SipAddressesProxyModel.cpp \
        src/components/sound-player/SoundPlayer.cpp \
        src/components/telephone-numbers/TelephoneNumbersModel.cpp \
        src/components/timeline/TimelineModel.cpp \
        src/components/url-handlers/UrlHandlers.cpp \
        src/utils/LinphoneUtils.cpp \
        src/utils/MediastreamerUtils.cpp \
        src/utils/QExifImageHeader.cpp \
        src/utils/Utils.cpp \
        src/utils/plugins/LinphonePlugin.cpp \
        src/utils/plugins/PluginDataAPI.cpp \
        src/utils/plugins/PluginNetworkHelper.cpp \
        src/utils/plugins/PluginsManager.cpp \
    src/components/other/desktop-tools/screen-saver/ScreenSaverDBus.cpp \
    src/components/other/desktop-tools/screen-saver/ScreenSaverXdg.cpp \
    src/components/other/desktop-tools/DesktopToolsLinux.cpp

INCLUDEPATH +=  $$PWD/src \
                $$PWD/sdk/linux/linphone-sdk/desktop/include \
				$$PWD/../

RESOURCES += resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	include/LinphoneApp/LinphonePlugin.hpp \
	include/LinphoneApp/PluginDataAPI.hpp \
	include/LinphoneApp/PluginNetworkHelper.hpp \
	src/app/App.hpp \
	src/app/paths/Paths.hpp \
	src/app/providers/AvatarProvider.hpp \
	src/app/providers/ExternalImageProvider.hpp \
	src/app/providers/ImageProvider.hpp \
	src/app/providers/ThumbnailProvider.hpp \
	src/app/translator/DefaultTranslator.hpp \
	src/components/assistant/AssistantModel.hpp \
	src/components/authentication/AuthenticationNotifier.hpp \
	src/components/call/CallModel.hpp \
	src/components/calls/CallsListModel.hpp \
	src/components/calls/CallsListProxyModel.hpp \
	src/components/camera/Camera.hpp \
	src/components/camera/CameraPreview.hpp \
	src/components/codecs/AbstractCodecsModel.hpp \
	src/components/codecs/AudioCodecsModel.hpp \
	src/components/codecs/VideoCodecsModel.hpp \
	src/components/conference/ConferenceAddModel.hpp \
	src/components/conference/ConferenceHelperModel.hpp \
	src/components/conference/ConferenceModel.hpp \
	src/components/contact/ContactModel.hpp \
	src/components/contact/VcardModel.hpp \
	src/components/contacts/ContactsImporterListModel.hpp \
	src/components/contacts/ContactsImporterListProxyModel.hpp \
	src/components/contacts/ContactsImporterModel.hpp \
	src/components/contacts/ContactsImporterPluginsManager.hpp \
	src/components/contacts/ContactsListModel.hpp \
	src/components/contacts/ContactsListProxyModel.hpp \
	src/components/core/CoreHandlers.hpp \
	src/components/core/CoreManager.hpp \
	src/components/file/FileDownloader.hpp \
	src/components/file/FileExtractor.hpp \
	src/components/ldap/LdapListModel.hpp \
	src/components/ldap/LdapModel.hpp \
	src/components/ldap/LdapProxyModel.hpp \
	src/components/notifier/Notifier.hpp \
	src/components/other/clipboard/Clipboard.hpp \
	src/components/other/colors/Colors.hpp \
	src/components/other/desktop-tools/DesktopTools.hpp \
	src/components/other/text-to-speech/TextToSpeech.hpp \
	src/components/other/units/Units.hpp \
	src/components/presence/OwnPresenceModel.hpp \
	src/components/presence/Presence.hpp \
	src/components/search/SearchHandler.hpp \
	src/components/settings/AccountSettingsModel.hpp \
	src/components/settings/SettingsModel.hpp \
	src/components/sip-addresses/SearchSipAddressesModel.hpp \
	src/components/sip-addresses/SipAddressObserver.hpp \
	src/components/sip-addresses/SipAddressesModel.hpp \
	src/components/sip-addresses/SipAddressesProxyModel.hpp \
	src/components/sound-player/SoundPlayer.hpp \
	src/components/telephone-numbers/TelephoneNumbersModel.hpp \
	src/components/timeline/TimelineModel.hpp \
	src/components/url-handlers/UrlHandlers.hpp \
	src/utils/LinphoneUtils.hpp \
	src/utils/MediastreamerUtils.hpp \
	src/utils/QExifImageHeader.hpp \
	src/utils/Utils.hpp \
	src/utils/plugins/PluginsManager.hpp \
    src/components/other/desktop-tools/DesktopToolsLinux.hpp \
    src/components/other/desktop-tools/screen-saver/ScreenSaverDBus.hpp \
    src/components/other/desktop-tools/screen-saver/ScreenSaverXdg.hpp

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
                                 -lvpx \
                                 -lbelcard \
                                 -lavcodec \
                                 -lavutil \
                                 -lswscale \
                                 -llime \
                                 -lgsm

TRANSLATIONS += assets\languages\en.ts \
				assets\languages\zh_CN.ts
