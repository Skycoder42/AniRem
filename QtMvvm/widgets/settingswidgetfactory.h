#ifndef SETTINGSWIDGETFACTORY_H
#define SETTINGSWIDGETFACTORY_H

#include <QMetaProperty>
#include <QWidget>

class SettingsWidgetFactory
{
public:
	virtual ~SettingsWidgetFactory();

	virtual QWidget *createWidget(const QString &type, QWidget *parent);
	virtual QMetaProperty userProperty(QWidget *widget);
};

#endif // SETTINGSWIDGETFACTORY_H
