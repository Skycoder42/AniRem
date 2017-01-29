#include "inputwidgetfactory.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>

InputWidgetFactory::~InputWidgetFactory() {}

QWidget *InputWidgetFactory::createWidget(const QByteArray &type, QWidget *parent, const QVariantMap &editProperties)
{
	QWidget *widget = nullptr;
	if(type == "bool")
		widget = new QCheckBox(parent);
	else if(type == "string" || type == "QString")
		widget = new QLineEdit(parent);
	else if(type == "int")
		widget = new QSpinBox(parent);
	else if(type == "double")
		widget = new QDoubleSpinBox(parent);
	else//TODO add list selector and more
		return nullptr;

	for(auto it = editProperties.constBegin(); it != editProperties.constEnd(); it++)
		widget->setProperty(it.key().toLatin1().constData(), it.value());
	return widget;
}

QMetaProperty InputWidgetFactory::userProperty(QWidget *widget)
{
	return widget->metaObject()->userProperty();
}
