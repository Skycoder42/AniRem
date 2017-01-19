import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import com.skycoder42.qtmvvm 1.0

AppBase {
	id: root

	function presentItem(item) {
		return mainStack.push(item) ? true : false;
	}

	function presentPopup(popup) {
		popup.parent = root;
		popup.open();
		return true;
	}

	PresentingStackView {
		id: mainStack
	}

	onClosing: close.accepted = mainStack.closeAction();
}
