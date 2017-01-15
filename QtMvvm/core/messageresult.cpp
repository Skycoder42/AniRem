#include "coreapp.h"
#include "messageresult.h"

MessageResult::MessageResult() :
	QObject(),
	_result()
{}

bool MessageResult::hasResult() const
{
	return _result.isValid();
}

QVariant MessageResult::result() const
{
	return _result;
}

void MessageResult::setResult(QVariant result)
{
	if (_result == result)
		return;

	_result = result;
	emit resultChanged(result);
}
