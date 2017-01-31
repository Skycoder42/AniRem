#ifndef PROXERSETTINGSCONTROL_H
#define PROXERSETTINGSCONTROL_H

#include <QObject>
#include <settingscontrol.h>

class ProxerSettingsControl : public SettingsControl
{
	Q_OBJECT

	SETTINGS_PROPERTY(bool, openEntries)
	SETTINGS_PROPERTY_WKEY(int, autoCheck, "updates/autoCheck")
	SETTINGS_PROPERTY_WKEY(int, checkLimit, "updates/checkLimit")

public:
	explicit ProxerSettingsControl(QObject *parent = nullptr);

signals:
	void openEntriesChanged(bool openEntries);
	void autoCheckChanged(int autoCheck);
	void checkLimitChanged(int checkLimit);
};

#endif // PROXERSETTINGSCONTROL_H
