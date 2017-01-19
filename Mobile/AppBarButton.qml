import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

ToolButton {
	id: toolButton
    implicitHeight: toolButton.size
    implicitWidth: toolButton.size
	Material.foreground: "#FFFFFF"
    opacity: enabled ? 1.0 : 0.5

	property alias imageSource: image.source
    property double size: 56.0

	QtObject {
		id: p

		property bool toolTipVisible: false
	}

	contentItem: Item {
		id: imageWrapper
        implicitHeight: toolButton.size
        implicitWidth: toolButton.size

		Image {
			id: image
			anchors.centerIn: parent
			fillMode: Image.PreserveAspectFit
			horizontalAlignment: Image.AlignHCenter
			verticalAlignment: Image.AlignVCenter
			width: 24
			height: 24
		}
	}

	ToolTip {
		parent: toolButton
		Material.foreground: "#FFFFFF"
		visible: p.toolTipVisible
		text: toolButton.text
	}

	onPressAndHold: p.toolTipVisible = true
	onCanceled: p.toolTipVisible = false
	onReleased: p.toolTipVisible = false
}
