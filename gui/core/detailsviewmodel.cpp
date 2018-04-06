#include "detailsviewmodel.h"
#include <QtMvvmCore/Messages>

const QString DetailsViewModel::ParamInfo = QStringLiteral("info");
const QString DetailsViewModel::ParamOwner = QStringLiteral("owner");

QVariantHash DetailsViewModel::params(const AnimeInfo &info, QtMvvm::ViewModel *owner)
{
	return {
		{ParamInfo, QVariant::fromValue(info)},
		{ParamOwner, QVariant::fromValue(owner)}
	};
}

DetailsViewModel::DetailsViewModel(QObject *parent) :
	ViewModel(parent),
	_store(new AniremStore(this)),
	_loader(nullptr),
	_animeInfo()
{}

AnimeInfo DetailsViewModel::animeInfo() const
{
	return _animeInfo;
}

QString DetailsViewModel::detailsText() const
{
	auto state = _animeInfo.seasonState();
	if(state.isEmpty()) {
		return tr("<i>Anime has no connections…</i>");
	} else {
		auto string = QStringLiteral("<table>");
		for(auto it = state.constBegin(); it != state.constEnd(); it++) {
			auto row = tr("<tr><td>&nbsp;%2:&nbsp;</td><td>&nbsp;%L1&nbsp;</td></tr>");
			if(it->second)
				row = row.arg(tr("<b><font color=\"#8A0E0E\">%L1 (new)</font></b>"));
			string += row.arg(it->first)
					  .arg(AnimeInfo::typeToString(it.key()));
		}
		return string + QStringLiteral("</table>");
	}
}

ImageLoader *DetailsViewModel::imageLoader() const
{
	return _loader;
}

void DetailsViewModel::setAnimeInfo(const AnimeInfo &animeInfo)
{
	if (_animeInfo == animeInfo)
		return;

	_animeInfo = animeInfo;
	emit animeInfoChanged();
}

void DetailsViewModel::clear()
{
	_animeInfo = {};
	emit animeInfoChanged();
}

void DetailsViewModel::uncheckAnime()
{
	if(_animeInfo && _animeInfo.hasNewSeasons()) {
		auto oldInfo = _animeInfo;
		try {
			_animeInfo.setAllUnchanged();
			_store->save(_animeInfo);
			emit animeInfoChanged();
		} catch(QException &e) {
			_animeInfo = oldInfo;
			qCritical() << "Failed to save entry with id" << _animeInfo.id()
						<< "and error:" << e.what();
			QtMvvm::critical(tr("Adding failed!"),
							 tr("Failed to add anime to underlying data storage."));
		}
	}
}

void DetailsViewModel::onInit(const QVariantHash &params)
{
	if(params.contains(ParamInfo))
		setAnimeInfo(params.value(ParamInfo).value<AnimeInfo>());
	auto owner = params.value(ParamOwner).value<QtMvvm::ViewModel*>();
	if(owner) {
		QMetaObject::invokeMethod(owner, "setDetailsView",
								  Q_ARG(QPointer<DetailsViewModel>, this));
	}
}
