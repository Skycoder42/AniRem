import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import "../../qtandroidstuff/qml"

AlertDialog {
	title: qsTr("Add Anime")
	property var control: null
	width: 350

	GridLayout {
		columns: 3
		columnSpacing: 20
		width: parent.width

		Label {
			text: qsTr("Id:")
		}

		TextField {
			Layout.fillWidth: true
			placeholderText: "894"
			focus: true
			validator: IntValidator {
				bottom: 1
				top: 2147483647
			}

			text: control && control.id > 0 ? control.id : ""
			onEditingFinished: control.id = text
		}

		Image {
			id: previewImage
			Layout.rowSpan: 2
			Layout.preferredHeight: 100
			Layout.preferredWidth: 66
			asynchronous: true
			fillMode: Image.PreserveAspectFit
			horizontalAlignment: Image.AlignHCenter
			verticalAlignment: Image.AlignVCenter
			source: control && control.id > 0 ? "https://cdn.proxer.me/cover/%1.jpg".arg(control.id) : ""
		}

		Label {
			text: qsTr("Title:")
		}

		TextField {
			Layout.fillWidth: true
			readOnly: true
			text: control ? control.title : ""
		}
	}

	standardButtons: Dialog.Ok | Dialog.Cancel
}
