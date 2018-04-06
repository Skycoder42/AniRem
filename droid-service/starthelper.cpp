#include "starthelper.h"
#include <QtAndroid>
#include <passiveupdater.h>
#include <QtMvvmCore/ServiceRegistry>

QMutex StartHelper::_runMutex;
QPointer<StartHelper> StartHelper::_runInstance = nullptr;
bool StartHelper::_wasStarted = false;

StartHelper::StartHelper(QObject *parent) :
	QObject(parent)
{
	QMutexLocker locker(&_runMutex);
	if(_wasStarted)
		QMetaObject::invokeMethod(_runInstance, "startUpdater", Qt::QueuedConnection);
	else
		_runInstance = this;
}

void StartHelper::handleStart()
{
	QMutexLocker locker(&_runMutex);
	_wasStarted = true;
	if(_runInstance) {
		QMetaObject::invokeMethod(_runInstance, "startUpdater", Qt::QueuedConnection);
		_runInstance = nullptr;//set to null again, this way only 1 queued invokation is done
	}
}

void StartHelper::startUpdater()
{
	auto updater = QtMvvm::ServiceRegistry::instance()->constructInjected<PassiveUpdater>();
	connect(updater, &PassiveUpdater::done,
			this, &StartHelper::done);
	updater->performUpdateCheck();
}

void StartHelper::done()
{
	auto service = QtAndroid::androidService();
	service.callMethod<void>("completeAction");
}

extern "C" {

JNIEXPORT void JNICALL Java_de_skycoder42_anirem_AniremService_handleStart(JNIEnv */*env*/, jobject /*obj*/)
{
	StartHelper::handleStart();
}

}
