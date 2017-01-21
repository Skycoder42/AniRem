import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import com.skycoder42.qtmvvm 1.0

Dialog {
	id: messageBox
	visible: false

	property string titleText
	property int iconType
	property alias message: contentLabel.text
	property string positiveText: ""
	property string negativeText: ""
	property string neutralText: ""
	property alias messageContentUrl: content.source
	property alias messageContentItem: content.item

	property MessageResult messageResult: null

	function showMessageBox(result, type, title, text, positiveText, negativeText, neutralText, inputUrl) {
		if(visible)
			reject();
		messageBox.messageResult = result;
		messageBox.iconType = type;
		messageBox.title = title;
		messageBox.titleText = title;
		result.setCloseTarget(messageBox, messageBox.close);
		messageBox.message = text;
		messageBox.positiveText = positiveText;
		messageBox.negativeText = negativeText;
		messageBox.neutralText = neutralText;
		messageBox.messageContentUrl = inputUrl;
		messageBox.open();
	}

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2//TODO include input method heigth into calc
	implicitWidth: parent ? Math.min(300, parent.width - 56) : 300
	modal: true
	focus: true
	closePolicy: Popup.CloseOnEscape

	onOpened: {
		var base = "qrc:/qtmvvm/icons/ic_%2_%1.svg".arg(Material.theme == Material.Dark ? "white" : "black");
		switch(iconType) {
		case QuickPresenter.Information:
		case QuickPresenter.Input:
			base = base.arg("info");
			break;
		case QuickPresenter.Question:
			base = base.arg("help");
			break;
		case QuickPresenter.Warning:
			base = base.arg("warning");
			break;
		case QuickPresenter.Critical:
			base = base.arg("error");
			break;
		}

		messageBox.title = "<img src=\"%1\" width=\"24\" height=\"24\" align=\"middle\">&nbsp;&nbsp;%2"
								.arg(base)
								.arg(title);
	}
	onClosed: messageResult = null

	contentItem: ColumnLayout {
		Label {
			id: contentLabel
			visible: text != ""

			wrapMode: Text.Wrap
			Layout.preferredWidth: messageBox.implicitWidth
			Layout.fillWidth: true
		}

		Loader {
			id: content

			Layout.preferredWidth: content.item ? content.item.implicitWidth : 0
			Layout.maximumWidth: messageBox.width
			Layout.fillWidth: true
		}
	}

	onAccepted: {
		if(messageResult)
			messageResult.complete(MessageResult.PositiveResult, messageContentItem ? messageContentItem.value : undefined)
	}

	onRejected:  {
		if(messageResult)
			messageResult.complete(MessageResult.NegativeResult, undefined)
	}

	footer: DialogButtonBox {
		standardButtons: {
			var btns = 0;
			if(positiveText != "")
				btns = btns | DialogButtonBox.Ok;
			if(negativeText != "")
				btns = btns | DialogButtonBox.Cancel;
			if(neutralText != "")
				btns = btns | DialogButtonBox.Discard;
			return btns;
		}

		onStandardButtonsChanged: {
			if(positiveText != "")
				standardButton(DialogButtonBox.Ok).text = Qt.binding(function(){ return positiveText; });
			if(negativeText != "")
				standardButton(DialogButtonBox.Cancel).text = Qt.binding(function(){ return negativeText; });
			if(neutralText != "")
				standardButton(DialogButtonBox.Discard).text = Qt.binding(function(){ return neutralText; });
		}

		onClicked: {
			if(messageResult && button === standardButton(DialogButtonBox.Discard))
				messageResult.complete(MessageResult.NeutralResult, undefined)
		}
	}
}
