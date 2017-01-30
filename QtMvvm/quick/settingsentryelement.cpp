#include "settingsentryelement.h"
#include <QDebug>
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

QVariantMap SettingsEntryElement::getEditProperties() const
{
	return editProperties;
}

bool SettingsEntryElement::returnFalse() const
{
	return false;
}

void SettingsEntryElement::showInputDialog(bool show)
{
	if(show) {
		CoreMessage::getInput(title + tr(":"), QString(), inputType.constData(), [=](QVariant value) {
			if(value.isValid())
				setSettingsValue(value);
		}, settingsValue(), editProperties);
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
