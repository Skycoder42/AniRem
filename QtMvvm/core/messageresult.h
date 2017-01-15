#ifndef MESSAGERESULT_H
#define MESSAGERESULT_H

#include "qtmvvm_core_global.h"
#include <QObject>
#include <QVariant>

class QTMVVM_CORE_SHARED_EXPORT MessageResult : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QVariant result READ result WRITE setResult NOTIFY resultChanged)

public:
	enum ResultType {
		PositiveResult,
		NegativeResult,
		NeutralResult
	};
	Q_ENUM(ResultType)

	explicit MessageResult();

	bool hasResult() const;
	QVariant result() const;

public slots:
	virtual void closeMessage() = 0;

	void setResult(QVariant result);

signals:
	void positiveAction();
	void negativeAction();
	void neutralAction();

	void anyAction(ResultType result);

	void resultChanged(QVariant result);

private:
	QVariant _result;
};

#endif // MESSAGERESULT_H
