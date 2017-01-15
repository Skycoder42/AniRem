#include "coreapp.h"
#include "coremessage.h"

MessageResult *CoreMessage::information(const QString &title, const QString &text, const QString &okText)
{
	return CoreApp::instance()->presenter()->showMessage(IPresenter::Information,
														 title,
														 text,
														 okText.isNull() ? CoreApp::tr("Ok") : okText);
}

bool CoreMessage::information(const QString &title, const QString &text, std::function<void ()> onResult, const QString &okText)
{
	auto result = CoreApp::instance()->presenter()->showMessage(IPresenter::Information,
																title,
																text,
																okText.isNull() ? CoreApp::tr("Ok") : okText);
	if(!result)
		return false;
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), onResult,
					 Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::question(const QString &title, const QString &text, const QString &yesText, const QString &noText)
{
	return CoreApp::instance()->presenter()->showMessage(IPresenter::Question,
														 title,
														 text,
														 yesText.isNull() ? CoreApp::tr("Yes") : yesText,
														 noText.isNull() ? CoreApp::tr("No") : noText);
}

bool CoreMessage::question(const QString &title, const QString &text, std::function<void (bool)> onResult, const QString &yesText, const QString &noText)
{
	auto result = CoreApp::instance()->presenter()->showMessage(IPresenter::Question,
																title,
																text,
																yesText.isNull() ? CoreApp::tr("Yes") : yesText,
																noText.isNull() ? CoreApp::tr("No") : noText);
	if(!result)
		return false;
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), [onResult](MessageResult::ResultType type) {
		onResult(type == MessageResult::PositiveResult);
	}, Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::warning(const QString &title, const QString &text, const QString &okText)
{
	return CoreApp::instance()->presenter()->showMessage(IPresenter::Warning,
														 title,
														 text,
														 okText.isNull() ? CoreApp::tr("Ok") : okText);
}

bool CoreMessage::warning(const QString &title, const QString &text, std::function<void ()> onResult, const QString &okText)
{
	auto result = CoreApp::instance()->presenter()->showMessage(IPresenter::Warning,
																title,
																text,
																okText.isNull() ? CoreApp::tr("Ok") : okText);
	if(!result)
		return false;
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), onResult,
					 Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::critical(const QString &title, const QString &text, const QString &okText)
{
	return CoreApp::instance()->presenter()->showMessage(IPresenter::Critical,
														 title,
														 text,
														 okText.isNull() ? CoreApp::tr("Ok") : okText);
}

bool CoreMessage::critical(const QString &title, const QString &text, std::function<void ()> onResult, const QString &okText)
{
	auto result = CoreApp::instance()->presenter()->showMessage(IPresenter::Critical,
																title,
																text,
																okText.isNull() ? CoreApp::tr("Ok") : okText);
	if(!result)
		return false;
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), onResult,
					 Qt::QueuedConnection);
	return true;
}

MessageResult *CoreMessage::getInput(const QString &title, const QString &text, int inputType, const QString &okText, const QString &cancelText)
{
	return CoreApp::instance()->presenter()->showMessage(IPresenter::Input,
														 title,
														 text,
														 okText.isNull() ? CoreApp::tr("Ok") : okText,
														 cancelText.isNull() ? CoreApp::tr("Cancel") : cancelText,
														 {},
														 inputType);
}

bool CoreMessage::getInput(const QString &title, const QString &text, int inputType, std::function<void (QVariant)> onResult, const QString &okText, const QString &cancelText)
{
	auto result = CoreApp::instance()->presenter()->showMessage(IPresenter::Input,
																title,
																text,
																okText.isNull() ? CoreApp::tr("Ok") : okText,
																cancelText.isNull() ? CoreApp::tr("Cancel") : cancelText,
																{},
																inputType);
	if(!result)
		return false;
	QObject::connect(result, &MessageResult::anyAction,
					 CoreApp::instance(), [onResult, result](MessageResult::ResultType type) {
		onResult(type == MessageResult::PositiveResult ? result->result() : QVariant());
	}, Qt::QueuedConnection);
	return true;
}
