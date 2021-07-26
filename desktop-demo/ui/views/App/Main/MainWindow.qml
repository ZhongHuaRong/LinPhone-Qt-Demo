import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0

import Common 1.0
import Linphone 1.0
import Linphone 1.0 as Linphone
import Utils 1.0

import App.Styles 1.0

import "Assistant"

import 'MainWindow.js' as Logic

// =============================================================================

ApplicationWindow {
  id: window
  width: 900
  height: 500

  property string _currentView
  property var _lockedInfo

  // ---------------------------------------------------------------------------
  onClosing: Qt.quit()

  // ---------------------------------------------------------------------------

  Connections {
    target: CoreManager
    onCoreManagerInitialized: mainLoader.active = true
  }

  // ---------------------------------------------------------------------------

  Loader {
    id: mainLoader

    active: false
    anchors.fill: parent

    sourceComponent: ColumnLayout {
      // Workaround to get these properties in `MainWindow.js`.
      // -----------------------------------------------------------------------

      AuthenticationNotifier {
        onAuthenticationRequested: Logic.handleAuthenticationRequested(authInfo, realm, sipAddress, userId)
      }

      // -----------------------------------------------------------------------
      // Toolbar properties.
      // -----------------------------------------------------------------------

      ToolBar {
        Layout.fillWidth: true
        Layout.preferredHeight: MainWindowStyle.toolBar.height

        background: MainWindowStyle.toolBar.background

        RowLayout {
          anchors {
            fill: parent
            leftMargin: MainWindowStyle.toolBar.leftMargin
            rightMargin: MainWindowStyle.toolBar.rightMargin
          }
          spacing: MainWindowStyle.toolBar.spacing

          AccountStatus {
            id: accountStatus

            Layout.fillHeight: parent.height
            Layout.preferredWidth: MainWindowStyle.accountStatus.width

            TooltipArea {
              text: AccountSettingsModel.sipAddress
              hoveringCursor: Qt.PointingHandCursor
            }

            onClicked:  {
                        CoreManager.forceRefreshRegisters()
                        window.attachVirtualWindow(Qt.resolvedUrl('Dialogs/ManageAccounts.qml'))
                        }
          }

          ActionButton {
            icon: 'new_conference'
            iconSize: MainWindowStyle.newConferenceSize
            visible: SettingsModel.conferenceEnabled
            onClicked:   {
                var callsWindow = Linphone.App.getCallsWindow()
                App.smartShowWindow(callsWindow)
                callsWindow.openConferenceManager()
            }
          }

          ActionButton {
            icon: 'burger_menu'
            iconSize: MainWindowStyle.menuBurgerSize
            onClicked: App.smartShowWindow(App.getSettingsWindow())
          }
        }
      }

      UseOtherSipAccount{
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
    }
  }
}
