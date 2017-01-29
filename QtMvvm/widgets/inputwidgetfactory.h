#ifndef INPUTWIDGETFACTORY_H
#define INPUTWIDGETFACTORY_H

#include <QMetaProperty>
#include <QWidget>

class InputWidgetFactory
{
public:
	virtual ~InputWidgetFactory();

	virtual QWidget *createWidget(const QByteArray &type, QWidget *parent, const QVariantMap &editProperties);
	virtual QMetaProperty userProperty(QWidget *widget);
};

#endif // INPUTWIDGETFACTORY_H
