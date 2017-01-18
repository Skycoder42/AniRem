import QtQuick 2.7
import QtQuick.Controls 2.0

Page {
	property var control: null

	Label {
		text: control ? "loaded!" : "loading..."
		anchors.centerIn: parent
	}
}
