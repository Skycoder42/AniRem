import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.qtmvvm 1.0
import de.skycoder42.quickextras 1.0

AlertDialog {
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
		if(result)
			result.setCloseTarget(messageBox, messageBox.close);
		messageBox.iconType = type;
		messageBox.title = title;
		messageBox.titleText = title;
		messageBox.message = text;
		messageBox.positiveText = positiveText;
		messageBox.negativeText = negativeText;
		messageBox.neutralText = neutralText;
		messageBox.messageContentUrl = inputUrl;
		messageBox.open();
	}

	function messageIcon() {
		var base = "image://svg/qtmvvm/icons/ic_%1";
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
		return base;
	}

	onClosed: messageResult = null

	header: RowLayout {
		spacing: 14

		TintIcon {
			id: icon
			source: messageIcon()
			Layout.preferredWidth: 24
			Layout.preferredHeight: 24
			Layout.margins: 24
			Layout.bottomMargin: 0
			Layout.rightMargin: 0
		}

		Label {
			text: messageBox.title
			visible: messageBox.title
			elide: Label.ElideRight
			font.bold: true
			font.pixelSize: 16
			Layout.fillWidth: true
			Layout.margins: 24
			Layout.bottomMargin: 0
			Layout.leftMargin: icon.visible ? 0 : 24
		}
	}

	contentItem: ColumnLayout {
		Label {
			id: contentLabel
			visible: text != ""
			Layout.preferredWidth: contentLabel.implicitWidth

			wrapMode: Text.Wrap
			Layout.fillWidth: true
		}

		Loader {
			id: content

			Layout.preferredWidth: content.item ? content.item.implicitWidth : 0
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
