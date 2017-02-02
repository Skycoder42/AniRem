#include "control.h"
#include "coreapp.h"

Control::Control(QObject *parent) :
	QObject(parent),
	_deleteOnClose(false)
{}

void Control::onShow() {}

void Control::onClose()
{
	if(_deleteOnClose)
		deleteLater();
}

Control *Control::parentControl() const
{
	return qobject_cast<Control*>(parent());
}

bool Control::deleteOnClose() const
{
	return _deleteOnClose;
}

void Control::close()
{
	QMetaObject::invokeMethod(CoreApp::instance(), "closeControl", Q_ARG(Control*, this));
}

void Control::setDeleteOnClose(bool deleteOnClose)
{
	if (_deleteOnClose == deleteOnClose)
		return;

	_deleteOnClose = deleteOnClose;
	emit deleteOnCloseChanged(deleteOnClose);
}

void Control::showControl(Control *control) const
{
	QMetaObject::invokeMethod(CoreApp::instance(), "showControl", Q_ARG(Control*, control));
}
