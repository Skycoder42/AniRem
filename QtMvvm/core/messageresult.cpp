#include "coreapp.h"
#include "messageresult.h"

MessageResult::MessageResult() :
	QObject(),
	_closeObject(nullptr),
	_closeMethod(),
	_closeRequested(false),
	_result(),
	_autoDelete(false)
{}

bool MessageResult::hasResult() const
{
	return _result.isValid();
}

QVariant MessageResult::result() const
{
	return _result;
}

bool MessageResult::autoDelete() const
{
	return _autoDelete;
}

void MessageResult::closeMessage()
{
	if(_closeObject)
		_closeMethod.invoke(_closeObject, Qt::QueuedConnection);
	else
		_closeRequested = true;
}

void MessageResult::setResult(QVariant result)
{
	if (_result == result)
		return;

	_result = result;
	emit resultChanged(result);
}

void MessageResult::setAutoDelete(bool autoDelete)
{
	if (_autoDelete == autoDelete)
		return;

	_autoDelete = autoDelete;
	emit autoDeleteChanged(autoDelete);
}

void MessageResult::setCloseTarget(QObject *closeObject, const QMetaMethod &closeMethod)
{
	QMetaObject::invokeMethod(this, "setCloseTargetInternal", Qt::QueuedConnection,
							  Q_ARG(QObject*, closeObject),
							  Q_ARG(QMetaMethod, closeMethod));
}

void MessageResult::complete(MessageResult::ResultType result, const QVariant &resultValue)
{
	QMetaObject::invokeMethod(this, "completeInternal", Qt::QueuedConnection,
							  Q_ARG(MessageResult::ResultType, result),
							  Q_ARG(QVariant, resultValue));
}

void MessageResult::setCloseTargetInternal(QObject *closeObject, const QMetaMethod &closeMethod)
{
	_closeObject = closeObject;
	_closeMethod = closeMethod;
	if(_closeRequested)
		closeMessage();
}

void MessageResult::completeInternal(MessageResult::ResultType result, const QVariant &resultValue)
{
	setResult(resultValue);
	switch (result) {
	case PositiveResult:
		emit positiveAction();
		break;
	case NegativeResult:
		emit negativeAction();
		break;
	case NeutralResult:
		emit neutralAction();
		break;
	default:
		Q_UNREACHABLE();
	}

	emit anyAction(result);

	if(_autoDelete)
		deleteLater();
}
