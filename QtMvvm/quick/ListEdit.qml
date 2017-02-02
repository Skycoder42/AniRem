import QtQuick 2.8
import QtQuick.Controls 2.1

ListView {
	id: edit
	property var inputValue
	property alias _list_elements: edit.model

	implicitHeight: dummyDelegate.height * model.length

	ScrollBar.vertical: ScrollBar {}

	delegate: RadioDelegate {
		width: parent.width
		text: typeof modelData == "string" ? modelData : edit.model[index].name

		checked: (typeof modelData == "string" ? modelData : edit.model[index].value) == inputValue
		onClicked: inputValue = (typeof modelData == "string" ? modelData : edit.model[index].value)
	}

	ItemDelegate {
		id:dummyDelegate
		visible: false
		text: "dummy"
	}
}
