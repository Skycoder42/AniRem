import QtQuick 2.7
import QtQuick.Controls 2.0
import com.skycoder42.qtmvvm 1.0

ProgressBar {
	visible: QuickPresenter.viewLoading
	value: QuickPresenter.loadingProgress

	anchors.left: parent.left
	anchors.right: parent.right
	anchors.top: parent.top
	z: 10
}
