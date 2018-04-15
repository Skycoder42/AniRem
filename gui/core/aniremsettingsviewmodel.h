#ifndef ANIREMSETTINGSVIEWMODEL_H
#define ANIREMSETTINGSVIEWMODEL_H

#include "datasyncsettingsviewmodel.h"
#include "imageloader.h"

class AniremSettingsViewModel : public DataSyncSettingsViewModel
{
	Q_OBJECT

	QTMVVM_INJECT_PROP(ImageLoader*, imageLoader, _loader)

public:
	Q_INVOKABLE explicit AniremSettingsViewModel(QObject *parent = nullptr);

	void saveValue(const QString &key, const QVariant &value) override;
	void resetValue(const QString &key) override;

public slots:
	void callAction(const QString &key, const QVariantMap &parameters) override;

private:
	ImageLoader *_loader;
};

#endif // ANIREMSETTINGSVIEWMODEL_H
