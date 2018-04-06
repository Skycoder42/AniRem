#include "aniremsettingsviewmodel.h"
#include "aniremapp.h"
#include <syncedsettings.h>

AniremSettingsViewModel::AniremSettingsViewModel(QObject *parent) :
	DataSyncSettingsViewModel(parent)
{}

void AniremSettingsViewModel::saveValue(const QString &key, const QVariant &value)
{
	DataSyncSettingsViewModel::saveValue(key, value);
	if(key == QStringLiteral("updates/autoCheck"))
		coreApp->updateAutoStartState(value.toInt());
}

void AniremSettingsViewModel::resetValue(const QString &key)
{
	DataSyncSettingsViewModel::resetValue(key);
	if(key == QStringLiteral("updates/autoCheck"))
		coreApp->updateAutoStartState(7);
}
