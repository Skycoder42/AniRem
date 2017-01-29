#include "coremessage.h"

#include <QThread>

MessageResult *CoreMessage::information(const QString &title, const QString &text, const QString &okText)
{
	CoreApp::MessageConfig config;
	config.type = CoreApp::Information;
	config.title = title;
	config.text = text;
	config.positiveAction = okText.isNull() ? CoreApp::tr("Ok") : okText;
	return message(config);
}

bool CoreMessage::information(const QString &title, const QString &text, std::function<void ()> onResult, const QString &okText)
{
	auto result = information(title, text, okText);
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
	CoreApp::MessageConfig config;
	config.type = CoreApp::Question;
	config.title = title;
	config.text = text;
	config.positiveAction = yesText.isNull() ? CoreApp::tr("Yes") : yesText;
	config.negativeAction = noText.isNull() ? CoreApp::tr("No") : noText;
	return message(config);
}

bool CoreMessage::question(const QString &title, const QString &text, std::function<void (bool)> onResult, const QString &yesText, const QString &noText)
{
	auto result = question(title, text, yesText, noText);
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
	CoreApp::MessageConfig config;
	config.type = CoreApp::Warning;
	config.title = title;
	config.text = text;
	config.positiveAction = okText.isNull() ? CoreApp::tr("Ok") : okText;
	return message(config);
}

bool CoreMessage::warning(const QString &title, const QString &text, std::function<void ()> onResult, const QString &okText)
{
	auto result = warning(title, text, okText);
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
	CoreApp::MessageConfig config;
	config.type = CoreApp::Critical;
	config.title = title;
	config.text = text;
	config.positiveAction = okText.isNull() ? CoreApp::tr("Ok") : okText;
	return message(config);
}

bool CoreMessage::critical(const QString &title, const QString &text, std::function<void ()> onResult, const QString &okText)
{
	auto result = critical(title, text, okText);
	if(!result)
		return false;
	result->setAutoDelete(true);
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), onResult,
					 Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::getInput(const QString &title, const QString &text, const char *inputType, const QVariant &defaultValue, const QVariantMap &editProperties, const QString &okText, const QString &cancelText)
{
	CoreApp::MessageConfig config;
	config.type = CoreApp::Input;
	config.title = title;
	config.text = text;
	config.positiveAction = okText.isNull() ? CoreApp::tr("Ok") : okText;
	config.negativeAction = cancelText.isNull() ? CoreApp::tr("Cancel") : cancelText;
	config.inputType = inputType;
	config.defaultValue = defaultValue;
	config.editProperties = editProperties;
	return message(config);
}

bool CoreMessage::getInput(const QString &title, const QString &text, int inputType, std::function<void (QVariant)> onResult, const QVariant &defaultValue, const QVariantMap &editProperties, const QString &okText, const QString &cancelText)
{
	return getInput(title, text, QMetaType::typeName(inputType), onResult, defaultValue, editProperties, okText, cancelText);
}

bool CoreMessage::getInput(const QString &title, const QString &text, const char *inputType, std::function<void (QVariant)> onResult, const QVariant &defaultValue, const QVariantMap &editProperties, const QString &okText, const QString &cancelText)
{
	auto result =  getInput(title, text, inputType, defaultValue, editProperties, okText, cancelText);
	if(!result)
		return false;
	result->setAutoDelete(true);
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), [onResult, result](MessageResult::ResultType type) {
		onResult(type == MessageResult::PositiveResult ? result->result() : QVariant());
	}, Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::getInput(const QString &title, const QString &text, int inputType, const QVariant &defaultValue, const QVariantMap &editProperties, const QString &okText, const QString &cancelText)
{
	return getInput(title, text, QMetaType::typeName(inputType), defaultValue, editProperties, okText, cancelText);
}

MessageResult *CoreMessage::message(const CoreApp::MessageConfig &config)
{
	auto result = new MessageResult();
	result->setAutoDelete(true);
	if(!QThread::currentThread()->eventDispatcher())//if not in qt, move to main thread
		result->moveToThread(qApp->thread());
	QMetaObject::invokeMethod(CoreApp::instance(), "showMessage", Qt::QueuedConnection,
							  Q_ARG(MessageResult*, result),
							  Q_ARG(CoreApp::MessageConfig, config));
	return result;
}
