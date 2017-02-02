import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import de.skycoder42.quickextras 1.0

SwipeDelegate {
	id: delegate
	width: parent.width
	text: title
	highlighted: hasNewSeasons

	signal animeDeleted

	contentItem: RowLayout {
		Image {
			id: previewImage
			asynchronous: true
			fillMode: Image.PreserveAspectFit
			horizontalAlignment: Image.AlignHCenter
			verticalAlignment: Image.AlignVCenter
			Layout.minimumWidth: 42
			Layout.maximumWidth: 42
			Layout.minimumHeight: 42
			Layout.maximumHeight: 42
			source: "https://cdn.proxer.me/cover/%1.jpg".arg(id)
			mipmap: true
		}

		Label {
			id: titleLabel
			elide: Label.ElideRight
			horizontalAlignment: Qt.AlignLeft
			verticalAlignment: Qt.AlignVCenter
			Layout.fillWidth: true
			Layout.fillHeight: true
			font.bold: hasNewSeasons
			color: hasNewSeasons ? Material.accent : Material.foreground
			text: delegate.text
		}

		Label {
			id: countLabel
			Layout.fillHeight: true
			horizontalAlignment: Qt.AlignRight
			verticalAlignment: Qt.AlignVCenter
			color: hasNewSeasons ? Material.accent : Material.foreground
			text: totalSeasonCount
		}
	}

	swipe.right: Rectangle {
		width: parent.height
		height: parent.height
		anchors.right: parent.right
		color: Material.color(Material.Red)

		AppBarButton {
			size: parent.height
			imageSource: "image://svg/icons/ic_delete_forever"
			text: qsTr("Delete Anime")

			onClicked: animeDeleted()
		}
	}
}
