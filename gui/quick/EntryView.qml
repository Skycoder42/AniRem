import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import de.skycoder42.QtMvvm.Core 1.0
import de.skycoder42.QtMvvm.Quick 1.0
import de.skycoder42.anirem 1.0
import "../../qml"

Page {
	id: mainView
	property EntryViewModel viewModel: null

	header: ContrastToolBar {
		id: toolbar

		ToolBarLabel {
			id: titleLabel
			anchors.fill: parent
			text: qsTr("Anime History")
		}
	}

	PresenterProgress {}

	ScrollView {
		anchors.fill: parent
		clip: true

		ListView {
			model: viewModel.model

			delegate: AnimeHistoryDelegate {
				onClicked: viewModel.addAnime(id)
			}
		}
	}
}
