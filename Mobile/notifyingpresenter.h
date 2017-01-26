#ifndef NOTIFYINGPRESENTER_H
#define NOTIFYINGPRESENTER_H

#include <quickpresenter.h>
#ifdef Q_OS_ANDROID
#include "statusview.h"
#endif

class NotifyingPresenter : public QuickPresenter
{
public:
	NotifyingPresenter();

	void present(Control *control) override;
	void withdraw(Control *control) override;

private:
#ifdef Q_OS_ANDROID
	QScopedPointer<StatusView, QScopedPointerDeleteLater> statusView;
#endif
};

#endif // NOTIFYINGPRESENTER_H
