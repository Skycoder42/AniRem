#include "inputwidgetfactory.h"
#include "listcombobox.h"

#include <QCheckBox>
#include <QComboBox>
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
	else if(type == "list")
		widget = createListWidget(parent, editProperties);
	else
		return nullptr;

	for(auto it = editProperties.constBegin(); it != editProperties.constEnd(); it++)
		widget->setProperty(it.key().toLatin1().constData(), it.value());
	return widget;
}

QMetaProperty InputWidgetFactory::userProperty(QWidget *widget)
{
	return widget->metaObject()->userProperty();
}

QWidget *InputWidgetFactory::createListWidget(QWidget *parent, const QVariantMap &editProperties)
{
	auto box = new ListComboBox(parent);
	foreach(auto item, editProperties.value("_list_elements").toList()) {
		if(item.type() == QMetaType::QString)
			box->addItem(item.toString(), item);
		else {
			auto iData = item.toMap();
			box->addItem(iData.value("name").toString(), iData.value("value"));
		}
	}
	return box;
}
