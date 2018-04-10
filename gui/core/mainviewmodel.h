#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QSortFilterProxyModel>
#include <QtMvvmCore/ViewModel>
#include <QtDataSync/DataStoreModel>
#include <syncedsettings.h>
#include <seasonstatusloader.h>

#include <animeinfo.h>
class DetailsViewModel;

class MainViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT

	Q_PROPERTY(QtDataSync::DataStoreModel* animeModel READ animeModel CONSTANT)
	Q_PROPERTY(QSortFilterProxyModel* sortedModel READ sortedModel CONSTANT)
	Q_PROPERTY(bool reloadingAnimes READ isReloadingAnimes NOTIFY reloadingAnimesChanged)
	Q_PROPERTY(QString filterString READ filterString WRITE setFilterString NOTIFY filterStringChanged)

	QTMVVM_INJECT_PROP(SyncedSettings*, settings, _settings)
	QTMVVM_INJECT_PROP(SeasonStatusLoader*, updater, _updater)

public:
	Q_INVOKABLE explicit MainViewModel(QObject *parent = nullptr);

	QtDataSync::DataStoreModel* animeModel() const;
	QSortFilterProxyModel* sortedModel() const;

	bool isReloadingAnimes() const;

	QString filterString() const;

public slots:
	void reload();
	void showSettings();
	void showSync();
	void showAbout();
	void showCaptcha();

	void uncheckAnime(int id);
	void itemAction(int id);

	void addAnime();
	void addAnimeFromClipboard();
	void showDetails(int id);
	void openUrl(int id);

	void removeAnime(int id);

	void setFilterString(QString filterString);

signals:
	void showStatus(const QString &message);
	void setProgress(int value, int max);

	void reloadingAnimesChanged(bool reloadingAnimes);

	void filterStringChanged(QString filterString);

protected:
	void onInit(const QVariantHash &params) override;

private slots:
	void setDetailsView(QPointer<DetailsViewModel> details);
	void updaterStarted();
	void updaterDone(bool hasUpdates, const QString &error);

private:
	QtDataSync::DataStoreModel *_model;
	QSortFilterProxyModel *_sortModel;
	SyncedSettings *_settings;
	SeasonStatusLoader *_updater;
	QString _filterString;
	bool _loading;
	bool _showNoChanges;
	int _migrationMax;

	QPointer<DetailsViewModel> _currentDetails;

	AnimeInfo infoFromId(int id) const;
};

#endif // MAINVIEWMODEL_H
