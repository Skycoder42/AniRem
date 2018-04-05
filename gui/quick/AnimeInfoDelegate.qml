import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Controls.Universal 2.3
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Quick 1.0

SwipeDelegate {
	id: delegate
	width: parent.width
	text: title
	highlighted: hasNewSeasons === "true"

	signal animeDeleted

	function redColor() {
		if(QuickPresenter.currentStyle === "Material")
			return Material.color(Material.Red);
		else if(QuickPresenter.currentStyle === "Universal")
			return Universal.color(Universal.Red);
		else
			return "#FF0000";
	}

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
			font.bold: delegate.highlighted
			Material.foreground: delegate.highlighted ? Material.accent : delegate.Material.foreground
			text: delegate.text
		}

		Label {
			id: countLabel
			Layout.fillHeight: true
			horizontalAlignment: Qt.AlignRight
			verticalAlignment: Qt.AlignVCenter
			Material.foreground: delegate.highlighted ? Material.accent : delegate.Material.foreground
			text: totalSeasonCount
		}
	}

	swipe.right: Rectangle {
		anchors.fill: parent
		color: redColor()

		MouseArea {
			anchors.fill: parent

			ActionButton {
				anchors.centerIn: parent
				icon.name: "user-trash"
				icon.source: "qrc:/icons/ic_delete_forever.svg"
				text: qsTr("Delete Anime")

				Material.foreground: "white"
				Universal.foreground: "white"

				onClicked: {
					delegate.swipe.close();
					animeDeleted()
				}
			}
		}
	}
}
