import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "qrc:///qtmvvm/qml"
import ".."

Page {
	property var control: null

	header: ToolBar {
		id: toolbar

		RowLayout {
			anchors.fill: parent
			Label {
				id: titleLabel
				font.pointSize: 16
				font.bold: true
				elide: Label.ElideRight
				horizontalAlignment: Qt.AlignLeft
				verticalAlignment: Qt.AlignVCenter
				Layout.fillWidth: true
				Layout.leftMargin: 10

				text: qsTr("Anime List")
			}

			AppBarButton {
				id: moreButton
				imageSource: "qrc:/icons/more.png"
				text: qsTr("More")
			}
		}
	}

	PresenterProgress {
		z: 10
	}

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
