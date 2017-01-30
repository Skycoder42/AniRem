#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "widgetpresenter.h"
#include <QGroupBox>
#include <QPushButton>
#include <QScrollArea>
#include <QDebug>
#include <coremessage.h>

#define TAB_CONTENT_NAME "tabContent_371342666"

SettingsDialog::SettingsDialog(Control *mControl, QWidget *parent) :
	QDialog(parent),
	control(static_cast<SettingsControl*>(mControl)),
	ui(new Ui::SettingsDialog),
	delegate(nullptr),
	maxWidthBase(0),
	entryMap(),
	changedEntries()
{
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setAutoDefault(false);
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setAutoDefault(false);
	ui->buttonBox->button(QDialogButtonBox::Apply)->setAutoDefault(false);
	ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->setAutoDefault(false);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);

	if(parentWidget()) {
		setWindowModality(Qt::WindowModal);
		setWindowFlags(Qt::Sheet | Qt::WindowCloseButtonHint);
	} else {
		setWindowModality(Qt::ApplicationModal);
		setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	}

#ifdef Q_OS_OSX
	auto font = ui->titleLabel->font();
	font.setPointSize(16);
	ui->titleLabel->setFont(font);
#endif

	int listSpacing = calcSpacing(Qt::Vertical);
	delegate = new CategoryItemDelegate(std::bind(&SettingsDialog::updateWidth, this, std::placeholders::_1),
										ui->categoryListWidget->iconSize(),
										qMax(qRound(listSpacing * (2./3.)), 1),
										this);
	ui->categoryListWidget->setItemDelegate(delegate);
	ui->categoryListWidget->setSpacing(qMax(qRound(listSpacing / 3.), 1) - 1);

	int spacing = calcSpacing(Qt::Horizontal);
	ui->contentLayout->setSpacing(spacing);
	ui->categoryLineSpacer->changeSize(spacing,
									   0,
									   QSizePolicy::Fixed,
									   QSizePolicy::Fixed);

	createUi(control->loadSetup("widgets"));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::resetListSize()
{
	int max = ui->categoryListWidget->count();
	if(max <= 1) {
		ui->categoryContentWidget->hide();
		resize(width() - ui->categoryContentWidget->sizeHint().width(), height());
	} else {
		auto width = ui->categoryListWidget->sizeHint().width();
		ui->categoryListWidget->setFixedWidth(width);
		maxWidthBase = width;
	}
}

void SettingsDialog::updateWidth(int width)
{
	if(width > maxWidthBase) {
		maxWidthBase = width;
		QStyle *style = ui->categoryListWidget->style();
		width += style->pixelMetric(QStyle::PM_ScrollBarExtent);
		width += style->pixelMetric(QStyle::PM_LayoutHorizontalSpacing);
		ui->categoryListWidget->setFixedWidth(width);
	}
}

void SettingsDialog::propertyChanged()
{
	auto widget = qobject_cast<QWidget*>(sender());
	if(widget)
		changedEntries.insert(widget);
}

void SettingsDialog::saveValues()
{
	for(auto it = changedEntries.begin(); it != changedEntries.end();) {
		auto widget = *it;
		auto info = entryMap.value(widget);
		control->saveValue(info.first.key, info.second.read(widget));
		if(info.second.hasNotifySignal())
			it = changedEntries.erase(it);
		else
			it++;
	}
}

void SettingsDialog::restoreValues()
{
	foreach(auto info, entryMap)
		control->resetValue(info.first.key);
}

void SettingsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	switch(ui->buttonBox->standardButton(button)) {
	case QDialogButtonBox::Ok:
		saveValues();
		accept();
		break;
	case QDialogButtonBox::Cancel:
		reject();
		break;
	case QDialogButtonBox::Apply:
		saveValues();
		break;
	case QDialogButtonBox::RestoreDefaults:
		if(control->canRestoreDefaults()) {
			auto result = CoreMessage::message(control->restoreConfig());
			connect(result, &MessageResult::positiveAction, this, [=](){
				restoreValues();
				accept();
			});
		}
		break;
	default:
		Q_UNREACHABLE();
	}
}

void SettingsDialog::on_filterLineEdit_textChanged(const QString &searchText)
{
	auto regex = QRegularExpression::escape(searchText);
	regex.replace(QStringLiteral("\\*"), QStringLiteral(".*"));
	regex.replace(QStringLiteral("\\?"), QStringLiteral("."));
	searchInDialog(QRegularExpression(regex, QRegularExpression::CaseInsensitiveOption));
}

