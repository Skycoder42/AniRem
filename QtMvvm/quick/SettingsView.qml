import QtQuick 2.8
import QtQuick.Controls 2.1
import com.skycoder42.qtmvvm 1.0
import "../../qtmvvm/qml"
import "../../qtmvvm/qml/settings"
import "../../quickextras/qml"

Page {
	id: settingsView
	property SettingsControl control: null

	header: ActionBar {
		title: qsTr("Settings")
		showMenuAsBack: true
		onMenuButtonClicked: control.close()

		TextField {
			id: searchField
			visible: false

			Component.onDestruction: visible = true
		}

		AppBarButton {
			id: searchButton
			visible: false
			imageSource: searchField.visible ? "image://svg/qtmvvm/icons/ic_close" : "image://svg/qtmvvm/icons/ic_search"
			text: qsTr("Search in settings")
			onClicked: {
				searchField.visible = !searchField.visible;
				if(searchField.visible)
					searchField.forceActiveFocus();
				else
					searchField.text = "";
			}

			Component.onDestruction: visible = true
		}

		AppBarButton {
			id: restoreButton
			visible: false
			imageSource: "image://svg/qtmvvm/icons/ic_settings_backup_restore"
			text: qsTr("Restore settings")

			Component.onDestruction: visible = true
		}
	}

	PresenterProgress {
		z: 10
	}

	SettingsUiBuilder {
		id: builder
		buildView: settingsView
		control: settingsView.control

		onInitActions: {
			searchButton.visible = allowSearch;
			restoreButton.visible = allowSearch;
		}

		onCreateView: {
			viewLoader.model = model;
			viewLoader.showSections = showSections;
			if(isOverview)
				viewLoader.source = "qrc:/qtmvvm/qml/settings/OverviewListView.qml";
			else
				console.log("TODO");
		}
	}

	Loader {
		id: viewLoader
		property var model: null
		property bool showSections: false

		onLoaded: {
			item.parent = settingsView.contentItem;
			item.model = viewLoader.model;
			item.showSections = viewLoader.showSections;
			item.anchors.fill = item.parent;
		}
	}
}
