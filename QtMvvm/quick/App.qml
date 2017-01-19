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
		return mainStack.push(item) ? true : false;
	}

	function presentPopup(popup) {
		popup.parent = root;
		popup.open();
		return true;
	}

	Component.onCompleted: QuickPresenter.qmlPresenter = root

	StackView { //TODO move into own file!
		id: mainStack
		anchors.fill: parent

		readonly property int animDuration: 150
		readonly property int opDuration: 75

		pushEnter: Transition {
			PropertyAnimation {
				property: "y"
				from: height * 0.3
				to: 0
				duration: mainStack.animDuration
			}
			PropertyAnimation {
				property: "opacity"
				from: 0.0
				to: 1.0
				duration: mainStack.opDuration
			}
		}
		pushExit: Transition {
			PauseAnimation {
				duration: mainStack.animDuration
			}
		}
		popEnter: Transition {
			PauseAnimation {
				duration: mainStack.animDuration
			}
		}
		popExit: Transition {
			PropertyAnimation {
				property: "y"
				from: 0
				to: height * 0.3
				duration: mainStack.animDuration
			}
			SequentialAnimation {
				PauseAnimation {
					duration: mainStack.animDuration - mainStack.opDuration
				}
				PropertyAnimation {
					id: pp1
					property: "opacity"
					from: 1.0
					to: 0.0
					duration: mainStack.opDuration
				}
			}
		}
	}

	ProgressBar {
		visible: QuickPresenter.viewLoading
		value: QuickPresenter.loadingProgress

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.topMargin: parent.header ? parent.header.height : 0
	}

	onClosing: {
		if(mainStack.depth <= 1) {
			mainStack.currentItem.destroy();
			close.accepted = true;
		} else {
			close.accepted = false;
			var item = mainStack.pop();
			item.destroy();
		}
	}
}
