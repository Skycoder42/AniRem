import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.qtmvvm 1.0
import de.skycoder42.quickextras 1.0

AlertDialog {
	id: messageBox
	visible: false

	property MessageResult messageResult: null

	function showMessageBox(result, type, title, text, positiveText, negativeText, neutralText, inputUrl, editProperties) {
		if(visible)
			reject();
		messageBox.messageResult = result;
		if(result)
			result.setCloseTarget(messageBox, messageBox.close);
		icon.iconType = type;
		messageBox.title = title;
		contentLabel.text = text;
		btnBox.positiveText = positiveText;
		btnBox.negativeText = negativeText;
		btnBox.neutralText = neutralText;
		if(inputUrl != "")
			contentLoader.setSource(inputUrl, editProperties);
		else
			contentLoader.source = "";
		messageBox.open();
	}

	onClosed: messageResult = null

	header: RowLayout {
		spacing: 14

		TintIcon {
			id: icon
			property int iconType

			visible: iconType != QuickPresenter.Input
			source: messageIcon()
			Layout.preferredWidth: 24
			Layout.preferredHeight: 24
			Layout.margins: 24
			Layout.bottomMargin: 0
			Layout.rightMargin: 0			

			function messageIcon() {
				var base = "image://svg/qtmvvm/icons/ic_%1";
				switch(iconType) {
				case QuickPresenter.Input:
					return "";
				case QuickPresenter.Information:
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
			id: contentLoader
			visible: item

			Layout.preferredWidth: contentLoader.item ? contentLoader.item.implicitWidth : 0
			Layout.fillWidth: true
		}
	}

	onAccepted: {
		if(messageResult)
			messageResult.complete(MessageResult.PositiveResult, contentLoader.item ? contentLoader.item.inputValue : undefined)
	}

	onRejected:  {
		if(messageResult)
			messageResult.complete(MessageResult.NegativeResult, undefined)
	}

	footer: DialogButtonBox {
		id: btnBox
		property string positiveText: ""
		property string negativeText: ""
		property string neutralText: ""

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
