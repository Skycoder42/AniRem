#ifndef XMLSETTINGSSETUPLOADER_H
#define XMLSETTINGSSETUPLOADER_H

#include "settingssetuploader.h"
#include <QXmlStreamReader>
#include <QCoreApplication>

class XmlSettingsSetupLoader : public SettingsSetupLoader
{
	Q_DECLARE_TR_FUNCTIONS(XmlSettingsSetupLoader)

public:
	SettingsSetup loadSetup(const QByteArray &platform, QIODevice *device, QIODevice *extraPropertyDevice) override;

private:
	void throwError(QXmlStreamReader &reader, const QString &customError = {});

	SettingsCategory readCategory(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsCategory readDefaultCategory(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsSection readSection(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsSection readDefaultSection(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsGroup readGroup(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsGroup readDefaultGroup(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsEntry readEntry(QXmlStreamReader &reader, const QByteArray &platform, bool &skip, const QVariantHash &extraProperties);

	QVariantHash readExtraProperties(QIODevice *device);

	QPair<QString, QVariant> readProperty(QXmlStreamReader &reader, const QString &overwriteType = {});
	QVariant readElement(QXmlStreamReader &reader, const QString &overwriteType = {});

	SettingsCategory createDefaultCategory();
	SettingsSection createDefaultSection();
};

#endif // XMLSETTINGSSETUPLOADER_H
