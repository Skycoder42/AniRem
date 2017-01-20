import QtQuick 2.8
import QtQuick.Controls 2.1
import com.skycoder42.qtmvvm 1.0

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

	onClosing: close.accepted = mainStack.closeAction();
}
