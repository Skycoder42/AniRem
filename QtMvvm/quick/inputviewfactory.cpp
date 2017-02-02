#include "inputviewfactory.h"
#include <QMetaType>

InputViewFactory::~InputViewFactory() {}

int InputViewFactory::metaTypeId(const QByteArray &type, const QVariantMap &properties)
{
	if(type == "string")
		return QMetaType::QString;
	else if(type == "list")
		return metaTypeId(properties.value("_list_data", "string").toByteArray(), properties);
	else
		return QMetaType::type(type);
}

QUrl InputViewFactory::getInput(const QByteArray &type, const QVariantMap &)
{
	if(type == "string" || type == "QString")
		return QStringLiteral("qrc:/qtmvvm/qml/inputs/TextField.qml");
	else if(type == "int")
		return QStringLiteral("qrc:/qtmvvm/qml/inputs/SpinBox.qml");
	else if(type == "double")
		return QStringLiteral("qrc:/qtmvvm/qml/inputs/DoubleSpinBox.qml");
	else if(type == "list")
		return QStringLiteral("qrc:/qtmvvm/qml/inputs/ListEdit.qml");
	else
		return QUrl();
}

QUrl InputViewFactory::getDelegate(const QByteArray &type, const QVariantMap &)
{
	if(type == "bool")
		return QStringLiteral("qrc:/qtmvvm/qml/settings/BoolDelegate.qml");
	else
		return QStringLiteral("qrc:/qtmvvm/qml/settings/MsgDelegate.qml");
}
