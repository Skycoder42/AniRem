import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Label {
	property string title

	width: parent.width
	font.bold: true
	font.capitalization: Font.SmallCaps
	padding: 14
	bottomPadding: 4
	text: title + qsTr(":")

	background: Rectangle {
		anchors.fill: parent
		color: Material.background

		Rectangle {
			anchors.left: parent.left
			anchors.bottom: parent.bottom
			anchors.right: parent.right
			height: 2
			color: Material.accent
		}
	}
}
