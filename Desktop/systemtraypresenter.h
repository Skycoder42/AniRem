#ifndef SYSTEMTRAYPRESENTER_H
#define SYSTEMTRAYPRESENTER_H

#include "widgetpresenter.h"
#include "statusview.h"

class SystemTrayPresenter : public WidgetPresenter
{
public:
	SystemTrayPresenter();

public:
	void present(Control *control) override;
	void withdraw(Control *control) override;

private:
	QScopedPointer<StatusView, QScopedPointerDeleteLater> statusView;
};

#endif // SYSTEMTRAYPRESENTER_H
