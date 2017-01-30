#ifndef CONTROL_H
#define CONTROL_H

#include "qtmvvm_core_global.h"
#include <QObject>

class QTMVVM_CORE_SHARED_EXPORT Control : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool deleteOnClose READ deleteOnClose WRITE setDeleteOnClose NOTIFY deleteOnCloseChanged)

public:
	explicit Control(QObject *parent = nullptr);

	virtual void onShow();
	virtual void onClose();

	virtual Control *parentControl() const;

	bool deleteOnClose() const;

public slots:
	void close();	
	void setDeleteOnClose(bool deleteOnClose);

signals:
	void deleteOnCloseChanged(bool deleteOnClose);

protected slots:
	void showControl(Control *control) const;

private:
	bool _deleteOnClose;
};

Q_DECLARE_METATYPE(Control*)

#endif // CONTROL_H
