#ifndef WIDGETPRESENTER_H
#define WIDGETPRESENTER_H

#include "ipresenter.h"
#include "messageresult.h"
#include <QWidget>
#include <coreapp.h>

class WidgetPresenter : public IPresenter
{
	Q_DECLARE_TR_FUNCTIONS(WidgetPresenter)

public:
	WidgetPresenter();

	template <typename TPresenter = WidgetPresenter>
	static void registerAsPresenter();
	template <typename TWidget>
	static void registerWidget();
	static void registerWidget(const QMetaObject &widgetType);
	template <typename TControl, typename TWidget>
	static void registerWidgetExplicitly();
	static void registerWidgetExplicitly(const char *controlName, const QMetaObject &widgetType);

	void present(Control *control) override;
	void withdraw(Control *control) override;
	void showMessage(MessageResult *result, const CoreApp::MessageConfig &config) override;

protected:
	virtual QMetaObject findWidgetMetaObject(const QMetaObject *controlMetaObject, bool &ok);
	virtual bool tryPresent(QWidget *widget, QWidget *parent);

	virtual QDialog *createInputDialog(const QString &title, const QString &text, int inputType, const QString &positiveText, const QString &negativeText, const QString &neutralText);
	virtual QVariant extractInputResult(QDialog *inputDialog);

	virtual void extendedShow(QWidget *widget) const;

private:
	QList<QMetaObject> implicitMappings;
	QHash<QByteArray, QMetaObject> explicitMappings;

	QHash<Control*, QWidget*> activeControls;

	static MessageResult::ResultType getResult(int dialogResult);
	static void registerDefaults();
};

// ------------- Generic Implementation -------------

template<typename TPresenter>
void WidgetPresenter::registerAsPresenter()
{
	static_assert(std::is_base_of<WidgetPresenter, TPresenter>::value, "TPresenter must inherit WidgetPresenter!");
	CoreApp::setMainPresenter(new TPresenter());
	registerDefaults();
}

template<typename TWidget>
void WidgetPresenter::registerWidget()
{
	static_assert(std::is_base_of<QWidget, TWidget>::value, "TWidget must inherit QWidget!");
	registerWidget(TWidget::staticMetaObject);
}

template<typename TControl, typename TWidget>
void WidgetPresenter::registerWidgetExplicitly()
{
	static_assert(std::is_base_of<QWidget, TWidget>::value, "TWidget must inherit QWidget!");
	static_assert(std::is_base_of<Control, TControl>::value, "TControl must inherit Control!");
	registerWidgetExplicitly(TControl::staticMetaObject.className(), TWidget::staticMetaObject);
}


#endif // WIDGETPRESENTER_H
