import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.0
import de.skycoder42.QtMvvm.Quick 1.0
import de.skycoder42.anirem 1.0

AlertDialog {
	id: addAnimeDialog
	title: qsTr("Add Anime")
	property AddAnimeViewModel viewModel: null

	Connections {
		target: viewModel
		onClose: QuickPresenter.popView()
	}

	GridLayout {
		columns: 2
		columnSpacing: 20
		width: parent.width

		Label {
			text: qsTr("Proxer-ID:")
			Layout.fillWidth: true
			color: idEdit.focus ? idEdit.selectionColor : idEdit.color
			opacity: idEdit.focus ? 1 : 0.5
		}

		Item {
			id: previewItem
			Layout.rowSpan: 3
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

		TextField {
			id: idEdit
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

		Label {
			text: qsTr("Title:")
			Layout.fillWidth: true
			color: titleField.focus ? idEdit.selectionColor : idEdit.color
			opacity: titleField.focus ? 1 : 0.5
		}

		ComboBox {
			id: titleField
			Layout.fillWidth: true
			Layout.columnSpan: 2
			editable: false
			model: viewModel.nameModel
			textRole: "display"
			onCurrentTextChanged: viewModel.title = currentText

			Connections {
				target: viewModel
				onTitleChanged: titleField.currentIndex = titleField.find(title)
			}
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
