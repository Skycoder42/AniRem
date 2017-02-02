#ifndef IPRESENTER_H
#define IPRESENTER_H

#include "control.h"
#include "messageresult.h"
#include "coreapp.h"

class IPresenter
{
public:


	inline virtual ~IPresenter() {}

	virtual void present(Control *control) = 0;
	virtual void withdraw(Control *control) = 0;

	virtual void showMessage(MessageResult *result, const CoreApp::MessageConfig &config) = 0;

};

#endif // IPRESENTER_H
