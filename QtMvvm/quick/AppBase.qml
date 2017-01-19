import QtQuick 2.7
import QtQuick.Controls 2.0
import com.skycoder42.qtmvvm 1.0

ApplicationWindow {
	id: root
	visible: true
	width: 360
	height: 520

	function presentItem(item) {
		//present the item
		return false;
	}

	function presentPopup(popup) {
		//present the popup
		return false;
	}

	Component.onCompleted: QuickPresenter.qmlPresenter = root

	PresenterProgress {
		z: 0
	}
}
