#ifndef IPRESENTINGWIDGET_H
#define IPRESENTINGWIDGET_H

#include <QWidget>


class IPresentingWidget
{
public:
	inline virtual ~IPresentingWidget() {}

	virtual bool tryPresent(QWidget *widget) = 0;
};

#endif // IPRESENTINGWIDGET_H
