#ifndef SETTINGSUIBUILDER_H
#define SETTINGSUIBUILDER_H

#include "inputviewfactory.h"
#include "multifilterproxymodel.h"
#include "settingsentryelement.h"
#include "settingsoverelement.h"

#include <QObject>
#include <QQuickItem>
#include <objectlistmodel.h>
#include <settingscontrol.h>

class SettingsUiBuilder : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QQuickItem* buildView MEMBER _buildView NOTIFY buildViewChanged)
	Q_PROPERTY(SettingsControl* control MEMBER _control NOTIFY controlChanged)
	Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)

public:
	explicit SettingsUiBuilder(QObject *parent = nullptr);

	QString filterText() const;

public slots:
	void loadSection(const SettingsSection &section);

	void restoreDefaults();
	void setFilterText(QString filterText);

signals:
	void initActions(bool allowSearch, bool allowRestore);
	void createView(bool isOverview, QAbstractItemModel *model, bool showSections);

	void buildViewChanged(QQuickItem* buildView);
	void controlChanged(SettingsControl* control);

	void filterTextChanged(QString filterText);

private slots:
	void startBuildUi();

private:
	QQuickItem* _buildView;
	SettingsControl *_control;
	QString _filterText;

	GenericListModel<SettingsOverElement> *_rootModel;
	MultiFilterProxyModel *_rootFilter;
	GenericListModel<SettingsEntryElement> *_currentEntryModel;
	MultiFilterProxyModel *_currentEntryFilter;

	static QUrl svgEscape(QUrl url);
};

#endif // SETTINGSUIBUILDER_H
