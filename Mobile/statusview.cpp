#include "statusview.h"
#include <QtAndroidExtras>

StatusView::StatusView(Control *mControl, QObject *parent) :
	QObject(parent),
	control(static_cast<StatusControl*>(mControl)),
	service(QtAndroid::androidService())
{
	Q_ASSERT(service.isValid());
	service.callMethod<void>("showProgressNotification");

	connect(control, &StatusControl::updateProgress,
			this, &StatusView::updateProgress);
	connect(control, &StatusControl::showUpdateNotification,
			this, &StatusView::showUpdateNotification);
}

void StatusView::updateProgress(int current, int max)
{
	service.callMethod<void>("updateProgress", "(II)V", (jint)current, (jint)max);
}

void StatusView::showUpdateNotification(bool success, const QString &title, const QString &message)
{
	service.callMethod<void>("showUpdateNotification",
							 "(ZLjava/lang/String;Ljava/lang/String;)V",
							 (jboolean)success,
							 QAndroidJniObject::fromString(title).object<jstring>(),
							 QAndroidJniObject::fromString(message).object<jstring>());
	qApp->quit();
}
