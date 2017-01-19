import QtQuick 2.8
import QtQuick.Controls 2.1
import com.skycoder42.qtmvvm 1.0

StackView {
	id: mainStack
	anchors.fill: parent

	readonly property int animDuration: 150
	readonly property int opDuration: 75

	function presentItem(item) {
		return push(item) ? true : false;
	}

	function withdrawItem(item) {
		if(currentItem === item)
			pop();
		item.destroy();
		return true;
	}

	function closeAction() {
		if(mainStack.depth <= 1) {
			mainStack.currentItem.destroy();
			return true;
		} else {
			var item = mainStack.pop();
			item.destroy();
			return false;
		}
	}

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
