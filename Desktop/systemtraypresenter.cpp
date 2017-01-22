#include "systemtraypresenter.h"
#include "statusview.h"

SystemTrayPresenter::SystemTrayPresenter() :
	WidgetPresenter()
{}

void SystemTrayPresenter::present(Control *control)
{
	if(control->inherits("StatusControl")) {
		new StatusView(control, qApp);
		control->onShow();
	} else
		WidgetPresenter::present(control);
}
