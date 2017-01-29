#ifndef SETTINGSENTRYELEMENT_H
#define SETTINGSENTRYELEMENT_H

#include <QObject>
#include <settingscontrol.h>

class SettingsEntryElement : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString group MEMBER group NOTIFY groupChanged)

	Q_PROPERTY(QString title MEMBER title NOTIFY titleChanged)
	Q_PROPERTY(QString tooltip MEMBER tooltip NOTIFY tooltipChanged)
	Q_PROPERTY(QVariant settingsValue READ settingsValue WRITE setSettingsValue NOTIFY settingsValueChanged)

	Q_PROPERTY(QUrl delegateUrl READ getDelegateUrl CONSTANT)
	Q_PROPERTY(QVariantMap delegateProperties READ getDelegateProperties CONSTANT)

	Q_PROPERTY(bool showInputDialog READ returnFalse WRITE showInputDialog)

public:
	explicit SettingsEntryElement(SettingsControl *control, QObject *parent = nullptr);

	QString group;
	QString title;
	QString tooltip;

	QUrl delegateUrl;
	QVariantMap delegateProperties;

	QString key;
	int conversionType;
	QVariant defaultValue;

	QVariant settingsValue() const;

	QUrl getDelegateUrl() const;
	QVariantMap getDelegateProperties() const;

	bool returnFalse() const;

public slots:
	void showInputDialog(bool show);

	void setSettingsValue(QVariant settingsValue);

signals:
	void groupChanged();
	void titleChanged();
	void tooltipChanged();
	void settingsValueChanged(QVariant settingsValue);

private:
	SettingsControl *control;
};

#endif // SETTINGSENTRYELEMENT_H
