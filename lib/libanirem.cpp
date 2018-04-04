#include "libanirem.h"

#include <QCoreApplication>
#include <QJsonSerializer>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>
#include <datasyncsettingsaccessor.h>

#include "jsonseasondataconverter.h"
#include "animeinfo.h"
#include "proxerapi.h"
#include "syncedsettings.h"

void AniRem::prepareTranslations()
{
	auto translator = new QTranslator(qApp);
	if(translator->load(QLocale(),
						QStringLiteral("anirem"),
						QStringLiteral("_"),
						QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
		qApp->installTranslator(translator);
}

void AniRem::setup(QtDataSync::Setup &setup, bool passive)
{
	setup.setRemoteConfiguration({QStringLiteral("wss://apps.skycoder42.de/datasync/")})
			.serializer()->addJsonTypeConverter<JsonSeasonDataConverter>();
	if(passive)
		setup.setRemoteObjectHost(QStringLiteral("local:de.skycoder42.anirem.daemon"));

	//also: setup API
	ProxerApi api;
	api.restClient()->serializer()->setAllowDefaultNull(true);//DEBUG use this to provoke an error to test error handling
}


namespace {

void cleanSettings()
{
	try {
		//WORKAROUND for settings destruction bug
		auto accessor = dynamic_cast<DataSyncSettingsAccessor*>(SyncedSettings::instance()->accessor());
		if(accessor)
			delete accessor;
		qDebug() << "Cleaned settings";
	} catch(QException &e) {
		qCritical() << "Failed to clean settings:" << e.what();
	}
}

void setupAniRemLib()
{
	qRegisterMetaType<AnimeInfo::SeasonMap>("AnimeInfo::SeasonMap");

	QJsonSerializer::registerListConverters<AnimeInfo>();

	qAddPostRoutine(cleanSettings);
}

}

Q_COREAPP_STARTUP_FUNCTION(setupAniRemLib)
