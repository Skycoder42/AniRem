#ifndef SETTINGSSETUPLOADER_H
#define SETTINGSSETUPLOADER_H

#include <QSharedPointer>
#include <QUrl>
#include <QVariant>
#include <QIODevice>

struct SettingsEntry
{
	QString key;
	QByteArray type;
	QString title;
	QString tooltip;
	QVariant defaultValue;
	QStringList searchKeys;
	QVariantMap properties;
};

struct SettingsGroup
{
	QString title;

	QList<SettingsEntry> entries;
};

struct SettingsSection
{
	QString title;
	QUrl icon;
	QString tooltip;

	QList<SettingsGroup> groups;
};

struct SettingsCategory
{
	QString title;
	QUrl icon;
	QString tooltip;

	QList<SettingsSection> sections;
};

struct SettingsSetup
{
	bool allowSearch;
	bool allowRestore;

	QList<SettingsCategory> categories;

	inline SettingsSetup() :
		allowSearch(false),
		allowRestore(false),
		categories()
	{}
};

class SettingsSetupLoader
{
public:
	virtual inline ~SettingsSetupLoader() {}

	virtual SettingsSetup loadSetup(const QByteArray &platform, QIODevice *device, QIODevice *extraPropertyDevice = nullptr) = 0;
};

#endif // SETTINGSSETUPLOADER_H
