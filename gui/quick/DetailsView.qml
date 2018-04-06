import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.0
import de.skycoder42.QtMvvm.Quick 1.0
import de.skycoder42.anirem 1.0

Page {
	id: detailsView
	property DetailsViewModel viewModel: null

	header: ContrastToolBar {
		id: toolbar
		RowLayout {
			anchors.fill: parent
			spacing: 0

			ActionButton {
				text: qsTr("Go back")
				icon.name: "arrow-left"
				icon.source: "qrc:/icons/ic_arrow_back.svg"
				onClicked: QuickPresenter.popView();
			}

			ToolBarLabel {
				text: qsTr("%1 Details").arg(viewModel.animeInfo ? viewModel.animeInfo.title : qsTr("Anime"))
				Layout.fillWidth: true
			}

			ActionButton {
				id: unmarkAction
				icon.name: "dialog-ok-applyk"
				icon.source: "qrc:/icons/ic_check.svg"
				text: qsTr("Unmark new seasons")
				onClicked: viewModel.uncheckAnime()
				visible: viewModel.animeInfo ? viewModel.animeInfo.hasNewSeasons : false
			}

			ActionButton {
				id: openInBrowser
				icon.name: "internet-web-browser"
				icon.source: "qrc:/icons/ic_open_in_browser.svg"
				text: qsTr("Open relations in your browser")
				onClicked: Qt.openUrlExternally(viewModel.animeInfo.relationsUrl)
			}
		}
	}

	PresenterProgress {
		z: 10
	}

	ScrollView {
		id: content
		anchors.fill: parent
		contentHeight: mainLayout.height
		contentWidth: width
		clip: true

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
				source: viewModel && viewModel.animeInfo ? "image://proxer/%1".arg(viewModel.animeInfo.id) : ""
				mipmap: true
			}

			Label {
				id: detailsLabel
				Layout.preferredWidth: implicitWidth + 16
				Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
				wrapMode: Text.Wrap
				textFormat: Text.RichText
				text: viewModel ? viewModel.detailsText : ""
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
			}
		}
	}
}
