#include "widgetmessageresult.h"

WidgetMessageResult::WidgetMessageResult(QDialog *msgBox) :
	MessageResult(),
	msgBox(msgBox),
	fin(false)
{
	connect(msgBox, &QDialog::finished,
			this, &WidgetMessageResult::finished);
	connect(msgBox, &QDialog::destroyed,
			this, &WidgetMessageResult::destruct);
}

void WidgetMessageResult::closeMessage()
{
	msgBox->close();
}

void WidgetMessageResult::finished(int result)
{
	fin = true;
	switch (result) {
	case QDialog::Accepted:
		emit positiveAction();
		emit anyAction(PositiveResult);
		break;
	case QDialog::Rejected:
		emit negativeAction();
		emit anyAction(NegativeResult);
		break;
	default:
		emit neutralAction();
		emit anyAction(NeutralResult);
		break;
	}
}

void WidgetMessageResult::destruct()
{
	if(!fin) {
		emit negativeAction();
		emit anyAction(NegativeResult);
	}

	deleteLater();
}
