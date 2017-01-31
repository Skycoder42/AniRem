#ifndef SETTINGSCONTROL_H
#define SETTINGSCONTROL_H

#include "qtmvvm_core_global.h"
#include "control.h"
#include "settingssetuploader.h"
#include "coreapp.h"
#include <QDir>
#include <QSettings>

class QTMVVM_CORE_SHARED_EXPORT SettingsControl : public Control
{
	Q_OBJECT

	Q_PROPERTY(bool canRestoreDefaults READ canRestoreDefaults CONSTANT)
	Q_PROPERTY(bool allowCaching READ allowCaching WRITE setAllowCaching NOTIFY allowCachingChanged)

public:
	explicit SettingsControl(QObject *parent = nullptr);
	explicit SettingsControl(QSettings *settings, QObject *parent = nullptr);
	explicit SettingsControl(const QString &setupFolder, QSettings *settings = nullptr, QObject *parent = nullptr);

	static void showSettingsControl(QObject *parent = nullptr);

	void setSetupLoader(SettingsSetupLoader *loader);
	void setSetupFolder(const QString &folder);

	SettingsSetup loadSetup(const QByteArray &platform) const;

	QSettings *settings() const;

	virtual bool canRestoreDefaults() const;	
	bool allowCaching() const;

	void setMapping(const QString &uiId, const QString &settingsKey);

	Q_INVOKABLE virtual QVariant loadValue(const QString &uiId, const QVariant &defaultValue = {}) const;
	Q_INVOKABLE virtual void saveValue(const QString &uiId, const QVariant &value);
	Q_INVOKABLE virtual void resetValue(const QString &uiId);

	Q_INVOKABLE CoreApp::MessageConfig restoreConfig() const;

public slots:
	void setAllowCaching(bool allowCaching);

signals:
	void valueChanged(const QString &key, const QVariant &value);	
	void allowCachingChanged(bool allowCaching);

private slots:
	void doAutoConnections();

private:
	QDir _setupFolder;
	QSettings *_settings;
	QScopedPointer<SettingsSetupLoader> _setupLoader;
	QHash<QString, QString> _keyMapping;

	bool _allowCaching;
	mutable QHash<QByteArray, SettingsSetup> _loadedSetups;
};

#define SETTINGS_PROPERTY_WKEY(type, name, key, defaultValue) \
	Q_PROPERTY(type name READ name WRITE set ## name NOTIFY name ## Changed) \
public: \
	inline type name() const { \
		return loadValue(key, defaultValue).value<type>(); \
	} \
public slots: \
	inline void set ## name(const type &name) { \
		saveValue(key, name); \
	} \
private: \
	Q_INVOKABLE inline void connectDelegateSignals ## name() { \
		connect(this, &SettingsControl::valueChanged, \
				this, [=](const QString &sigKey, const QVariant &value){ \
			if(sigKey == key) \
				emit name ## Changed(value.value<type>()); \
		}); \
	}

#define SETTINGS_PROPERTY(type, name, defaultValue) SETTINGS_PROPERTY_WKEY(type, name, #name, defaultValue)

#endif // SETTINGSCONTROL_H
