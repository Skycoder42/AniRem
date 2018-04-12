import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Controls.Universal 2.3
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Quick 1.0

ItemDelegate {
	id: delegate
	width: parent.width
	text: name

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
			text: delegate.text
		}

		Label {
			id: ratingLabel
			Layout.fillHeight: true
			horizontalAlignment: Qt.AlignRight
			verticalAlignment: Qt.AlignVCenter
			text: qsTr("%1/10 ☆").arg(rating)
		}
	}
}
