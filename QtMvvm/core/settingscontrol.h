#ifndef SETTINGSCONTROL_H
#define SETTINGSCONTROL_H

#include "qtmvvm_core_global.h"
#include "control.h"
#include "settingssetuploader.h"
#include <QSettings>

#define SETTINGS_PROPERTY(type, name) \
public: \
	inline type name() const { \
		return loadValue(#name).value<type>(); \
	} \
public slots: \
	inline void set ## name(const type &name) { \
		saveValue(#name, name); \
	} \
signals: \
	void name ## Changed(const type &name); /*TODO not working...*/\
private: \
	Q_INVOKABLE inline void connectDelegateSignals ## name() { \
		connect(this, &SettingsControl::valueChanged, \
				this, [=](const QString &key, const QVariant &value){ \
			if(key == #name) \
				emit name ## Changed(value.value<type>()); \
		}); \
	} \
	Q_PROPERTY(type name READ name WRITE set ## name NOTIFY name ## Changed)

class QTMVVM_CORE_SHARED_EXPORT SettingsControl : public Control
{
	Q_OBJECT

	Q_PROPERTY(bool canRestoreDefaults READ canRestoreDefaults CONSTANT)

	SETTINGS_PROPERTY(bool, test)

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

#endif // SETTINGSCONTROL_H
