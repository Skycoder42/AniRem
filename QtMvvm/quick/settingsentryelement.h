#ifndef SETTINGSENTRYELEMENT_H
#define SETTINGSENTRYELEMENT_H

#include <QObject>

class SettingsEntryElement : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString group MEMBER group NOTIFY groupChanged)

	Q_PROPERTY(QString title MEMBER title NOTIFY titleChanged)
	Q_PROPERTY(QString tooltip MEMBER tooltip NOTIFY tooltipChanged)

public:
	explicit SettingsEntryElement(QObject *parent = nullptr);

	QString group;
	QString title;
	QString tooltip;

signals:
	void groupChanged();
	void titleChanged();
	void tooltipChanged();
};

#endif // SETTINGSENTRYELEMENT_H
