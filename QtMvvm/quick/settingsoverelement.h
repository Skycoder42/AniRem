#ifndef SETTINGSOVERELEMENT_H
#define SETTINGSOVERELEMENT_H

#include <QObject>
#include <QUrl>

class SettingsOverElement : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString category MEMBER category NOTIFY categoryChanged)

	Q_PROPERTY(QString title MEMBER title NOTIFY titleChanged)
	Q_PROPERTY(QUrl icon MEMBER icon NOTIFY iconChanged)
	Q_PROPERTY(QString tooltip MEMBER tooltip NOTIFY tooltipChanged)

public:
	explicit SettingsOverElement(QObject *parent = nullptr);

	QString category;
	QString title;
	QUrl icon;
	QString tooltip;

signals:
	void categoryChanged();
	void titleChanged();
	void iconChanged();
	void tooltipChanged();
};

#endif // SETTINGSOVERELEMENT_H
