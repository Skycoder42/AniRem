import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import com.skycoder42.quickextras 1.0
import com.skycoder42.seasonproxer 1.0
import "../../qtmvvm/qml"
import "../../quickextras/qml"
import ".."

Page {
	id: mainView
	property MainControl control: null

	Connections {
		target: control
		onShowStatus: QuickExtras.showToast(message, true)
	}

	header: ToolBar {
		id: toolbar

		RowLayout {
			anchors.fill: parent
			spacing: 0

			Label {
				id: titleLabel
				font.pointSize: 16
				font.bold: true
				elide: Label.ElideRight
				horizontalAlignment: Qt.AlignLeft
				verticalAlignment: Qt.AlignVCenter
				Layout.fillWidth: true
				Layout.leftMargin: 10

				text: qsTr("Anime List")
			}

			AppBarButton {
				id: refreshButton
				imageSource: "image://svg/icons/ic_autorenew_white"
				text: qsTr("Refresh")
				onClicked: control.reload()
			}

			AppBarButton {
				id: moreButton
				imageSource: "image://svg/icons/ic_more_vert_white"
				text: qsTr("More")
				onClicked: moreMenu.open()

				Menu {
					id: moreMenu

					MenuItem {
						id: pasteId
						text: qsTr("Paste ID/URL")
						onClicked: control.addAnimeFromClipboard()
					}

					MenuItem {
						id: about
						text: qsTr("About")
					}

					MenuItem {
						id: aboutQt
						text: qsTr("About Qt")
						onClicked: Qt.openUrlExternally("https://www.qt.io/")
					}
				}
			}
		}
	}

	PresenterProgress {
		z: 10
	}

	ListView {
		anchors.fill: parent
		model: control ? control.animeModel : null

		ScrollBar.vertical: ScrollBar {}

		delegate: AnimeInfoDelegate {
			onAnimeDeleted: control.removeAnime(index)
			onClicked: control.showDetails(id)
			onPressAndHold: {
				QtAndroidStuff.hapticFeedback();
				control.uncheckAnime(index);
			}
		}
	}

	FloatingActionButton {
		id: addButton

		anchors.right: parent.right
		anchors.rightMargin: 10
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 10

		imageSource: "image://svg/icons/ic_add_white"
		text: qsTr("Add Anime")

		onClicked: control.addAnime()
	}

	AlertDialog {
		id: reloadingDialog
		y: (parent.height - toolbar.height - height) / 2
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
