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
		onShowStatus: console.log(message) //TODO show toast!
	}

	header: ContrastToolBar {
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

	ListView {
		anchors.fill: parent

		model: viewModel.animeModel // TODO sort in viewmodel

		ScrollBar.vertical: ScrollBar {}

		delegate: AnimeInfoDelegate {
			onAnimeDeleted: viewModel.removeAnime(id)
			onClicked: viewModel.showDetails(id)
			onPressAndHold: {
				QuickPresenter.hapticLongPress();
				viewModel.itemAction(id);
			}
		}
	}
}
