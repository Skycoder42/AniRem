#ifndef SETTINGSUIBUILDER_H
#define SETTINGSUIBUILDER_H

#include <QObject>
#include <QQuickItem>
#include <objectlistmodel.h>
#include <settingscontrol.h>

class SettingsUiBuilder : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QQuickItem* buildView MEMBER _buildView NOTIFY buildViewChanged)
	Q_PROPERTY(SettingsControl* control MEMBER _control NOTIFY controlChanged)

public:
	explicit SettingsUiBuilder(QObject *parent = nullptr);

public slots:
	void loadSection(const SettingsSection &section);

signals:
	void initActions(bool allowSearch, bool allowRestore);
	void createView(bool isOverview, ObjectListModel *model, bool showSections);

	void buildViewChanged(QQuickItem* buildView);
	void controlChanged(SettingsControl* control);

private slots:
	void startBuildUi();

private:
	QQuickItem* _buildView;
	SettingsControl *_control;

	static QUrl svgEscape(QUrl url);
};

#endif // SETTINGSUIBUILDER_H
