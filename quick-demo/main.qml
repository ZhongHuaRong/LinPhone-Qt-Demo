import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.2
import an.qt.im 1.0

Window {
    visible: true
    width: 1000
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
                text: qsTr("insert")
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

        RowLayout {
            id: rowLayout
            spacing: 50
            Layout.fillWidth: true

            Button {
                id: button5
                text: qsTr("enable video")
                checkable: true
                onClicked: core.callcore.enableCamera(button5.checked)

            }
			
			Button {
                id: button9
                text: qsTr("microMuted")
                checkable: true
                onClicked: core.callcore.microMuted = button9.checked

            }

            Button {
                id: button6
                text: qsTr("screenshot")
                onClicked: core.callcore.takeSnapshot()
            }
        }
		
		ColumnLayout{
			RowLayout{
				Text{
					text:qsTr("call state:")
				}
				
				
				Text{
					text:{
						switch(core.callcore.callState){
						case CallCore.CallStateUnknown:
							return qsTr("未知状态")
						case CallCore.CallStateReadying:
							return qsTr("通话准备中(连接服务)")
						case CallCore.CallStatePausing:
							return qsTr("通话暂停")
						case CallCore.CallStateRunning:
							return qsTr("正在通话")
						case CallCore.CallStateEnd:
							return qsTr("通话结束")
						}
					}
				}
				
				Text{
					text: qsTr("playerVolume")
				}
				
				Slider{
					id:slider
					from:0.0
					to:1.0
					onMoved: core.callcore.playerVolume = value
					
					Binding{
						target: slider
						property: "value"
						when:!slider.pressed
						value: core.callcore.playerVolume
					}
				}
			}
			
			RowLayout{
				Text{
					text:qsTr("account state:")
				}
				
				Text{
					id:accountText
					text:{
						switch(core.accountSettings.registrationState){
						case AccountSettings.RegistrationStateInProgress:
							return qsTr("登陆中")
						case AccountSettings.RegistrationStateRegistered:
							return qsTr("已登录")
						case AccountSettings.RegistrationStateNotRegistered:
							return qsTr("未登录")
						case AccountSettings.RegistrationStateNoProxy:
							return qsTr("未设置账号")
						default:
							return "unknown"
						}
					}
				}
			}
			
			RowLayout{
				
				Text {
					text: qsTr("camera")
				}
				
				ComboBox{
					Layout.fillWidth: true
					model:core.callcore.videoDevices
					onCurrentIndexChanged: core.callcore.videoDevice = currentIndex
					
					Component.onCompleted: core.callcore.reloadCamera()
				}
			}
			
			RowLayout{
				
				Text {
					text: qsTr("playout")
				}
				
				ComboBox{
					Layout.fillWidth: true
					model:core.callcore.playbackDevices
					onCurrentIndexChanged: core.callcore.playbackDevice = currentIndex
					
					Component.onCompleted: core.callcore.reloadSoundDevice()
				}
			}
		}

        Rectangle{
            width: 1000
            height: 500
            color: "#000000"
            Loader{
                id:loader1
                x:0
                y:0
                width: 500
                height: 500
                sourceComponent:camera
                active:core.callcore.callState == CallCore.CallStateRunning
            }
            Loader{
                id:loader2
                x:500
                y:0
                width: 500
                height: 500
                sourceComponent:camera
                active:loader1.active
                onLoaded: item.isPreview = true
            }
        }


        Component{
            id:camera

            Camera{
                width: 300
                height: 300
            }

        }

    }
}
