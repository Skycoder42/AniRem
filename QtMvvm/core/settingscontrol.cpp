#include "settingscontrol.h"
#include "xmlsettingssetuploader.h"

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

SettingsControl::SettingsControl(const QString &setupFolder, QSettings *settings, QObject *parent) :
	Control(parent),
	_setupFolder(setupFolder.isNull() ? QStringLiteral(":/etc") : setupFolder),
	_settings(settings),
	_setupLoader(new XmlSettingsSetupLoader()),
	_keyMapping(),
	_allowCaching(true),
	_loadedSetups()
{
	if(_settings)
		_settings->setParent(this);
	else
		_settings = new QSettings(this);
	QMetaObject::invokeMethod(this, "doAutoConnections", Qt::QueuedConnection);
}

void SettingsControl::showSettingsControl(QObject *parent)
{
	auto control = new SettingsControl(parent);
	control->setDeleteOnClose(true);
	control->showControl(control);
}

void SettingsControl::setSetupLoader(SettingsSetupLoader *loader)
{
	_setupLoader.reset(loader);
	_loadedSetups.clear();
}

void SettingsControl::setSetupFolder(const QString &folder)
{
	_setupFolder = folder;
	_loadedSetups.clear();
}

SettingsSetup SettingsControl::loadSetup(const QByteArray &platform) const
{
	if(!_loadedSetups.contains(platform)) {
		try {
			SettingsSetup setup;
			QFile setupFile(_setupFolder.absoluteFilePath(QStringLiteral("settings.xml")));
			if(setupFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QFile extraFile(_setupFolder.absoluteFilePath(QStringLiteral("properties.xml")));
				if(extraFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
					setup = _setupLoader->loadSetup(platform, &setupFile, &extraFile);
					extraFile.close();
				} else
					setup = _setupLoader->loadSetup(platform, &setupFile);
				setupFile.close();
			} else
				throw setupFile.errorString();

			if(_allowCaching)
				_loadedSetups.insert(platform, setup);
			else
				return setup;
		} catch(QString s) {
			qWarning() << "Failed to load settings setup with error"
					   << s;
			return {};
		}
	}

	return _loadedSetups.value(platform);
}

QSettings *SettingsControl::settings() const
{
	return _settings;
}

bool SettingsControl::canRestoreDefaults() const
{
	return true;
}

bool SettingsControl::allowCaching() const
{
	return _allowCaching;
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

void SettingsControl::setAllowCaching(bool allowCaching)
{
	if (_allowCaching == allowCaching)
		return;

	_allowCaching = allowCaching;
	_loadedSetups.clear();
	emit allowCachingChanged(allowCaching);
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
