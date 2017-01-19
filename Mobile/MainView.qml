import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import "../../qtmvvm/qml"
import "../../qtandroidstuff/qml"
import ".."

Page {
	property var control: null

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
				imageSource: "qrc:/icons/ic_refresh_white.svg"
				text: qsTr("Refresh")
			}

			AppBarButton {
				id: moreButton
				imageSource: "qrc:/icons/ic_more_vert_white.svg"
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
		}
	}

	FloatingActionButton {
		id: addButton
		size: 56

		anchors.right: parent.right
		anchors.rightMargin: 10
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 10

		imageSource: "qrc:/icons/ic_add_white.svg"
		text: qsTr("Add Anime")

		onClicked: control.addAnime()
	}
}
