#include "ipresentingwidget.h"
#include "widgetpresenter.h"

#include <coreapp.h>
#include <QDebug>
#include <QMainWindow>
#include <QDialog>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QMdiArea>

WidgetPresenter::WidgetPresenter() :
	implicitMappings(),
	explicitMappings(),
	activeControls(),
	currentRoot(nullptr)
{}

void WidgetPresenter::registerWidget(const QMetaObject &widgetType)
{
	Q_ASSERT(dynamic_cast<WidgetPresenter*>(CoreApp::instance()->presenter()));
	auto presenter = static_cast<WidgetPresenter*>(CoreApp::instance()->presenter());
	presenter->implicitMappings.append(widgetType);
}

void WidgetPresenter::registerWidgetExplicitly(const char *controlName, const QMetaObject &widgetType)
{
	Q_ASSERT(dynamic_cast<WidgetPresenter*>(CoreApp::instance()->presenter()));
	auto presenter = static_cast<WidgetPresenter*>(CoreApp::instance()->presenter());
	presenter->explicitMappings.insert(controlName, widgetType);
}

bool WidgetPresenter::present(Control *control)
{
	auto ok = false;
	auto widgetMetaObject = findWidgetMetaObject(control->metaObject(), ok);
	if(ok && widgetMetaObject.className()) {//TODO test
		auto widget = qobject_cast<QWidget*>(widgetMetaObject.newInstance(Q_ARG(QWidget*, currentRoot)));
		if(!widget) {
			qCritical() << "Failed to create widget of type"
						<< widgetMetaObject.className()
						<< "(did you mark the constructor as Q_INVOKABLE?)";
			return false;
		}

		widget->setAttribute(Qt::WA_DeleteOnClose, true);
		if(!QMetaObject::invokeMethod(widget, "setControl", Q_ARG(Control*, control))) {
			qCritical() << "Failed to pass control to widget of type"
						<< widgetMetaObject.className()
						<< "(requires slot: \"void setControl(Control*)\")";
			widget->deleteLater();
			return false;
		}

		auto presented = false;
		auto tPresenter = dynamic_cast<IPresentingWidget*>(widget);
		if(tPresenter) {
			auto newRoot = false;
			presented = tPresenter->tryPresent(widget, newRoot);
			if(newRoot)
				currentRoot = widget;
		}

		if(!presented) {
			auto newRoot = false;
			presented = tryPresent(widget, currentRoot, newRoot);
			if(newRoot)
				currentRoot = widget;
		}

		if(presented)
			activeControls.insert(control, widget);
		else
			widget->deleteLater();
		return presented;
	} else {
		qCritical() << "Unable to find widget for control of type"
					<< control->metaObject()->className();
		return false;
	}
}

bool WidgetPresenter::withdraw(Control *control)
{
	auto widget = activeControls.take(control);
	if(widget) {
		if(currentRoot == widget)
			currentRoot = widget->parentWidget();
		widget->close();
		return true;
	} else
		return false;
}

QMetaObject WidgetPresenter::findWidgetMetaObject(const QMetaObject *controlMetaObject, bool &ok)
{
	ok = true;
	QByteArray cName = controlMetaObject->className();
	if(explicitMappings.contains(cName))
		return explicitMappings.value(cName);
	else {
		auto lIndex = cName.lastIndexOf("Control");
		if(lIndex > 0)
			cName.truncate(lIndex);
		foreach(auto metaObject, implicitMappings) {
			QByteArray wName = metaObject.className();
			if(wName.startsWith(cName))
				return metaObject;
		}
	}

	ok = false;
	return QMetaObject();
}

bool WidgetPresenter::tryPresent(QWidget *widget, QWidget *parent, bool &makeNewRoot)
{
	makeNewRoot = false;

	auto metaObject = widget->metaObject();
	if(metaObject->inherits(&QDialog::staticMetaObject)) {
		makeNewRoot = true;
		qobject_cast<QDialog*>(widget)->open();
		return true;
	} else if(parent && parent->metaObject()->inherits(&QMainWindow::staticMetaObject)) {
		auto mainWindow = qobject_cast<QMainWindow*>(parent);
		if(metaObject->inherits(&QDockWidget::staticMetaObject)) {
			mainWindow->addDockWidget(Qt::LeftDockWidgetArea, qobject_cast<QDockWidget*>(widget));
			return true;
		} else if(metaObject->inherits(&QMdiSubWindow::staticMetaObject) &&
		   mainWindow->centralWidget() &&
		   mainWindow->centralWidget()->metaObject()->inherits(&QMdiArea::staticMetaObject)) {
			auto mdiArea = qobject_cast<QMdiArea*>(mainWindow->centralWidget());
			mdiArea->addSubWindow(widget);
			return true;
		}
	}
	makeNewRoot = true;
	widget->show();
	return true;
}
