#ifndef ANIREMSETTINGSVIEWMODEL_H
#define ANIREMSETTINGSVIEWMODEL_H

#include "datasyncsettingsviewmodel.h"

class AniremSettingsViewModel : public DataSyncSettingsViewModel
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit AniremSettingsViewModel(QObject *parent = nullptr);

	void saveValue(const QString &key, const QVariant &value) override;
	void resetValue(const QString &key) override;
};

#endif // ANIREMSETTINGSVIEWMODEL_H
