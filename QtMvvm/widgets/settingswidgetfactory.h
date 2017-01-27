#ifndef SETTINGSWIDGETFACTORY_H
#define SETTINGSWIDGETFACTORY_H

#include <QWidget>

class SettingsWidgetFactory
{
public:
	virtual ~SettingsWidgetFactory();

	virtual QWidget *createWidget(const QString &type, QWidget *parent);
	virtual void setValue(const QString &type, QWidget *widget, const QVariant &value);
	virtual QVariant getValue(const QString &type, QWidget *widget);
};

#endif // SETTINGSWIDGETFACTORY_H
