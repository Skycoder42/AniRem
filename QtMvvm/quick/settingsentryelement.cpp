#include "settingsentryelement.h"
#include <coremessage.h>

SettingsEntryElement::SettingsEntryElement(SettingsControl *control, QObject *parent) :
	QObject(parent),
	control(control)
{}

QVariant SettingsEntryElement::settingsValue() const
{
	auto res = control->loadValue(key, defaultValue);
	res.convert(conversionType);
	return res;
}

QVariantMap SettingsEntryElement::getDelegateProperties() const
{
	return delegateProperties;
}

bool SettingsEntryElement::returnFalse() const
{
	return false;
}

void SettingsEntryElement::showInputDialog(bool show)
{
	if(show) {
		CoreMessage::getInput(title + tr(":"),
							  QString(),
							  conversionType,
							  [=](QVariant value) {
			setSettingsValue(value);
		});
	}
}

void SettingsEntryElement::setSettingsValue(QVariant settingsValue)
{
	control->saveValue(key, settingsValue);
}

QUrl SettingsEntryElement::getDelegateUrl() const
{
	return delegateUrl;
}
