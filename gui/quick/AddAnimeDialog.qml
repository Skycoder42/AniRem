import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.0
import de.skycoder42.QtMvvm.Quick 1.0
import de.skycoder42.anirem 1.0

AlertDialog {
	title: qsTr("Add Anime")
	property AddAnimeViewModel viewModel: null

	Connections {
		target: viewModel
		onClose: QuickPresenter.popView()
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

			text: viewModel.id > 0 ? viewModel.id : ""
			onEditingFinished: viewModel.id = text;
		}

		Item {
			id: previewItem
			Layout.rowSpan: 2
			Layout.preferredHeight: 100
			Layout.preferredWidth: 66

			readonly property bool showLoading: viewModel.loading || previewImage.status == Image.Loading

			Image {
				id: previewImage
				anchors.fill: parent
				asynchronous: true
				fillMode: Image.PreserveAspectFit
				horizontalAlignment: Image.AlignHCenter
				verticalAlignment: Image.AlignVCenter
				source: viewModel.id > 0 ? "image://proxer/%1".arg(viewModel.id) : ""
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
			text: viewModel.title
		}
	}

	footer: DialogButtonBox {
		id: btnBox
		standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

		Component.onCompleted: {
			var btn = standardButton(DialogButtonBox.Ok);
			btn.enabled = Qt.binding(function() { return viewModel.acceptable; });
		}
	}

	onAccepted: viewModel.accept();
}
