#include "statusview.h"
#include <QtAndroidExtras>

StatusView::StatusView(Control *mControl, QObject *parent) :
	QObject(parent),
	control(static_cast<StatusControl*>(mControl)),
	service(QtAndroid::androidService())
{
	Q_ASSERT(service.isValid());
	connect(control, &StatusControl::showUpdateNotification,
			this, &StatusView::showUpdateNotification);
}

void StatusView::showUpdateNotification(bool success, const QString &title, const QString &message)
{
	service.callMethod<void>("showUpdateNotification",
							 "(ZLjava/lang/String;Ljava/lang/String;)V",
							 (jboolean)success,
							 QAndroidJniObject::fromString(title).object<jstring>(),
							 QAndroidJniObject::fromString(message).object<jstring>());

	service.callMethod<void>("stopSelf");
	//TODO QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
}
