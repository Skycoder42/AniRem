#include "aniremsettingsviewmodel.h"
#include "aniremapp.h"
#include <syncedsettings.h>

AniremSettingsViewModel::AniremSettingsViewModel(QObject *parent) :
	DataSyncSettingsViewModel(parent),
	_loader(nullptr)
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

void AniremSettingsViewModel::callAction(const QString &key, const QVariantMap &parameters)
{
	Q_UNUSED(parameters)
	if(key == QStringLiteral("clearcache")) {
		QtMvvm::question(tr("Clear Cache?"),
						 tr("Do you want to remove all cached preview images? This will free some storage, "
							"but the app will have to download previews of you animes again as soon as they are needed."),
						 this, [this](bool ok){
			if(ok)
				_loader->clearCache();
		});
	}
}
