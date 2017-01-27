#include "jsonsettingssetuploader.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

JsonSettingsSetupLoader::JsonSettingsSetupLoader(){}

SettingsSetup JsonSettingsSetupLoader::loadSetup(QIODevice *device, QIODevice *extraPropertyDevice)
{
	QJsonParseError error;
	auto doc = QJsonDocument::fromJson(device->readAll(), &error);
	if(error.error != QJsonParseError::NoError) {
		throw QStringLiteral("%1 (at %2)")
				.arg(error.errorString())
				.arg(error.offset);
	}

	QVariantHash extraProperties;
	if(extraPropertyDevice)
		extraProperties = loadExtraProperties(extraPropertyDevice);

	SettingsSetup setup;

	auto root = doc.object();
	if(root.contains(QStringLiteral("allowSearch")))
		setup.allowSearch = root[QStringLiteral("allowSearch")].toBool();
	if(root.contains(QStringLiteral("allowRestore")))
		setup.allowRestore = root[QStringLiteral("allowRestore")].toBool();

	if(root.contains(QStringLiteral("categories")))
		setup.categories = parseCategories(root[QStringLiteral("categories")].toArray(), extraProperties);
	else {
		//create default --> dummy
		QJsonObject dummyCategory;
		dummyCategory["_default"] = true;
		if(root.contains(QStringLiteral("sections")))
			dummyCategory[QStringLiteral("sections")] = root[QStringLiteral("sections")];
		else if(root.contains(QStringLiteral("groups")))
			dummyCategory[QStringLiteral("groups")] = root[QStringLiteral("groups")];
		else if(root.contains(QStringLiteral("entries")))
			dummyCategory[QStringLiteral("entries")] = root[QStringLiteral("entries")];
		else
			throw QStringLiteral("setup: No valid entry key found");
		QJsonArray dummyArray;
		dummyArray.append(dummyCategory);
		setup.categories = parseCategories(dummyArray, extraProperties);
	}

	return setup;
}

QVariantHash JsonSettingsSetupLoader::loadExtraProperties(QIODevice *device)
{
	QJsonParseError error;
	auto doc = QJsonDocument::fromJson(device->readAll(), &error);
	if(error.error != QJsonParseError::NoError) {
		throw QStringLiteral("%1 (at %2)")
				.arg(error.errorString())
				.arg(error.offset);
	} else
		return doc.object().toVariantHash();
}

QList<SettingsCategory> JsonSettingsSetupLoader::parseCategories(QJsonArray data, const QVariantHash &extraProperties)
{
	QList<SettingsCategory> categories;
	foreach(auto value, data) {
		auto cJson = value.toObject();
		SettingsCategory category;
		if(cJson[QStringLiteral("_default")].toBool()) {
			category.title = tr("General Settings");
			category.icon = QIcon(QStringLiteral(":/icons/settings.ico"));//TODO more generic pls
		}
		if(cJson.contains(QStringLiteral("title")))
			category.title = cJson[QStringLiteral("title")].toString();
		if(cJson.contains(QStringLiteral("icon")))
			category.icon = QIcon(cJson[QStringLiteral("icon")].toString());
		if(cJson.contains(QStringLiteral("tooltip")))
			category.tooltip = cJson[QStringLiteral("tooltip")].toString();

		if(cJson.contains(QStringLiteral("sections")))
			category.sections = parseSections(cJson[QStringLiteral("sections")].toArray(), extraProperties);
		else {
			//create default --> dummy
			QJsonObject dummySection;
			dummySection["_default"] = true;
			if(cJson.contains(QStringLiteral("groups")))
				dummySection[QStringLiteral("groups")] = cJson[QStringLiteral("groups")];
			else if(cJson.contains(QStringLiteral("entries")))
				dummySection[QStringLiteral("entries")] = cJson[QStringLiteral("entries")];
			else
				throw QStringLiteral("category: No valid entry key found");
			QJsonArray dummyArray;
			dummyArray.append(dummySection);
			category.sections = parseSections(dummyArray, extraProperties);
		}

		categories.append(category);
	}

	return categories;
}

QList<SettingsSection> JsonSettingsSetupLoader::parseSections(QJsonArray data, const QVariantHash &extraProperties)
{
	QList<SettingsSection> sections;
	foreach(auto value, data) {
		auto sJson = value.toObject();
		SettingsSection section;
		if(sJson[QStringLiteral("_default")].toBool())
			section.title = tr("General");
		if(sJson.contains(QStringLiteral("title")))
			section.title = sJson[QStringLiteral("title")].toString();
		if(sJson.contains(QStringLiteral("icon")))
			section.icon = QIcon(sJson[QStringLiteral("icon")].toString());
		if(sJson.contains(QStringLiteral("tooltip")))
			section.tooltip = sJson[QStringLiteral("tooltip")].toString();

		if(sJson.contains(QStringLiteral("groups")))
			section.groups = parseGroups(sJson[QStringLiteral("groups")].toArray(), extraProperties);
		else {
			//create default --> dummy
			QJsonObject dummyGroup;
			dummyGroup["_default"] = true;
			if(sJson.contains(QStringLiteral("entries")))
				dummyGroup[QStringLiteral("entries")] = sJson[QStringLiteral("entries")];
			else
				throw QStringLiteral("section: No valid entry key found");
			QJsonArray dummyArray;
			dummyArray.append(dummyGroup);
			section.groups = parseGroups(dummyArray, extraProperties);
		}

		sections.append(section);
	}

	return sections;
}

QList<SettingsGroup> JsonSettingsSetupLoader::parseGroups(QJsonArray data, const QVariantHash &extraProperties)
{
	QList<SettingsGroup> groups;
	foreach(auto value, data) {
		auto gJson = value.toObject();
		SettingsGroup group;
		if(gJson.contains(QStringLiteral("title")))
			group.title = gJson[QStringLiteral("title")].toString();

		if(gJson.contains(QStringLiteral("entries")))
			group.entries = parseEntries(gJson[QStringLiteral("entries")].toObject(), extraProperties);
		else
			throw QStringLiteral("group: No valid entry key found");

		groups.append(group);
	}

	return groups;
}

QList<SettingsEntry> JsonSettingsSetupLoader::parseEntries(QJsonObject data, const QVariantHash &extraProperties)
{
	QList<SettingsEntry> entries;
	for(auto it = data.constBegin(); it != data.constEnd(); it++) {
		auto eJson = it.value().toObject();
		SettingsEntry entry;
		entry.key = it.key();
		entry.title = eJson[QStringLiteral("title")].toString();
		entry.tooltip = eJson[QStringLiteral("tooltip")].toString();
		entry.type = eJson[QStringLiteral("type")].toString();
		entry.defaultValue = eJson[QStringLiteral("default")].toVariant();
		foreach(auto key, eJson["searchKeys"].toArray())
			entry.searchKeys.append(key.toString());
		entry.properties = eJson[QStringLiteral("properties")].toVariant().toMap();
		auto extras = extraProperties.value(entry.key).toMap();
		for(auto jt = extras.constBegin(); jt != extras.constEnd(); jt++)
			entry.properties.insert(jt.key(), jt.value());
		entries.append(entry);
	}
	return entries;
}
