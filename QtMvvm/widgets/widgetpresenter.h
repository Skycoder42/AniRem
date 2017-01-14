#ifndef WIDGETPRESENTER_H
#define WIDGETPRESENTER_H

#include "ipresenter.h"
#include <QWidget>

class WidgetPresenter : public IPresenter
{
public:
	WidgetPresenter();

	template <typename T>
	static void registerWidget();
	static void registerWidget(const QMetaObject &widgetType);
	template <typename TControl, typename TWidget>
	static void registerWidgetExplicitly();
	static void registerWidgetExplicitly(const char *controlName, const QMetaObject &widgetType);

	bool present(Control *control) override;
	bool withdraw(Control *control) override;

protected:
	virtual QMetaObject findWidgetMetaObject(const QMetaObject *controlMetaObject, bool &ok);
	virtual bool tryPresent(QWidget *widget, QWidget *parent, bool &makeNewRoot);

private:
	QList<QMetaObject> implicitMappings;
	QHash<QByteArray, QMetaObject> explicitMappings;

	QHash<Control*, QWidget*> activeControls;
	QWidget *currentRoot;
};
// ------------- Generic Implementation -------------

template<typename T>
void WidgetPresenter::registerWidget()
{
	static_assert(std::is_base_of<QWidget, T>::value, "T must inherit QWidget!");
	registerWidget(T::staticMetaObject);
}

template<typename TControl, typename TWidget>
void WidgetPresenter::registerWidgetExplicitly()
{
	static_assert(std::is_base_of<QWidget, TWidget>::value, "TWidget must inherit QWidget!");
	static_assert(std::is_base_of<Control, TControl>::value, "TControl must inherit Control!");
	registerWidgetExplicitly(TControl::staticMetaObject.className(), TWidget::staticMetaObject);
}


#endif // WIDGETPRESENTER_H
