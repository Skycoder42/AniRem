#include "qmltoast.h"
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#else
#include <QDebug>
#endif

QmlToast::QmlToast(QObject *parent) :
	QObject(parent)
{}

void QmlToast::toast(const QString &text, QmlToast::Duration duration)
{
#ifdef Q_OS_ANDROID
	QtAndroid::runOnAndroidThread([text, duration](){
		auto toast = QAndroidJniObject::callStaticObjectMethod("android/widget/Toast", "makeText",
															   "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;",
															   QtAndroid::androidContext().object(),
															   QAndroidJniObject::fromString(text).object(),
															   static_cast<jint>(duration));
		if(toast.isValid())
			toast.callMethod<void>("show");
	});
#else
	Q_UNUSED(duration)
	qInfo().noquote() << "TOAST:" << text;
#endif
}
