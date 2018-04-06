#ifndef STARTHELPER_H
#define STARTHELPER_H

#include <QMutex>
#include <QObject>
#include <QPointer>

class StartHelper : public QObject
{
	Q_OBJECT

public:
	explicit StartHelper(QObject *parent = nullptr);

	static void handleStart();

private slots:
	void startUpdater();
	void done();

private:
	static QMutex _runMutex;
	static QPointer<StartHelper> _runInstance;
	static bool _wasStarted;
};

#endif // STARTHELPER_H
