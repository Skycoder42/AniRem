#ifndef JSONSETTINGSSETUPLOADER_H
#define JSONSETTINGSSETUPLOADER_H

#include "settingssetuploader.h"
#include <QCoreApplication>

#include <QJsonArray>

class JsonSettingsSetupLoader : public SettingsSetupLoader
{
	Q_DECLARE_TR_FUNCTIONS(JsonSettingsSetupLoader)

public:
	JsonSettingsSetupLoader();

	SettingsSetup loadSetup(QIODevice *device) override;

private:
	QList<SettingsCategory> parseCategories(QJsonArray data);
	QList<SettingsSection> parseSections(QJsonArray data);
	QList<SettingsGroup> parseGroups(QJsonArray data);
	QList<SettingsEntry> parseEntries(QJsonObject data);
};

#endif // JSONSETTINGSSETUPLOADER_H
