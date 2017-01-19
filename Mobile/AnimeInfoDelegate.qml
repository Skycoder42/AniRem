import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import "../qtandroidstuff/qml"

SwipeDelegate {
	id: delegate
	width: parent.width
	text: title
	highlighted: hasNewSeasons

	signal animeDeleted

	contentItem: RowLayout {
		Image {
			id: previewImage
			sourceSize: Qt.size(42, 42)
			asynchronous: true
			fillMode: Image.PreserveAspectFit
			horizontalAlignment: Image.AlignHCenter
			verticalAlignment: Image.AlignVCenter
			Layout.preferredWidth: 42
			source: "https://cdn.proxer.me/cover/%1.jpg".arg(id)
		}

		Label {
			id:titleLabel
			elide: Label.ElideRight
			horizontalAlignment: Qt.AlignLeft
			verticalAlignment: Qt.AlignVCenter
			Layout.fillWidth: true
			Layout.fillHeight: true
			text: delegate.text
		}
	}

	swipe.right: Rectangle {
		width: parent.height
		height: parent.height
		anchors.right: parent.right
		color: Material.color(Material.Red)

		AppBarButton {
			size: parent.height
			imageSource: "qrc:/icons/ic_delete_forever_white.svg"
			text: qsTr("Delete Anime")

			onClicked: animeDeleted()
		}
	}
}
