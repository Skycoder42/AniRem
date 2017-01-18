#ifndef CONTROL_H
#define CONTROL_H

#include "qtmvvm_core_global.h"
#include <QObject>

class QTMVVM_CORE_SHARED_EXPORT Control : public QObject
{
	Q_OBJECT

public:
	explicit Control(QObject *parent = nullptr);

	virtual void onShow();
	virtual void onClose();

	virtual Control *parentControl() const;

public slots:
	void close();

protected slots:
	void showControl(Control *control) const;
};

Q_DECLARE_METATYPE(Control*)

#endif // CONTROL_H
