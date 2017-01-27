#ifndef SETTINGSCONTROL_H
#define SETTINGSCONTROL_H

#include "qtmvvm_core_global.h"
#include "control.h"
#include "settingssetuploader.h"
#include <QSettings>

class QTMVVM_CORE_SHARED_EXPORT SettingsControl : public Control
{
	Q_OBJECT

	Q_PROPERTY(QString setupFilePath READ setupFilePath CONSTANT)
	Q_PROPERTY(bool canRestoreDefaults READ canRestoreDefaults CONSTANT)

public:
	explicit SettingsControl(QObject *parent = nullptr);
	explicit SettingsControl(QSettings *settings, QObject *parent = nullptr);
	explicit SettingsControl(const QString &setupFilePath, QSettings *settings = nullptr, QObject *parent = nullptr);

	void setSetupLoader(SettingsSetupLoader *loader);
	SettingsSetup loadSetup() const;

	QString setupFilePath() const;
	virtual bool canRestoreDefaults() const;

	void setMapping(const QString &uiId, const QString &settingsKey);

public slots:
	virtual void restoreDefaults(const QStringList &keyList);

protected:
	virtual QVariant loadValue(const QString &uiId);
	virtual void saveValue(const QString &uiId, const QVariant &value);
	virtual void resetValue(const QString &uiId);

private:
	const QString _setupFile;
	QSettings *_settings;
	QScopedPointer<SettingsSetupLoader> _setupLoader;
	QHash<QString, QString> _keyMapping;
};



#endif // SETTINGSCONTROL_H
