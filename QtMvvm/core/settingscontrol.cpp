#include "jsonsettingssetuploader.h"
#include "settingscontrol.h"

SettingsControl::SettingsControl(QObject *parent) :
	SettingsControl({}, nullptr, parent)
{}

SettingsControl::SettingsControl(QSettings *settings, QObject *parent) :
	SettingsControl({}, settings, parent)
{}

SettingsControl::SettingsControl(const QString &setupFilePath, QSettings *settings, QObject *parent) :
	Control(parent),
	_setupFile(setupFilePath.isNull() ? QStringLiteral(":/etc/settings.json") : setupFilePath),
	_settings(settings),
	_setupLoader(new JsonSettingsSetupLoader()),
	_keyMapping()
{
	if(_settings)
		_settings->setParent(this);
	else
		_settings = new QSettings(this);
}

void SettingsControl::setSetupLoader(SettingsSetupLoader *loader)
{
	_setupLoader.reset(loader);
}

SettingsSetupLoader *SettingsControl::setupLoader() const
{
	return _setupLoader.data();
}

QString SettingsControl::setupFilePath() const
{
	return _setupFile;
}

bool SettingsControl::canRestoreDefaults() const
{
	return true;
}

void SettingsControl::setMapping(const QString &uiId, const QString &settingsKey)
{
	_keyMapping.insert(uiId, settingsKey);
}

void SettingsControl::restoreDefaults(const QStringList &keyList)
{
	foreach(auto key, keyList)
		resetValue(key);
}

QVariant SettingsControl::loadValue(const QString &uiId)
{
	return _settings->value(_keyMapping.value(uiId, uiId));
}

void SettingsControl::saveValue(const QString &uiId, const QVariant &value)
{
	_settings->setValue(_keyMapping.value(uiId, uiId), value);
}

void SettingsControl::resetValue(const QString &uiId)
{
	_settings->remove(_keyMapping.value(uiId, uiId));
}
