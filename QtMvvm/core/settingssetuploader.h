#ifndef SETTINGSSETUPLOADER_H
#define SETTINGSSETUPLOADER_H

#include <QIcon>
#include <QSharedPointer>
#include <QVariant>

struct SettingsEntry
{
	QString key;
	QString title;
	QString tooltip;
	QString type;
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
	QIcon icon;
	QString tooltip;

	QList<SettingsGroup> groups;
};

struct SettingsCategory
{
	QString title;
	QIcon icon;
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

	virtual SettingsSetup loadSetup(QIODevice *device, QIODevice *extraPropertyDevice = nullptr) = 0;
};

#endif // SETTINGSSETUPLOADER_H
