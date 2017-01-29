#ifndef SETTINGSDELEGATEFACTORY_H
#define SETTINGSDELEGATEFACTORY_H

#include <QUrl>

class SettingsDelegateFactory
{
public:
	virtual ~SettingsDelegateFactory();

	virtual QUrl delegateUrl(const QString &type);
	virtual int metaTypeId(const QString &type);
};

#endif // SETTINGSDELEGATEFACTORY_H
