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
		delay(animDuration + 500, item.destroy);
		return true;
	}

	function closeAction() {
		if(mainStack.depth <= 1) {
			mainStack.currentItem.destroy();
			return true;
		} else {
			var item = mainStack.pop();
			delay(animDuration + 500, item.destroy);
			return false;
		}
	}

	function delay(delayMSec, fn) {
		var timer = Qt.createQmlObject("import QtQuick 2.8; Timer {}", mainStack);
		timer.interval = delayMSec;
		timer.repeat = false;
		timer.triggered.connect(function(){
			fn();
			timer.destroy();
		});
		timer.start();
	}

	pushEnter: Transition {
		PropertyAnimation {
			property: "y"
			easing.type: Easing.InOutQuad
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
			easing.type: Easing.InOutQuad
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
