#include "inputviewfactory.h"
#include <QMetaType>

InputViewFactory::~InputViewFactory() {}

int InputViewFactory::metaTypeId(const QByteArray &type)
{
	if(type == "string")
		return QMetaType::QString;
	else
		return QMetaType::type(type);
}

QUrl InputViewFactory::getInput(const QByteArray &type)
{
	if(type == "string" || type == "QString")
		return QStringLiteral("qrc:/qtmvvm/qml/inputs/TextField.qml");
	else if(type == "int")
		return QStringLiteral("qrc:/qtmvvm/qml/inputs/SpinBox.qml");
	else if(type == "double")
		return QStringLiteral("qrc:/qtmvvm/qml/inputs/DoubleSpinBox.qml");
	else//TODO add list selector and more
		return QUrl();
}

QUrl InputViewFactory::getDelegate(const QByteArray &type)
{
	if(type == "bool")
		return QStringLiteral("qrc:/qtmvvm/qml/settings/BoolDelegate.qml");
	else
		return QStringLiteral("qrc:/qtmvvm/qml/settings/MsgDelegate.qml");
}
