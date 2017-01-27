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

	SettingsSetup loadSetup(QIODevice *device, QIODevice *extraPropertyDevice) override;

private:
	QVariantHash loadExtraProperties(QIODevice *device);
	QList<SettingsCategory> parseCategories(QJsonArray data, const QVariantHash &extraProperties);
	QList<SettingsSection> parseSections(QJsonArray data, const QVariantHash &extraProperties);
	QList<SettingsGroup> parseGroups(QJsonArray data, const QVariantHash &extraProperties);
	QList<SettingsEntry> parseEntries(QJsonObject data, const QVariantHash &extraProperties);
};

#endif // JSONSETTINGSSETUPLOADER_H
