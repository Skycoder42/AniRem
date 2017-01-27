#include "jsonsettingssetuploader.h"
#include "settingscontrol.h"

#include <QFile>
#include <QDebug>

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

SettingsSetup SettingsControl::loadSetup() const
{
	try {
		SettingsSetup setup;
		QFile setupFile(":/etc/settings.json");
		if(setupFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QFile extraFile(":/etc/properties.json");
			if(extraFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
				setup = _setupLoader->loadSetup(&setupFile, &extraFile);
				extraFile.close();
			} else
				setup = _setupLoader->loadSetup(&setupFile);
			setupFile.close();
		}
		return setup;
	} catch(QString s) {
		qWarning() << "Failed to load settings setup with error"
				   << s;
		return {};
	}
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

QVariant SettingsControl::loadValue(const QString &uiId, const QVariant &defaultValue)
{
	return _settings->value(_keyMapping.value(uiId, uiId), defaultValue);
}

void SettingsControl::saveValue(const QString &uiId, const QVariant &value)
{
	_settings->setValue(_keyMapping.value(uiId, uiId), value);
}

void SettingsControl::resetValue(const QString &uiId)
{
	_settings->remove(_keyMapping.value(uiId, uiId));
}
