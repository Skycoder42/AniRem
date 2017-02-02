#ifndef SETTINGSOVERELEMENT_H
#define SETTINGSOVERELEMENT_H

#include <QObject>
#include <QUrl>
#include <settingssetuploader.h>

class SettingsOverElement : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString category MEMBER category NOTIFY categoryChanged)

	Q_PROPERTY(QString title MEMBER title NOTIFY titleChanged)
	Q_PROPERTY(QUrl icon MEMBER icon NOTIFY iconChanged)
	Q_PROPERTY(QString tooltip MEMBER tooltip NOTIFY tooltipChanged)

	Q_PROPERTY(SettingsSection settingsSection READ settingsSection WRITE setSettingsSection NOTIFY settingsSectionChanged)
	Q_PROPERTY(QStringList sectionSearchKeys READ sectionSearchKeys NOTIFY settingsSectionChanged)

public:
	explicit SettingsOverElement(QObject *parent = nullptr);

	QString category;
	QString title;
	QUrl icon;
	QString tooltip;

	SettingsSection settingsSection() const;
	void setSettingsSection(const SettingsSection &section);

	QStringList sectionSearchKeys() const;

signals:
	void categoryChanged();
	void titleChanged();
	void iconChanged();
	void tooltipChanged();
	void settingsSectionChanged();

private:
	SettingsSection section;
	QStringList searchKeys;
};

Q_DECLARE_METATYPE(SettingsSection)

#endif // SETTINGSOVERELEMENT_H
