#include "settingsoverelement.h"

SettingsOverElement::SettingsOverElement(QObject *parent) :
	QObject(parent)
{}

SettingsSection SettingsOverElement::settingsSection() const
{
	return section;
}

void SettingsOverElement::setSettingsSection(const SettingsSection &section)
{
	this->section = section;

	searchKeys.clear();
	searchKeys.append(section.title);
	searchKeys.append(section.tooltip);
	foreach(auto group, section.groups) {
		searchKeys.append(group.title);
		foreach(auto entry, group.entries) {
			searchKeys.append(entry.title);
			searchKeys.append(entry.tooltip);
			searchKeys.append(entry.searchKeys);
		}
	}

	emit settingsSectionChanged();
}

QStringList SettingsOverElement::sectionSearchKeys() const
{
	return searchKeys;
}
