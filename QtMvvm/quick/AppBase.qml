import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
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

	ProgressBar {
		visible: QuickPresenter.viewLoading
		value: QuickPresenter.loadingProgress

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.topMargin: parent.header ? parent.header.height : 0
	}
}
