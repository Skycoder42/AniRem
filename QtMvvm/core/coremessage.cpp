#include "coremessage.h"

#include <QThread>

MessageResult *CoreMessage::information(const QString &title, const QString &text, const QString &okText)
{
	return message(CoreApp::Information,
				   title,
				   text,
				   okText.isNull() ? CoreApp::tr("Ok") : okText);
}

bool CoreMessage::information(const QString &title, const QString &text, std::function<void ()> onResult, const QString &okText)
{
	auto result = message(CoreApp::Information,
						  title,
						  text,
						  okText.isNull() ? CoreApp::tr("Ok") : okText);
	if(!result)
		return false;
	result->setAutoDelete(true);
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), onResult,
					 Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::question(const QString &title, const QString &text, const QString &yesText, const QString &noText)
{
	return message(CoreApp::Question,
				   title,
				   text,
				   yesText.isNull() ? CoreApp::tr("Yes") : yesText,
				   noText.isNull() ? CoreApp::tr("No") : noText);
}

bool CoreMessage::question(const QString &title, const QString &text, std::function<void (bool)> onResult, const QString &yesText, const QString &noText)
{
	auto result = message(CoreApp::Question,
						  title,
						  text,
						  yesText.isNull() ? CoreApp::tr("Yes") : yesText,
						  noText.isNull() ? CoreApp::tr("No") : noText);
	if(!result)
		return false;
	result->setAutoDelete(true);
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), [onResult](MessageResult::ResultType type) {
		onResult(type == MessageResult::PositiveResult);
	}, Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::warning(const QString &title, const QString &text, const QString &okText)
{
	return message(CoreApp::Warning,
				   title,
				   text,
				   okText.isNull() ? CoreApp::tr("Ok") : okText);
}

bool CoreMessage::warning(const QString &title, const QString &text, std::function<void ()> onResult, const QString &okText)
{
	auto result = message(CoreApp::Warning,
						  title,
						  text,
						  okText.isNull() ? CoreApp::tr("Ok") : okText);
	if(!result)
		return false;
	result->setAutoDelete(true);
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), onResult,
					 Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::critical(const QString &title, const QString &text, const QString &okText)
{
	return message(CoreApp::Critical,
				   title,
				   text,
				   okText.isNull() ? CoreApp::tr("Ok") : okText);
}

bool CoreMessage::critical(const QString &title, const QString &text, std::function<void ()> onResult, const QString &okText)
{
	auto result = message(CoreApp::Critical,
						  title,
						  text,
						  okText.isNull() ? CoreApp::tr("Ok") : okText);
	if(!result)
		return false;
	result->setAutoDelete(true);
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), onResult,
					 Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::getInput(const QString &title, const QString &text, int inputType, const QString &okText, const QString &cancelText)
{
	return message(CoreApp::Input,
				   title,
				   text,
				   okText.isNull() ? CoreApp::tr("Ok") : okText,
				   cancelText.isNull() ? CoreApp::tr("Cancel") : cancelText,
				   {},
				   inputType);
}

bool CoreMessage::getInput(const QString &title, const QString &text, int inputType, std::function<void (QVariant)> onResult, const QString &okText, const QString &cancelText)
{
	auto result = message(CoreApp::Input,
						  title,
						  text,
						  okText.isNull() ? CoreApp::tr("Ok") : okText,
						  cancelText.isNull() ? CoreApp::tr("Cancel") : cancelText,
						  {},
						  inputType);
	if(!result)
		return false;
	result->setAutoDelete(true);
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), [onResult, result](MessageResult::ResultType type) {
		onResult(type == MessageResult::PositiveResult ? result->result() : QVariant());
	}, Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::message(CoreApp::MessageType type, const QString &title, const QString &text, const QString &positiveAction, const QString &negativeAction, const QString &neutralAction, int inputType)
{
	auto result = new MessageResult();
	result->setAutoDelete(true);
	if(!QThread::currentThread()->eventDispatcher())//if not in qt, move to main thread
		result->moveToThread(qApp->thread());
	QMetaObject::invokeMethod(CoreApp::instance(), "showMessage", Qt::QueuedConnection,
							  Q_ARG(MessageResult*, result),
							  Q_ARG(CoreApp::MessageType, type),
							  Q_ARG(QString, title),
							  Q_ARG(QString, text),
							  Q_ARG(QString, positiveAction),
							  Q_ARG(QString, negativeAction),
							  Q_ARG(QString, neutralAction),
							  Q_ARG(int, inputType));
	return result;
}
