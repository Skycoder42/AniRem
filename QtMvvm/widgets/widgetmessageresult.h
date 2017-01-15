#ifndef WIDGETMESSAGERESULT_H
#define WIDGETMESSAGERESULT_H

#include <QDialog>
#include <QObject>
#include <messageresult.h>

class WidgetMessageResult : public MessageResult
{
	Q_OBJECT
public:
	explicit WidgetMessageResult(QDialog *msgBox);

public slots:
	void closeMessage();

private slots:
	void finished(int result);
	void destruct();

private:
	QDialog *msgBox;
	bool fin;
};

#endif // WIDGETMESSAGERESULT_H