int SettingsDialog::calcSpacing(Qt::Orientation orientation)
{
	auto baseSize = style()->pixelMetric(orientation == Qt::Horizontal ?
											 QStyle::PM_LayoutHorizontalSpacing :
											 QStyle::PM_LayoutVerticalSpacing);
	if(baseSize < 0)
		baseSize = style()->layoutSpacing(QSizePolicy::DefaultType, QSizePolicy::DefaultType, orientation);
	if(baseSize < 0)
		baseSize = style()->layoutSpacing(QSizePolicy::LineEdit, QSizePolicy::LineEdit, orientation);
	if(baseSize < 0) {
#ifdef Q_OS_OSX
		baseSize = 10;
#else
		baseSize = 6;
#endif
	}

	return baseSize;
}

void SettingsDialog::createUi(const SettingsSetup &setup)
{
	ui->filterLineEdit->setVisible(setup.allowSearch);
	ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->setVisible(setup.allowRestore && control->canRestoreDefaults());

	foreach(auto category, setup.categories)
		createCategory(category);

	resetListSize();
	ui->categoryListWidget->setCurrentRow(0);
}

void SettingsDialog::createCategory(const SettingsCategory &category)
{
	auto item = new QListWidgetItem();
	item->setText(category.title);
	item->setIcon(loadIcon(category.icon));
	item->setToolTip(category.tooltip.isNull() ? category.title : category.tooltip);
	auto tab = new QTabWidget();
	tab->setTabBarAutoHide(true);

	ui->contentStackWidget->addWidget(tab);
	ui->categoryListWidget->addItem(item);

	foreach(auto section, category.sections)
		createSection(section, tab);
}

void SettingsDialog::createSection(const SettingsSection &section, QTabWidget *tabWidget)
{
	auto scrollArea = new QScrollArea();
	scrollArea->setWidgetResizable(true);
	scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setAutoFillBackground(true);
	auto pal = scrollArea->palette();
	pal.setColor(QPalette::Window, tabWidget->palette().color(QPalette::Base));
	scrollArea->setPalette(pal);
	scrollArea->setFrameShape(QFrame::NoFrame);

	auto scrollContent = new QWidget(scrollArea);
	scrollContent->setObjectName(TAB_CONTENT_NAME);
	auto layout = new QFormLayout(scrollContent);
	scrollContent->setLayout(layout);
	scrollArea->setWidget(scrollContent);

	auto index = tabWidget->addTab(scrollArea, loadIcon(section.icon), section.title);
	tabWidget->tabBar()->setTabToolTip(index, section.tooltip.isNull() ? section.title : section.tooltip);

	foreach(auto group, section.groups)
		createGroup(group, scrollContent, layout);
}

void SettingsDialog::createGroup(const SettingsGroup &group, QWidget *contentWidget, QFormLayout *layout)
{
	QWidget *sectionWidget = nullptr;
	QFormLayout *sectionLayout = nullptr;
	if(group.title.isNull()) {
		sectionWidget = contentWidget;
		sectionLayout = layout;
	} else {
		auto groupBox = new QGroupBox(group.title, contentWidget);
		layout->addRow(groupBox);
		sectionWidget = groupBox;
		sectionLayout = new QFormLayout(groupBox);
		groupBox->setLayout(sectionLayout);
	}

	foreach(auto entry, group.entries)
		createEntry(entry, sectionWidget, sectionLayout);
}

void SettingsDialog::createEntry(const SettingsEntry &entry, QWidget *sectionWidget, QFormLayout *layout)
{
	auto widgetFactory = WidgetPresenter::inputWidgetFactory();
	QWidget *content = widgetFactory->createWidget(entry.type, sectionWidget, entry.properties);
	if(!content) {
		qWarning() << "Failed to create settings widget for type" << entry.type;
		return;
	}
	auto property = widgetFactory->userProperty(content);
	if(!property.isValid()) {
		qWarning() << "Failed to get user property for type" << entry.type;
		return;
	}

	property.write(content, control->loadValue(entry.key, entry.defaultValue));
	if(property.hasNotifySignal()) {
		auto changedSlot = metaObject()->method(metaObject()->indexOfSlot("propertyChanged()"));
		connect(content, property.notifySignal(),
				this, changedSlot);
	} else
		changedEntries.insert(content);

	auto label = new QLabel(entry.title + tr(":"), sectionWidget);
	label->setBuddy(content);
	label->setToolTip(entry.tooltip.isNull() ? entry.title : entry.tooltip);
	if(content->toolTip().isNull())
		content->setToolTip(label->toolTip());

	layout->addRow(label, content);
	entryMap.insert(content, {entry, property});
}

