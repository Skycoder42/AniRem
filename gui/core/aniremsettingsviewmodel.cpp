#include "aniremsettingsviewmodel.h"
#include "aniremapp.h"
#include <syncedsettings.h>

AniremSettingsViewModel::AniremSettingsViewModel(QObject *parent) :
	DataSyncSettingsViewModel(parent)
{}

void AniremSettingsViewModel::saveValue(const QString &key, const QVariant &value)
{
	if(key == QStringLiteral("updates/autoCheck")) {
		SyncedSettings::instance()->updates.autoCheck = value;
		coreApp->updateAutoStartState();
	} else
		DataSyncSettingsViewModel::saveValue(key, value);
}

void AniremSettingsViewModel::resetValue(const QString &key)
{
	if(key == QStringLiteral("updates/autoCheck")) {
		SyncedSettings::instance()->updates.autoCheck.reset();
		coreApp->updateAutoStartState();
	} else
		DataSyncSettingsViewModel::resetValue(key);
}
