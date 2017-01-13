#ifndef IPRESENTER_H
#define IPRESENTER_H

#include "control.h"

class IPresenter
{
public:
	inline virtual ~IPresenter() {}

	virtual bool present(Control *control) = 0;
	virtual bool withdraw(Control *control) = 0;
};

#endif // IPRESENTER_H
