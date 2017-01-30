#include "jsonsettingssetuploader.h"
#include "settingscontrol.h"

#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QMetaMethod>

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
	QMetaObject::invokeMethod(this, "doAutoConnections", Qt::QueuedConnection);
}

void SettingsControl::setSetupLoader(SettingsSetupLoader *loader)
{
	_setupLoader.reset(loader);
}

SettingsSetup SettingsControl::loadSetup(const QByteArray &platform) const
{
	try {
		SettingsSetup setup;
		QFile setupFile(_setupFile);
		if(setupFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QFile extraFile(QFileInfo(_setupFile).dir().absoluteFilePath(QStringLiteral("properties.json")));
			if(extraFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
				setup = _setupLoader->loadSetup(platform, &setupFile, &extraFile);
				extraFile.close();
			} else
				setup = _setupLoader->loadSetup(platform, &setupFile);
			setupFile.close();
		}
		return setup;
	} catch(QString s) {
		qWarning() << "Failed to load settings setup with error"
				   << s;
		return {};
	}
}

bool SettingsControl::canRestoreDefaults() const
{
	return true;
}

void SettingsControl::setMapping(const QString &uiId, const QString &settingsKey)
{
	_keyMapping.insert(uiId, settingsKey);
}

QVariant SettingsControl::loadValue(const QString &uiId, const QVariant &defaultValue) const
{
	return _settings->value(_keyMapping.value(uiId, uiId), defaultValue);
}

void SettingsControl::saveValue(const QString &uiId, const QVariant &value)
{
	auto key = _keyMapping.value(uiId, uiId);
	_settings->setValue(key, value);
	emit valueChanged(key, value);
}

void SettingsControl::resetValue(const QString &uiId)
{
	auto key = _keyMapping.value(uiId, uiId);
	_settings->remove(key);
	emit valueChanged(key, QVariant());
}

CoreApp::MessageConfig SettingsControl::restoreConfig() const
{
	CoreApp::MessageConfig config;
	config.type = CoreApp::Warning;
	config.title = tr("Restore Defaults?");
	config.text = tr("All custom changes will be deleted and the defaults restored. <i>This cannot be undone!</i>");
	config.positiveAction = tr("Yes");
	config.negativeAction = tr("No");
	return config;
}

void SettingsControl::doAutoConnections()
{
	auto meta = metaObject();
	for(auto i = SettingsControl::staticMetaObject.methodOffset(); i < meta->methodCount(); i++) {
		auto method = meta->method(i);
		if(method.name().startsWith("connectDelegateSignals"))
			method.invoke(this);
	}
}
