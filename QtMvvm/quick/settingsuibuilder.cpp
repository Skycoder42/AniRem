#include "quickpresenter.h"
#include "settingsentryelement.h"
#include "settingsoverelement.h"
#include "settingsuibuilder.h"
#include <QRegularExpression>
#include <objectlistmodel.h>
#include <coremessage.h>

SettingsUiBuilder::SettingsUiBuilder(QObject *parent) :
	QObject(parent),
	_buildView(nullptr),
	_control(nullptr),
	_filterText(),
	_rootModel(nullptr),
	_rootFilter(new MultiFilterProxyModel(this)),
	_currentEntryModel(nullptr),
	_currentEntryFilter(new MultiFilterProxyModel(this))
{
	connect(this, &SettingsUiBuilder::buildViewChanged,
			this, &SettingsUiBuilder::startBuildUi);
	connect(this, &SettingsUiBuilder::controlChanged,
			this, &SettingsUiBuilder::startBuildUi);
}

QString SettingsUiBuilder::filterText() const
{
	return _filterText;
}

void SettingsUiBuilder::loadSection(const SettingsSection &section)
{
	if(_currentEntryModel)
		_currentEntryModel->deleteLater();

	auto inputFactory = QuickPresenter::inputViewFactory();
	_currentEntryModel = new GenericListModel<SettingsEntryElement>(true, this);
	auto rIndex = 0;
	foreach(auto group, section.groups) {
		foreach(auto entry, group.entries) {
			auto element = new SettingsEntryElement(_control);
			element->title = entry.title.remove(QRegularExpression(QStringLiteral("&(?!&)")));
			element->tooltip = entry.tooltip;
			element->searchKeys = entry.searchKeys;
			element->delegateUrl = inputFactory->getDelegate(entry.type, entry.properties);
			element->inputType = entry.type;
			element->conversionType = inputFactory->metaTypeId(entry.type, entry.properties);
			element->editProperties = entry.properties;
			element->key = entry.key;
			element->defaultValue = entry.defaultValue;
			if(group.entries.size() == 1)
				_currentEntryModel->insertObject(rIndex++, element);
			else {
				element->group = group.title;
				_currentEntryModel->addObject(element);
			}
		}
	}

	_currentEntryFilter->clearFilterRoles();
	_currentEntryFilter->setSourceModel(_currentEntryModel);
	_currentEntryFilter->addFilterRole("group");
	_currentEntryFilter->addFilterRole("title");
	_currentEntryFilter->addFilterRole("tooltip");
	_currentEntryFilter->addFilterRole("searchKeys");
	emit createView(false, _currentEntryFilter, true);
}

void SettingsUiBuilder::restoreDefaults()
{
	if(!_control->canRestoreDefaults())
		return;

	auto result = CoreMessage::message(_control->restoreConfig());
	connect(result, &MessageResult::positiveAction, this, [=](){
		if(_rootModel) {
			foreach(auto overElement, _rootModel->objects()) {
				foreach(auto group, overElement->settingsSection().groups) {
					foreach(auto entry, group.entries)
						_control->resetValue(entry.key);
				}
			}
		} else if(_currentEntryModel) {
			foreach(auto entryElement, _currentEntryModel->objects())
				_control->resetValue(entryElement->key);
		}
		_control->close();
	});
}

void SettingsUiBuilder::setFilterText(QString filterText)
{
	if (_filterText == filterText)
		return;

	_filterText = filterText;
	emit filterTextChanged(filterText);

	auto regexString = QRegularExpression::escape(filterText);
	regexString.replace(QStringLiteral("\\*"), QStringLiteral(".*"));
	regexString.replace(QStringLiteral("\\?"), QStringLiteral("."));
	QRegularExpression regex(regexString, QRegularExpression::CaseInsensitiveOption);
	_rootFilter->setFilter(regex);
	_currentEntryFilter->setFilter(regex);
}

void SettingsUiBuilder::startBuildUi()
{
	if(!_buildView || !_control)
		return;

	auto setup = _control->loadSetup("quick");
	emit initActions(setup.allowSearch, setup.allowRestore);

	_rootModel = new GenericListModel<SettingsOverElement>(true, this);
	auto rIndex = 0;
	auto hasMultiSections = false;
	foreach(auto cat, setup.categories) {
		if(cat.sections.size() == 1) {
			auto element = new SettingsOverElement();
			element->category = tr("Others");
			element->title = cat.title;
			element->icon = svgEscape(cat.icon);
			element->tooltip = cat.tooltip;
			element->setSettingsSection(cat.sections.first());
			_rootModel->addObject(element);
		} else {
			hasMultiSections = true;
			foreach(auto section, cat.sections) {
				auto element = new SettingsOverElement();
				element->category = cat.title;
				element->title = section.title;
				element->icon = svgEscape(section.icon);
				element->tooltip = section.tooltip;
				element->setSettingsSection(section);
				_rootModel->insertObject(rIndex++, element);
			}
		}
	}

	if(_rootModel->rowCount() == 1) {
		auto section = _rootModel->object(0)->settingsSection();
		loadSection(section);
		_rootModel->deleteLater();
		_rootModel = nullptr;
	} else {
		_rootFilter->clearFilterRoles();
		_rootFilter->setSourceModel(_rootModel);
		_rootFilter->addFilterRole("category");
		_rootFilter->addFilterRole("title");
		_rootFilter->addFilterRole("tooltip");
		_rootFilter->addFilterRole("sectionSearchKeys");
		emit createView(true, _rootFilter, setup.categories.size() > 1 && hasMultiSections);
	}
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
