import QtQuick 2.8
import QtQuick.Controls 2.1

SpinBox {
	id: control
	editable: true
	from: Number.MIN_VALUE
	to: Number.MAX_VALUE

	validator: DoubleValidator {
		locale: control.locale.name
		bottom: Math.min(control.from, control.to)
		top: Math.max(control.from, control.to)
	}
}
