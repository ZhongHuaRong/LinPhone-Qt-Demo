import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2
import an.qt.im 1.0

Window {
    visible: true
    width: 960
    height: 720
    title: qsTr("Hello World")

    ColumnLayout {
        id: column
        anchors.fill: parent
        spacing: 10

        RowLayout {
            id: row
            spacing: 50
            Layout.fillWidth: true

            TextField {
                id: textField
                text: "\"1110\" <sip:1110@112.17.116.160:7898>"
                Layout.fillWidth: true
            }

            Button {
                id: button4
                text: qsTr("update")
                onClicked: {
                    core.accountSettings.addSipAccount({
                                                           sipAddress: textField.text,
                                                           serverAddress: "<sip:112.17.116.160:7898;transport=tcp>",
                                                           transport: "TCP",
                                                           password:"Qd2344"
                                                         })
                }
            }

            Button{
                id:button10
                text: qsTr("remove all")
                onClicked: core.accountSettings.removeProxyConfig(null)
            }
        }

        TextField {
            id: textField1
            Layout.fillWidth: true
            text: qsTr("sip:1111@112.17.116.160:7898")
        }

        RowLayout{
            spacing: 50
            Layout.fillWidth: true

            Button {
                id: button
                x: 76
                y: 220
                text: "answer"
                onClicked: core.callcore.callAccept()
            }

            Button {
                id: button1
                x: 197
                y: 220
                text: qsTr("hang up")
                onClicked: core.callcore.callTerminate()
            }

            Button {
                id: button2
                x: 324
                y: 122
                text: qsTr("audio call")
                onClicked: core.callcore.launchAudioCall(textField1.text)
            }

            Button {
                id: button2_1
                x: 324
                y: 122
                text: qsTr("video call")
                onClicked: core.callcore.launchVideoCall(textField1.text)
            }

            Button {
                id: button3
                x: 453
                y: 122
                text: qsTr("cancel")
                onClicked: core.callcore.terminateAllCalls()
            }
        }

        Text{
            id:infoText
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
