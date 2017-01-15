#ifndef IPRESENTER_H
#define IPRESENTER_H

#include "control.h"
#include "messageresult.h"

class IPresenter
{
public:
	enum MessageType {
		Information,
		Question,
		Warning,
		Critical,
		Input
	};

	inline virtual ~IPresenter() {}

	virtual bool present(Control *control) = 0;
	virtual bool withdraw(Control *control) = 0;

	virtual MessageResult *showMessage(MessageType type,
									   const QString &title,
									   const QString &text,
									   const QString &positiveAction = {},
									   const QString &negativeAction = {},
									   const QString &neutralAction = {},
									   int inputType = QMetaType::UnknownType) = 0;

};

#endif // IPRESENTER_H
