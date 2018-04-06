#ifndef ANDROIDUPDATENOTIFIER_H
#define ANDROIDUPDATENOTIFIER_H

#include <QObject>
#include <QAndroidJniObject>
#include <iupdatenotifier.h>

class AndroidUpdateNotifier : public QObject, public IUpdateNotifier
{
	Q_OBJECT
	Q_INTERFACES(IUpdateNotifier)

public:
	Q_INVOKABLE explicit AndroidUpdateNotifier(QObject *parent = nullptr);

public slots:
	void showNotification(bool isPositive, const QString &title, const QString &description) override;
	void updateProgress(int current, int max) override;

private:
	QAndroidJniObject _service;
};

#endif // ANDROIDUPDATENOTIFIER_H
