#include "quickpresenter.h"
#include "settingsentryelement.h"
#include "settingsoverelement.h"
#include "settingsuibuilder.h"
#include <QRegularExpression>
#include <objectlistmodel.h>

SettingsUiBuilder::SettingsUiBuilder(QObject *parent) :
	QObject(parent),
	_buildView(nullptr),
	_control(nullptr)
{
	connect(this, &SettingsUiBuilder::buildViewChanged,
			this, &SettingsUiBuilder::startBuildUi);
	connect(this, &SettingsUiBuilder::controlChanged,
			this, &SettingsUiBuilder::startBuildUi);
}

void SettingsUiBuilder::loadSection(const SettingsSection &section)
{
	auto inputFactory = QuickPresenter::inputViewFactory();
	auto model = new GenericListModel<SettingsEntryElement>(true, this);
	auto rIndex = 0;
	foreach(auto group, section.groups) {
		foreach(auto entry, group.entries) {
			auto element = new SettingsEntryElement(_control);
			element->title = entry.title.remove(QRegularExpression(QStringLiteral("&(?!&)")));
			element->tooltip = entry.tooltip;
			element->delegateUrl = inputFactory->getDelegate(entry.type);
			element->conversionType = inputFactory->metaTypeId(entry.type);
			element->editProperties = entry.properties;
			element->key = entry.key;
			element->defaultValue = entry.defaultValue;
			if(group.entries.size() == 1)
				model->insertObject(rIndex++, element);
			else {
				element->group = group.title;
				model->addObject(element);
			}
		}
	}
	emit createView(false, model, true);
}

void SettingsUiBuilder::startBuildUi()
{
	if(!_buildView || !_control)
		return;

	auto setup = _control->loadSetup();
	emit initActions(setup.allowSearch, setup.allowRestore);

	auto model = new GenericListModel<SettingsOverElement>(true, this);
	auto rIndex = 0;
	auto hasMultiSections = false;
	foreach(auto cat, setup.categories) {
		if(cat.sections.size() == 1) {
			auto element = new SettingsOverElement();
			element->category = tr("Others");
			element->title = cat.title;
			element->icon = svgEscape(cat.icon);
			element->tooltip = cat.tooltip;
			element->settingsSection = cat.sections.first();
			model->addObject(element);
		} else {
			hasMultiSections = true;
			foreach(auto section, cat.sections) {
				auto element = new SettingsOverElement();
				element->category = cat.title;
				element->title = section.title;
				element->icon = svgEscape(section.icon);
				element->tooltip = section.tooltip;
				element->settingsSection = section;
				model->insertObject(rIndex++, element);
			}
		}
	}

	if(model->rowCount() == 1) {
		auto section = model->object(0)->settingsSection;
		loadSection(section);
		model->deleteLater();
	} else
		emit createView(true, model, setup.categories.size() > 1 && hasMultiSections);
}

QUrl SettingsUiBuilder::svgEscape(QUrl url)
{
	if(url.scheme() == QStringLiteral("qrc")) {
		auto path = url.path();
		if(path.endsWith(QStringLiteral(".svg"))) {
			path.chop(4);
			path.prepend(QStringLiteral("image://svg"));
			return path;
		}
	}

	return url;
}
