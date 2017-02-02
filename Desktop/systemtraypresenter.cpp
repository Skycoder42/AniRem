#include "systemtraypresenter.h"

SystemTrayPresenter::SystemTrayPresenter() :
	WidgetPresenter()
{}

void SystemTrayPresenter::present(Control *control)
{
	if(control->inherits("StatusControl")) {
		statusView.reset(new StatusView(control, qApp));
		control->onShow();
	} else
		WidgetPresenter::present(control);
}

void SystemTrayPresenter::withdraw(Control *control)
{
	if(control->inherits("StatusControl") && statusView) {
		statusView.reset();
		control->onClose();
	} else
		WidgetPresenter::present(control);
}
