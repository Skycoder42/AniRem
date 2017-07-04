#ifndef PROXERSETTINGSCONTROL_H
#define PROXERSETTINGSCONTROL_H

#include <QObject>
#include <settingscontrol.h>

class ProxerSettingsControl : public SettingsControl
{
	Q_OBJECT

	SETTINGS_PROPERTY(bool, openEntries, false)
	SETTINGS_PROPERTY(bool, uncheckEntries, true)
	SETTINGS_PROPERTY_WKEY(int, autoCheck, "updates/autoCheck", 7)
	SETTINGS_PROPERTY_WKEY(int, checkLimit, "updates/checkLimit", 15)

public:
	explicit ProxerSettingsControl(QObject *parent = nullptr);

	void ensureAutoStart();

signals:
	void openEntriesChanged(bool openEntries);
	void uncheckEntriesChanged(bool openEntries);
	void autoCheckChanged(int autoCheck);
	void checkLimitChanged(int checkLimit);

private slots:
	void updateAutoStart(int interval);

	bool setAutoStart(bool autoStart);
};

#endif // PROXERSETTINGSCONTROL_H
