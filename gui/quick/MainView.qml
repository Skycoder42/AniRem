import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.0
import de.skycoder42.QtMvvm.Quick 1.0
import de.skycoder42.anirem 1.0
import "../../qml"

Page {
	id: mainView
	property MainViewModel viewModel: null

	Connections {
		target: viewModel
		onShowStatus: QmlToast.toast(message)
	}

	function closeAction() {
		if(reloadingDialog.visible) {
			return true;
		} else
			return false;
	}

	header: ContrastToolBar {
		id: toolbar
		RowLayout {
			anchors.fill: parent
			spacing: 0

			ToolBarLabel {
				text: qsTr("Anime List")
				Layout.fillWidth: true
			}

			ActionButton {
				id: refreshButton
				icon.source: "qrc:/icons/ic_autorenew.svg"
				icon.name: "view-refresh"
				text: qsTr("Check for new Seasons")
				onClicked: viewModel.reload()
			}

			MenuButton {
				MenuItem {
					id: pasteId
					text: qsTr("Paste ID/URL")
					onClicked: viewModel.addAnimeFromClipboard()
				}

				MenuSeparator {}

				MenuItem {
					id: settings
					text: qsTr("Settings")
					onClicked: viewModel.showSettings()
				}

				MenuItem {
					id: sync
					text: qsTr("Synchronization")
					onClicked: viewModel.showSync()
				}

				MenuSeparator {}

				MenuItem {
					id: about
					text: qsTr("About")
					onClicked: viewModel.showAbout()
				}

				MenuItem {
					id: unblockCaptcha
					text: qsTr("Unblock IP Captcha")
					onClicked: viewModel.showCaptcha()
				}
			}
		}
	}

	PresenterProgress {}

	ScrollView {
		anchors.fill: parent
		clip: true

		ListView {
			model: viewModel.sortedModel

			delegate: AnimeInfoDelegate {
				onAnimeDeleted: viewModel.removeAnime(id)
				onClicked: viewModel.showDetails(id)
				onPressAndHold: {
					QuickPresenter.hapticLongPress();
					viewModel.itemAction(id);
				}
			}

			footer: Item {
				height: addButton.height + addButton.anchors.bottomMargin + addButton.anchors.topMargin
			}
		}
	}

	RoundActionButton {
		id: addButton

		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.margins: 16

		icon.name: "gtk-add"
		icon.source: "qrc:/icons/ic_add.svg"
		text: qsTr("Add Anime")

		onClicked: viewModel.addAnime()
	}

	AlertDialog {
		id: reloadingDialog
		title: qsTr("Checking for new seasons…")
		closePolicy: Popup.NoAutoClose
		visible: viewModel.reloadingAnimes
		extraHeight: toolbar.height

		ProgressBar {
			anchors.left: parent.left
			anchors.right: parent.right
			id: reloadingProgress
			to: 0.0
			indeterminate: to == 0.0

			Connections {
				target: viewModel
				onSetProgress: {
					reloadingProgress.to = max;
					reloadingProgress.value = value;
				}
			}
		}
	}
}
