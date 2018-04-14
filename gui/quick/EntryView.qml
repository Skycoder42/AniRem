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

		RowLayout {
			anchors.fill: parent
			spacing: 0

			ToolBarLabel {
				id: titleLabel
				Layout.fillWidth: true
				text: qsTr("%1 History").arg(viewModel.listTypes[viewModel.typeIndex])
			}

			MenuButton {
				id: menuButton
				icon.name: "view-filter"
				icon.source: "qrc:/icons/ic_filter_list.svg"
				text: qsTr("Select Mode")

				ButtonGroup {
					id: indexGroup
					exclusive: true
				}

				Repeater {
					id: typeRep
					model: viewModel.listTypes
					delegate: MenuItem {
						id: mItem
						text: modelData
						checkable: true
						checked: viewModel.typeIndex == index
						onClicked: viewModel.typeIndex = index

						Component.onCompleted: indexGroup.addButton(mItem)
					}
				}
			}
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
