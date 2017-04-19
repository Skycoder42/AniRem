import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.quickextras 2.0
import de.skycoder42.androidutils 1.0
import de.skycoder42.seasonproxer 1.0
import de.skycoder42.qtmvvm.quick 1.0
import ".."

Page {
	id: mainView
	property MainControl control: null

	Connections {
		target: control
		onShowStatus: AndroidUtils.showToast(message, true)
	}

	header: ActionBar {
		id: toolbar
		title: qsTr("Anime List")
		showMenuButton: false

		AppBarButton {
			id: refreshButton
			imageSource: "image://svg/icons/ic_autorenew"
			text: qsTr("Refresh")
			onClicked: control.reload()
		}

		moreMenu: Menu {
			id: moreMenu

			MenuItem {
				id: pasteId
				text: qsTr("Paste ID/URL")
				onClicked: control.addAnimeFromClipboard()
			}

			MenuItem {
				id: settings
				text: qsTr("Settings")
				onClicked: control.showSettings()
			}

			MenuItem {
				id: about
				text: qsTr("About")
				onClicked: control.showAbout()
			}

			MenuItem {
				id: resync
				text: qsTr("Resync data")
				onClicked: control.resyncData()
			}
		}
	}

	PresenterProgress {
		z: 10
	}

	ListView {
		anchors.fill: parent

		SortFilterProxyModel {
			id: sortModel
			sourceModel: control ? control.animeModel : null
			sortRoleName: control ? "id" : ""
		}

		model: sortModel

		ScrollBar.vertical: ScrollBar {}

		delegate: AnimeInfoDelegate {
			onAnimeDeleted: control.removeAnime(id)
			onClicked: control.showDetails(id)
			onPressAndHold: {
				QuickExtras.hapticLongPress();
				control.itemAction(id);
			}
		}
	}

	FloatingActionButton {
		id: addButton

		anchors.right: parent.right
		anchors.rightMargin: 10
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 10

		imageSource: "image://svg/icons/ic_add"
		text: qsTr("Add Anime")

		onClicked: control.addAnime()
	}

	AlertDialog {
		id: reloadingDialog
		extraHeight: toolbar.height
		title: qsTr("Checking for new seasons…")
		closePolicy: Popup.NoAutoClose
		visible: control && control.reloadingAnimes

		ProgressBar {
			anchors.left: parent.left
			anchors.right: parent.right
			id: reloadingProgress
			to: 0.0
			indeterminate: to == 0.0

			Connections {
				target: control
				onSetProgress: {
					reloadingProgress.to = max;
					reloadingProgress.value = value;
				}
			}
		}
	}
}
