import QtQuick 2.8
import QtQuick.Controls 2.1
import com.skycoder42.qtmvvm 1.0
import "../../qtmvvm/qml"
import "../../qtmvvm/qml/settings"
import "../../quickextras/qml"

Page {
	id: settingsView
	property SettingsControl control: null

	function closeAction() {
		return settingsStack.closeAction();
	}

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

	StackView {
		id: settingsStack
		anchors.fill: parent

		function closeAction() {
			if(settingsStack.depth <= 1)
				return true;
			else {
				var item = settingsStack.pop();
				return false;
			}
		}
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
			if(isOverview) {
				settingsStack.push("qrc:/qtmvvm/qml/settings/OverviewListView.qml", {
									   "model": model,
									   "showSections": showSections
								   });
			} else {
				settingsStack.push("qrc:/qtmvvm/qml/settings/SectionListView.qml", {
									   "model": model
								   });
			}
		}
	}
}
