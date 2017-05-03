import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.quickextras 2.0

SwipeDelegate {
	id: delegate
	width: parent.width
	text: title
	highlighted: hasNewSeasons

	CommonStyle {
		id: style

		function highlight(high) {
			if(style.isMaterial)
				return high ? style.accent : style.foreground;
			else
				return style.foreground;
		}

		function redColor() {
			if(style.isMaterial)
				return Material.color(Material.Red);
			else if(style.isUniversal)
				return Universal.color(Universal.Red);
			else
				return "#FF0000";
		}
	}

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
			source: "image://proxer/%1".arg(id)
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
			color: style.highlight(hasNewSeasons)
			text: delegate.text
		}

		Label {
			id: countLabel
			Layout.fillHeight: true
			horizontalAlignment: Qt.AlignRight
			verticalAlignment: Qt.AlignVCenter
			color: style.highlight(hasNewSeasons)
			text: totalSeasonCount
		}
	}

	swipe.right: Rectangle {
		width: parent.height
		height: parent.height
		anchors.right: parent.right
		color: style.redColor()
		AppBarButton {
			size: parent.height
			imageSource: "image://svg/icons/ic_delete_forever"
			text: qsTr("Delete Anime")

			onClicked: animeDeleted()
		}
	}
}
