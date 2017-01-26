#include "statusview.h"
#include <QtAndroidExtras>

StatusView::StatusView(Control *mControl, QObject *parent) :
	QObject(parent),
	control(static_cast<StatusControl*>(mControl)),
	javaNotifier()
{
	qDebug() << "status view created!";
	//QtAndroid::runOnAndroidThreadSync([this](){
		auto activity = QtAndroid::androidActivity();
		javaNotifier = QAndroidJniObject("de/skycoder42/seasonproxer/Notifier",
										 "(Landroid/content/Context;)V",
										 activity.object());
	//});
	Q_ASSERT(javaNotifier.isValid());
	connect(control, &StatusControl::showUpdateNotification,
			this, &StatusView::showUpdateNotification);
}

void StatusView::showUpdateNotification(bool success, const QString &title, const QString &message)
{
	//QtAndroid::runOnAndroidThread([=](){
		javaNotifier.callMethod<void>("showUpdateNotification",
									  "(ZLjava/lang/String;Ljava/lang/String;)V",
									  (jboolean)success,
									  QAndroidJniObject::fromString(title).object<jstring>(),
									  QAndroidJniObject::fromString(message).object<jstring>());

		QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
	//});
}
