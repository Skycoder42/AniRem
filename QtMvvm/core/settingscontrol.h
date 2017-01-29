#ifndef SETTINGSCONTROL_H
#define SETTINGSCONTROL_H

#include "qtmvvm_core_global.h"
#include "control.h"
#include "settingssetuploader.h"
#include "coreapp.h"
#include <QSettings>

class QTMVVM_CORE_SHARED_EXPORT SettingsControl : public Control
{
	Q_OBJECT

	Q_PROPERTY(bool canRestoreDefaults READ canRestoreDefaults CONSTANT)

public:
	explicit SettingsControl(QObject *parent = nullptr);
	explicit SettingsControl(QSettings *settings, QObject *parent = nullptr);
	explicit SettingsControl(const QString &setupFilePath, QSettings *settings = nullptr, QObject *parent = nullptr);

	void setSetupLoader(SettingsSetupLoader *loader);
	SettingsSetup loadSetup() const;

	virtual bool canRestoreDefaults() const;

	void setMapping(const QString &uiId, const QString &settingsKey);

	Q_INVOKABLE virtual QVariant loadValue(const QString &uiId, const QVariant &defaultValue = {}) const;
	Q_INVOKABLE virtual void saveValue(const QString &uiId, const QVariant &value);
	Q_INVOKABLE virtual void resetValue(const QString &uiId);

	Q_INVOKABLE CoreApp::MessageConfig restoreConfig() const;

signals:
	void valueChanged(const QString &key, const QVariant &value);

private slots:
	void doAutoConnections();

private:
	const QString _setupFile;
	QSettings *_settings;
	QScopedPointer<SettingsSetupLoader> _setupLoader;
	QHash<QString, QString> _keyMapping;
};

#define _SETTINGS_PROPERTY_METHODS_IMPL(type, name) \
public: \
	inline type name() const { \
		return loadValue(#name).value<type>(); \
	} \
public slots: \
	inline void set ## name(const type &name) { \
		saveValue(#name, name); \
	} \
private:

#define SETTINGS_PROPERTY(type, name) \
	Q_PROPERTY(type name READ name WRITE set ## name) \
	_SETTINGS_PROPERTY_METHODS_IMPL(type, name)

#define SETTINGS_PROPERTY_NOT(type, name) \
	Q_PROPERTY(type name READ name WRITE set ## name NOTIFY name ## Changed) \
	_SETTINGS_PROPERTY_METHODS_IMPL(type, name) \
	Q_INVOKABLE inline void connectDelegateSignals ## name() { \
		connect(this, &SettingsControl::valueChanged, \
				this, [=](const QString &key, const QVariant &value){ \
			if(key == #name) \
				emit name ## Changed(value.value<type>()); \
		}); \
	}

#endif // SETTINGSCONTROL_H
