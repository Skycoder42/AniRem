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

			Item {
				id: labelContainer
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.leftMargin: 16

				ToolBarLabel {
					id: titleLabel
					anchors.fill: parent
					leftPadding: 0
					text: qsTr("Anime List")
				}

				TextField {
					id: searchField
					horizontalAlignment: Qt.AlignLeft
					verticalAlignment: Qt.AlignVCenter
					anchors.right: parent.right
					anchors.verticalCenter: parent.verticalCenter
					height: Math.min(implicitHeight, parent.height)
					width: parent.width

					placeholderText: qsTr("Filter the animes…")
					MvvmBinding {
						viewModel: mainView.viewModel
						viewModelProperty: "filterString"
						view: searchField
						viewProperty: "text"
						type: MvvmBinding.OneWayToViewModel
					}
				}
			}

			ActionButton {
				id: searchButton
				text: qsTr("Filter the animes")
				onClicked: {
					if(toolbar.state == "title")
						toolbar.state = "search";
					else
						toolbar.state = "title";
				}
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

		states: [
			State {
				name: "title"
				PropertyChanges {
					target: searchButton
					icon.name: "search"
					icon.source: "qrc:/de/skycoder42/qtmvvm/quick/icons/ic_search.svg"
				}
				PropertyChanges {
					target: titleLabel
					visible: true
				}
				PropertyChanges {
					target: searchField
					visible: false
					width: 0
				}
				StateChangeScript {
					name: "focusScript"
					script: searchField.clear();
				}
			},
			State {
				name: "search"
				PropertyChanges {
					target: searchButton
					icon.name: "gtk-close"
					icon.source: "qrc:/de/skycoder42/qtmvvm/quick/icons/ic_close.svg"
				}
				PropertyChanges {
					target: titleLabel
					visible: false
				}
				PropertyChanges {
					target: searchField
					visible: true
					width: labelContainer.width
				}
				StateChangeScript {
					name: "focusScript"
					script: searchField.forceActiveFocus();
				}
			}
		]
		transitions: [
			Transition {
				from: "title"
				to: "search"
				SequentialAnimation {
					PropertyAnimation {
						target: searchField
						property: "visible"
						duration: 0
					}
					PropertyAnimation {
						target: searchField
						property: "width"
						duration: 250
						easing.type: Easing.InOutCubic
					}
					PropertyAnimation {
						target: titleLabel
						property: "visible"
						duration: 0
					}
				}
			},
			Transition {
				from: "search"
				to: "title"
				SequentialAnimation {
					PropertyAnimation {
						target: titleLabel
						property: "visible"
						duration: 0
					}
					PropertyAnimation {
						target: searchField
						property: "width"
						duration: 250
						easing.type: Easing.InOutCubic
					}
					PropertyAnimation {
						target: searchField
						property: "visible"
						duration: 0
					}
				}
			}
		]

		state: "title"
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

		z: 7
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.margins: 16
		checkable: true

		icon.name: checked ? "tab-close" : "list-add"
		icon.source: checked ?
						 "qrc:/de/skycoder42/qtmvvm/quick/icons/ic_close.svg" :
						 "qrc:/de/skycoder42/qtmvvm/quick/icons/ic_add.svg"
		text: qsTr("Add Anime")
	}

	SubButton {
		id: addEntryButton
		z: 3
		reference: addButton
		expanded: addButton.checked

		text: qsTr("Add Anime from your history")
		icon.name: "format-list-unordered"
		icon.source: "qrc:/icons/ic_playlist_add.svg"

		onClicked: {
			viewModel.addAnimeFromEntryList();
			addButton.checked = false;
		}
	}

	SubButton {
		id: addBlankButton
		z: 3
		reference: addEntryButton
		expanded: addButton.checked

		text: qsTr("Add Anime by it's id")
		icon.name: "tool-text"
		icon.source: "qrc:/icons/ic_text_fields.svg"

		onClicked: {
			viewModel.addAnimeBlank();
			addButton.checked = false;
		}
	}

	SubButton {
		id: addClipboardButton
		z: 3
		reference: addBlankButton
		expanded: addButton.checked

		text: qsTr("Paste ID/URL")
		icon.name: "edit-paste"
		icon.source: "qrc:/icons/ic_content_paste.svg"

		onClicked: {
			viewModel.addAnimeFromClipboard();
			addButton.checked = false;
		}
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
