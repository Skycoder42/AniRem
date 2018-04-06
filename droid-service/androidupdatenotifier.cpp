#include "androidupdatenotifier.h"
#include <QtAndroid>

AndroidUpdateNotifier::AndroidUpdateNotifier(QObject *parent) :
	QObject(parent),
	_service(QtAndroid::androidService())
{}

void AndroidUpdateNotifier::showNotification(bool isPositive, const QString &title, const QString &description)
{
	_service.callMethod<void>("showUpdateNotification",
							  "(ZLjava/lang/String;Ljava/lang/String;)V",
							  static_cast<jboolean>(isPositive),
							  QAndroidJniObject::fromString(title).object(),
							  QAndroidJniObject::fromString(description).object());
}

void AndroidUpdateNotifier::updateProgress(int current, int max)
{
	_service.callMethod<void>("updateProgress", "(II)V",
							  static_cast<jint>(current),
							  static_cast<jint>(max));
}
