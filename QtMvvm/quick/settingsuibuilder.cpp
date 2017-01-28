#include "settingsoverelement.h"
#include "settingsuibuilder.h"
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

void SettingsUiBuilder::startBuildUi()
{
	if(!_buildView || !_control)
		return;

	auto setup = _control->loadSetup();
	emit initActions(setup.allowSearch, setup.allowRestore);

	auto model = new GenericListModel<SettingsOverElement>(true, this);
	foreach(auto cat, setup.categories) {
		foreach(auto section, cat.sections) {
			auto element = new SettingsOverElement();
			element->category = cat.title;
			element->title = section.title;
			element->icon = svgEscape(section.icon);
			element->tooltip = section.tooltip;
			model->addObject(element);
		}
	}
	emit createView(true, model, setup.categories.size() > 1);
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
