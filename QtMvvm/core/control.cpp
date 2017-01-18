#include "control.h"
#include "coreapp.h"

Control::Control(QObject *parent) :
	QObject(parent)
{}

void Control::onShow() {}

void Control::onClose() {}

Control *Control::parentControl() const
{
	return qobject_cast<Control*>(parent());
}

void Control::close()
{
	QMetaObject::invokeMethod(CoreApp::instance(), "closeControl", Q_ARG(Control*, this));
}

void Control::showControl(Control *control) const
{
	QMetaObject::invokeMethod(CoreApp::instance(), "showControl", Q_ARG(Control*, control));
}
