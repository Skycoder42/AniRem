import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import "../../../quickextras/qml"

ItemDelegate {
	id: msgDelegate

	text: title

	contentItem: ColumnLayout {
		Label {
			id: titleLabel
			text: msgDelegate.text
			font.bold: true
			elide: Label.ElideRight
			Layout.fillWidth: true
		}

		Label {
			id: textLabel
			visible: tooltip
			text: tooltip
			wrapMode: Text.WordWrap
			Layout.fillWidth: true
		}
	}

	onClicked: showInputDialog = true
}
