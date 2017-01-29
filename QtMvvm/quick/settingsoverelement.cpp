#include "settingsoverelement.h"

SettingsOverElement::SettingsOverElement(QObject *parent) : QObject(parent)
{

}

SettingsSection SettingsOverElement::getSettingsSection() const
{
    return settingsSection;
}

void SettingsOverElement::setSettingsSection(const SettingsSection &value)
{
    settingsSection = value;
}
