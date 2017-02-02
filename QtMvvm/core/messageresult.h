#ifndef MESSAGERESULT_H
#define MESSAGERESULT_H

#include "qtmvvm_core_global.h"
#include <QObject>
#include <QVariant>
#include <QMetaProperty>
#include <QPointer>

class QTMVVM_CORE_SHARED_EXPORT MessageResult : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QVariant result READ result WRITE setResult NOTIFY resultChanged)
	Q_PROPERTY(bool autoDelete READ autoDelete WRITE setAutoDelete NOTIFY autoDeleteChanged)

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
	bool autoDelete() const;

public slots:
	void closeMessage();

	void setResult(QVariant result);
	void setAutoDelete(bool autoDelete);

	//USE IN GUI ONLY!
	void setCloseTarget(QObject *closeObject, const QMetaMethod &closeMethod);
	void complete(MessageResult::ResultType result, const QVariant &resultValue);

signals:
	void positiveAction();
	void negativeAction();
	void neutralAction();

	void anyAction(ResultType result);

	void resultChanged(QVariant result);
	void autoDeleteChanged(bool autoDelete);

private slots:
	void setCloseTargetInternal(QObject *closeObject, const QMetaMethod &closeMethod);
	void completeInternal(MessageResult::ResultType result, const QVariant &resultValue);

private:
	QPointer<QObject> _closeObject;
	QMetaMethod _closeMethod;
	bool _closeRequested;
	QVariant _result;
	bool _autoDelete;
};

Q_DECLARE_METATYPE(MessageResult*)

#endif // MESSAGERESULT_H
