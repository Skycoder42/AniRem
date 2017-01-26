#include "notifyingpresenter.h"

NotifyingPresenter::NotifyingPresenter() :
	QuickPresenter()
#ifdef Q_OS_ANDROID
	,statusView(nullptr)
  #endif
{}

void NotifyingPresenter::present(Control *control)
{
#ifdef Q_OS_ANDROID
	if(control->inherits("StatusControl")) {
		statusView.reset(new StatusView(control, qApp));
		control->onShow();
	} else
#endif
		QuickPresenter::present(control);

}

void NotifyingPresenter::withdraw(Control *control)
{
#ifdef Q_OS_ANDROID
	if(control->inherits("StatusControl") && statusView) {
		statusView.reset();
		control->onClose();
	} else
#endif
		QuickPresenter::present(control);
}
