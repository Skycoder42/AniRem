import QtQuick 2.8
import QtQuick.Controls 2.1
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

	function showMessage(result, type, title, text, positiveText, negativeText, neutralText, inputUrl) {
		result.complete(MessageResult.NegativeResult, undefined);
	}

	function withdrawItem(item) {
		return false;
	}

	function withdrawPopup(popup) {
		return false;
	}

	Component.onCompleted: QuickPresenter.qmlPresenter = root

	PresenterProgress {
		z: 0
	}
}
