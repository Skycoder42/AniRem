import QtQuick 2.7
import QtQuick.Controls 2.0

Page {
	property var control: null

	ListView {
		anchors.fill: parent
		model: control ? control.animeModel : null

		ScrollBar.vertical: ScrollBar {}

		delegate: ItemDelegate {
			width: parent.width
			text: title
		}
	}
}
