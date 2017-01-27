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

void SettingsWidgetFactory::setValue(const QString &type, QWidget *widget, const QVariant &value)
{
	if(type == "bool")
		static_cast<QCheckBox*>(widget)->setChecked(value.toBool());
	else if(type == "string")
		static_cast<QLineEdit*>(widget)->setText(value.toString());
	else if(type == "int")
		static_cast<QSpinBox*>(widget)->setValue(value.toInt());
	else if(type == "double")
		static_cast<QDoubleSpinBox*>(widget)->setValue(value.toDouble());
}

QVariant SettingsWidgetFactory::getValue(const QString &type, QWidget *widget)
{
	if(type == "bool")
		return static_cast<QCheckBox*>(widget)->isChecked();
	else if(type == "string")
		return static_cast<QLineEdit*>(widget)->text();
	else if(type == "int")
		return static_cast<QSpinBox*>(widget)->value();
	else if(type == "double")
		return static_cast<QDoubleSpinBox*>(widget)->value();
	else
		return {};
}
