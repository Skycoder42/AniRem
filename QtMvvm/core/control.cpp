#include "control.h"
#include "coreapp.h"

Control::Control(QObject *parent) :
	QObject(parent)
{}

bool Control::close()
{
	return CoreApp::instance()->closeControl(this);
}

bool Control::showControl(Control *control) const
{
	return CoreApp::instance()->showControl(control);
}

void Control::onShow() {}

void Control::onClose() {}
