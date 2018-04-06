#ifndef IUPDATENOTIFIER_H
#define IUPDATENOTIFIER_H

#include <QObject>

class IUpdateNotifier
{
	Q_DISABLE_COPY(IUpdateNotifier)

public:
	inline IUpdateNotifier() = default;
	virtual inline ~IUpdateNotifier() = default;

public slots:
	virtual void showNotification(bool isPositive, const QString &title, const QString &description) = 0;
	virtual void updateProgress(int value, int max) = 0;
};

#define IUpdateNotifierIid "de.skycoder42.anirem.IUpdateNotifier"
Q_DECLARE_INTERFACE(IUpdateNotifier, IUpdateNotifierIid)
Q_DECLARE_METATYPE(IUpdateNotifier*)

#endif // IUPDATENOTIFIER_H
