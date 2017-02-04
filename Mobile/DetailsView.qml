import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.quickextras 1.0
import de.skycoder42.qtmvvm 1.0
import de.skycoder42.seasonproxer 1.0

Page {
	id: mainView
	property DetailsControl control: null

	header: ActionBar {
		id: toolbar
		title: qsTr("%1 Details").arg(control && control.animeInfo ? control.animeInfo.title : qsTr("Anime"))
		showMenuAsBack: true
		onMenuButtonClicked: control.close()
	}

	PresenterProgress {
		z: 10
	}

	Flickable {
		id: content
		anchors.fill: parent
		contentHeight: mainLayout.height

		ScrollBar.vertical: ScrollBar {}

		ColumnLayout {
			id: mainLayout
			anchors.left: parent.left
			anchors.right: parent.right

			Image {
				id: previewImage
				asynchronous: true
				fillMode: Image.PreserveAspectFit
				horizontalAlignment: Image.AlignHCenter
				verticalAlignment: Image.AlignVCenter
				Layout.minimumWidth: 200
				Layout.maximumWidth: 200
				Layout.minimumHeight: 277
				Layout.maximumHeight: 277
				Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
				source: "https://cdn.proxer.me/cover/%1.jpg".arg(control && control.animeInfo ? control.animeInfo.id : -1)
				mipmap: true
			}

			Label {
				id: detailsLabel
				Layout.preferredWidth: implicitWidth + 14
				Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
				wrapMode: Text.Wrap
				textFormat: Text.RichText
				text: control ? control.detailsText : ""
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
			}
		}
	}
}
