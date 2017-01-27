#include "settingswidgetfactory.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>

SettingsWidgetFactory::~SettingsWidgetFactory() {}

QWidget *SettingsWidgetFactory::createWidget(const QString &type, QWidget *parent)
{
	if(type == "bool")
		return new QCheckBox(parent);
	else if(type == "string")
		return new QLineEdit(parent);
	else if(type == "int")
		return new QSpinBox(parent);
	else if(type == "double")
		return new QDoubleSpinBox(parent);
	else//TODO add list selector and more
		return nullptr;
}

QMetaProperty SettingsWidgetFactory::userProperty(QWidget *widget)
{
	return widget->metaObject()->userProperty();
}
