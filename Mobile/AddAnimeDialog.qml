import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import de.skycoder42.seasonproxer 1.0
import de.skycoder42.quickextras 2.0

AlertDialog {
	title: qsTr("Add Anime")
	property AddAnimeControl control: null

	property bool hasError: errorLabel.text !== ""

	Connections {
		target: control
		onLoadError: errorLabel.text = error
	}

	GridLayout {
		columns: 3
		columnSpacing: 20
		width: parent.width

		Label {
			text: qsTr("Proxer-ID:")
		}

		TextField {
			Layout.fillWidth: true
			placeholderText: "894"
			focus: true
			selectByMouse: true
			validator: IntValidator {
				bottom: 1
				top: 2147483647
			}

			text: control && control.id > 0 ? control.id : ""
			onEditingFinished: {
				errorLabel.text = "";
				control.id = text;
			}

			AppBarButton {
				id: retryButton
				anchors.right: parent.right
				anchors.rightMargin: (24 - size) / 2
				anchors.verticalCenter: parent.verticalCenter
				visible: hasError
				imageSource: "image://svg/icons/ic_refresh"
				text: qsTr("Retry")

				onClicked: {
					errorLabel.text = "";
					control.retry();
				}
			}
		}

		Item {
			id: previewItem
			Layout.rowSpan: 2
			Layout.preferredHeight: 100
			Layout.preferredWidth: 66

			readonly property bool showLoading: (control && control.loading) || previewImage.status == Image.Loading

			Image {
				id: previewImage
				anchors.fill: parent
				asynchronous: true
				fillMode: Image.PreserveAspectFit
				horizontalAlignment: Image.AlignHCenter
				verticalAlignment: Image.AlignVCenter
				source: control && control.id > 0 ? "https://cdn.proxer.me/cover/%1.jpg".arg(control.id) : ""
				mipmap: true
				visible: !previewItem.showLoading
			}

			BusyIndicator {
				anchors.fill: parent
				visible: previewItem.showLoading
			}
		}

		Label {
			text: qsTr("Title:")
		}

		TextField {
			id: titleField
			Layout.fillWidth: true
			readOnly: true
			selectByMouse: true
			text: control ? control.title : ""
		}

		TextField {
			id: errorLabel
			Layout.columnSpan: 3
			Layout.fillWidth: true
			visible: text != ""
			color: Material.accent
			wrapMode: Text.Wrap
			readOnly: true
			selectByMouse: true

			background: Item {}
		}
	}

	footer: DialogButtonBox {
		id: btnBox
		standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

		Component.onCompleted: {
			var btn = standardButton(DialogButtonBox.Ok);
			btn.enabled = Qt.binding(function() { return control && (hasError || control.acceptable); });
			btn.text = Qt.binding(function() { return hasError ? qsTr("Save anyway") : qsTr("Ok"); });
		}
	}

	onAccepted: control.accept(hasError);
}
