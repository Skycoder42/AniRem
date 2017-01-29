#include "settingsdelegatefactory.h"
#include <QMetaType>

SettingsDelegateFactory::~SettingsDelegateFactory() {}

QUrl SettingsDelegateFactory::delegateUrl(const QString &type)
{
	if(type == QStringLiteral("bool"))
		return QStringLiteral("qrc:/qtmvvm/qml/settings/BoolDelegate.qml");
	else
		return QStringLiteral("qrc:/qtmvvm/qml/settings/MsgDelegate.qml");
}

int SettingsDelegateFactory::metaTypeId(const QString &type)
{
	if(type == QStringLiteral("bool"))
		return QMetaType::Bool;
	else if(type == QStringLiteral("string"))
		return QMetaType::QString;
	else if(type == QStringLiteral("int"))
		return QMetaType::Int;
	else if(type == QStringLiteral("double"))
		return QMetaType::Double;
	else//TODO add list selector and more
		return QMetaType::UnknownType;
}
