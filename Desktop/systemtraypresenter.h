#ifndef SYSTEMTRAYPRESENTER_H
#define SYSTEMTRAYPRESENTER_H

#include "widgetpresenter.h"

class SystemTrayPresenter : public WidgetPresenter
{
public:
	SystemTrayPresenter();

public:
	void present(Control *control) override;
};

#endif // SYSTEMTRAYPRESENTER_H
