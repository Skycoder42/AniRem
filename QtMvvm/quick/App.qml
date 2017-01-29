import QtQuick 2.8
import QtQuick.Controls 2.1
import de.skycoder42.qtmvvm 1.0
import QtQuick.Controls.Material 2.1

AppBase {
	id: root

	function presentItem(item) {
		return mainStack.presentItem(item);
	}

	function presentPopup(popup) {
		popup.parent = root.contentItem;
		popup.closed.connect(popup.destroy);
		popup.open();
		return true;
	}

	function showMessage(result, type, title, text, positiveText, negativeText, neutralText, inputUrl, editProperties) {
		messageBox.showMessageBox(result, type, title, text, positiveText, negativeText, neutralText, inputUrl, editProperties);
	}

	function withdrawItem(item) {
		return mainStack.withdrawItem(item);
	}

	function withdrawPopup(popup) {
		popup.close();
		return true;
	}

	PresentingStackView {
		id: mainStack
	}

	MessageBox {
		id:messageBox
	}

	onClosing: close.accepted = mainStack.closeAction();
}
