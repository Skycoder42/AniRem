import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.0
import de.skycoder42.QtMvvm.Quick 1.0
import de.skycoder42.anirem 1.0

AlertDialog {
	id: loginDialog
	title: qsTr("Login")
	property LoginViewModel viewModel: null

	Connections {
		target: viewModel
		onLoginSuccessful: QuickPresenter.popView()
	}

	Item {
		id: content
		anchors.fill: parent
		implicitWidth: contentLayout.implicitWidth
		implicitHeight: contentLayout.implicitHeight

		ColumnLayout {
			id: contentLayout
			anchors.fill: parent
			enabled: !viewModel.loading

			Label {
				text: qsTr("Username:")
				Layout.fillWidth: true
				color: userNameEdit.focus ? userNameEdit.selectionColor : userNameEdit.color
				opacity: userNameEdit.focus ? 1 : 0.5
			}

			TextField {
				id: userNameEdit
				Layout.fillWidth: true
				focus: true
				selectByMouse: true

				MvvmBinding {
					viewModel: loginDialog.viewModel
					viewModelProperty: "userName"
					view: userNameEdit
					viewProperty: "text"
					viewChangeSignal: "editingFinished()"
				}
			}

			Label {
				text: qsTr("Password:")
				Layout.fillWidth: true
				color: passwordEdit.focus ? passwordEdit.selectionColor : userNameEdit.color
				opacity: passwordEdit.focus ? 1 : 0.5
			}

			TextField {
				id: passwordEdit
				Layout.fillWidth: true
				selectByMouse: true
				echoMode: TextInput.Password

				MvvmBinding {
					viewModel: loginDialog.viewModel
					viewModelProperty: "password"
					view: passwordEdit
					viewProperty: "text"
					viewChangeSignal: "editingFinished()"
				}
			}

			Label {
				text: qsTr("Authenticaton Code (optional):")
				Layout.fillWidth: true
				color: authEdit.focus ? authEdit.selectionColor : userNameEdit.color
				opacity: authEdit.focus ? 1 : 0.5
			}

			TextField {
				id: authEdit
				Layout.fillWidth: true
				selectByMouse: true
				validator: IntValidator {
					bottom: 0
					top: 999999
				}

				MvvmBinding {
					viewModel: loginDialog.viewModel
					viewModelProperty: "authCode"
					view: authEdit
					viewProperty: "text"
					viewChangeSignal: "editingFinished()"
				}
			}

			//FEATURE disable for now
//			Switch {
//				id: storePwSwitch
//				text: qsTr("Remember my password")

//				MvvmBinding {
//					viewModel: loginDialog.viewModel
//					viewModelProperty: "storePassword"
//					view: storePwSwitch
//					viewProperty: "checked"
//				}
//			}
		}

		BusyIndicator {
			anchors.centerIn: parent
			z: 50
			visible: viewModel.loading
		}
	}

	footer: DialogButtonBox {
		id: btnBox
		standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
		enabled: !viewModel.loading
	}

	onAccepted: {
		viewModel.login();
		loginDialog.visible = true;
	}
}
