import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import "../../../quickextras/qml"

ListView {
	id: listView

	section.property: "group"
	section.labelPositioning: ViewSection.InlineLabels
	section.delegate: ListSection {
		title: section
	}

	delegate: ItemDelegate {
		id: delegate
		width: parent.width
		text: title
	}
}