void SettingsDialog::searchInDialog(const QRegularExpression &regex)
{
	for(int i = 0, max = ui->categoryListWidget->count(); i < max; ++i) {
		auto item = ui->categoryListWidget->item(i);
		auto tab = static_cast<QTabWidget*>(ui->contentStackWidget->widget(i));

		if(searchInCategory(regex, tab) ||
		   regex.match(item->text()).hasMatch()) {
			item->setHidden(false);

			if(ui->categoryListWidget->currentRow() == -1)
				ui->categoryListWidget->setCurrentRow(i);
		} else {
			item->setHidden(true);

			if(ui->categoryListWidget->currentRow() == i) {
				auto found = false;
				for(int j = 0; j < max; j++) {
					if(!ui->categoryListWidget->item(j)->isHidden()){
						ui->categoryListWidget->setCurrentRow(j);
						found = true;
						break;
					}
				}
				if(!found) {
					ui->categoryListWidget->setCurrentRow(-1);
					ui->contentStackWidget->setCurrentIndex(max);
				}
			}
		}
	}
}

bool SettingsDialog::searchInCategory(const QRegularExpression &regex, QTabWidget *tab)
{
	auto someFound = false;

	for(int i = 0, max = tab->count(); i < max; ++i) {
		if(searchInSection(regex, tab->widget(i)->findChild<QWidget*>(TAB_CONTENT_NAME)) ||
		   regex.match(tab->tabText(i)).hasMatch()){
			tab->setTabEnabled(i, true);
			someFound = true;
		} else
			tab->setTabEnabled(i, false);
	}

	return someFound;
}

bool SettingsDialog::searchInSection(const QRegularExpression &regex, QWidget *contentWidget)
{
	auto someFound = false;

	auto layout = static_cast<QFormLayout*>(contentWidget->layout());
	for(int i = 0, max = layout->rowCount(); i < max; ++i) {
		auto spanItem = layout->itemAt(i, QFormLayout::SpanningRole);
		if(spanItem) {
			auto group = static_cast<QGroupBox*>(spanItem->widget());
			someFound |= searchInGroup(regex, group) ||
						 regex.match(group->title()).hasMatch();
		} else {
			auto label = static_cast<QLabel*>(layout->itemAt(i, QFormLayout::LabelRole)->widget());
			auto content = layout->itemAt(i, QFormLayout::FieldRole)->widget();
			someFound |= searchInEntry(regex, label, content);
		}
	}

	return someFound;
}

bool SettingsDialog::searchInGroup(const QRegularExpression &regex, QGroupBox *groupWidget)
{
	auto someFound = false;

	auto layout = static_cast<QFormLayout*>(groupWidget->layout());
	for(int i = 0, max = layout->rowCount(); i < max; ++i) {
		auto label = static_cast<QLabel*>(layout->itemAt(i, QFormLayout::LabelRole)->widget());
		auto content = layout->itemAt(i, QFormLayout::FieldRole)->widget();
		someFound |= searchInEntry(regex, label, content);
	}

	return someFound;
}

bool SettingsDialog::searchInEntry(const QRegularExpression &regex, QLabel *label, QWidget *content)
{
	if(regex.pattern().isEmpty()) {
		label->setStyleSheet(QString());
		return false;
	}
	auto keys = entryMap.value(content).first.searchKeys;
	keys.append(label->text());
	foreach(auto key, keys) {
		if(regex.match(key).hasMatch()) {
			label->setStyleSheet(QStringLiteral("QLabel {"
												"    background-color: rgba(19,232,51,0.4);"
												"    border: 1px solid rgba(19,196,45,0.8);"
												"    border-radius: 4px;"
												"}"));
			return true;
		}
	}
	label->setStyleSheet(QString());
	return false;
}

QIcon SettingsDialog::loadIcon(const QUrl &icon)
{
	if(icon.scheme() == QStringLiteral("qrc"))
		return QIcon(QLatin1Char(':') + icon.path());
	else
		return QIcon(icon.toLocalFile());
}



CategoryItemDelegate::CategoryItemDelegate(std::function<void (int)> updateFunc, const QSize &iconSize, int layoutSpacing, QObject *parent) :
	QStyledItemDelegate(parent),
	iconSize(),
	updateFunc(updateFunc)
{
	this->iconSize = iconSize + QSize(0, layoutSpacing);
}

QSize CategoryItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	updateFunc(size.width());
	return size.expandedTo(iconSize);
}
