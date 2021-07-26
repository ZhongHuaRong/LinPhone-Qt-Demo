import QtQuick 2.7

import Common 1.0
import Linphone 1.0
import Utils 1.0

import App.Styles 1.0

import 'ManageAccount.js' as Logic

// =============================================================================

DialogPlus {
    buttons: [
        TextButtonB {
            text: qsTr('ok')

            onClicked: exit(0)
        }
    ]

    centeredButtons: true
    objectName: '__manageAccounts'

    height: SettingsModel.rlsUriEnabled ? ManageAccountsStyle.height : ManageAccountsStyle.heightWithoutPresence
    width: ManageAccountsStyle.width

    // ---------------------------------------------------------------------------

    FormLine {
        anchors.fill: parent
        FormGroup {
            label: qsTr('selectAccountLabel')

            ScrollableListViewField {
                width: parent.width
                height: ManageAccountsStyle.accountSelector.height

                radius: 0

                ScrollableListView {
                    id: view

                    property string textRole: 'fullSipAddress' // Used by delegate.

                    anchors.fill: parent
                    model: AccountSettingsModel.accounts

                    onModelChanged: currentIndex = Utils.findIndex(AccountSettingsModel.accounts, function (account) {
                        return account.sipAddress === AccountSettingsModel.sipAddress
                    })

                    delegate: CommonItemDelegate {
                        id: item
                        container: view
                        flattenedModel: modelData
                        itemIcon: ''//Start with no error and let some time before getting status with the below timer
                        width: parent.width

                        Timer{// This timer is used to synchronize registration state by proxy, without having to deal with change signals
                            interval: 1000; running: item.visible; repeat: true
                            onTriggered:itemIcon= Logic.getItemIcon(flattenedModel)
                        }

                        ActionButton {
                            icon: 'options'
                            iconSize: 30
                            anchors.fill: parent
                            visible:false
                            //TODO handle click and jump to proxy config settings
                        }

                        onClicked: {
                            container.currentIndex = index
                            if(flattenedModel.proxyConfig)
                                AccountSettingsModel.setDefaultProxyConfig(flattenedModel.proxyConfig)
                            else
                                AccountSettingsModel.setDefaultProxyConfig()
                        }
                    }
                }
            }
        }
    }
}
