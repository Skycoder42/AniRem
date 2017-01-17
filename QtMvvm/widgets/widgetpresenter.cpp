#include "ipresentingwidget.h"
#include "widgetmessageresult.h"
#include "widgetpresenter.h"

#include <QDebug>
#include <QMainWindow>
#include <QDialog>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QMessageBox>
#include <QInputDialog>
#include <QPushButton>
#include <QApplication>

WidgetPresenter::WidgetPresenter() :
	implicitMappings(),
	explicitMappings(),
	activeControls()
{}

void WidgetPresenter::registerWidget(const QMetaObject &widgetType)
{
	auto presenter = static_cast<WidgetPresenter*>(CoreApp::instance()->presenter());
	if(!presenter) {
		presenter = new WidgetPresenter();
		CoreApp::setMainPresenter(presenter);
	}
	presenter->implicitMappings.append(widgetType);
}

void WidgetPresenter::registerWidgetExplicitly(const char *controlName, const QMetaObject &widgetType)
{
	auto presenter = static_cast<WidgetPresenter*>(CoreApp::instance()->presenter());
	if(!presenter) {
		presenter = new WidgetPresenter();
		CoreApp::setMainPresenter(presenter);
	}
	presenter->explicitMappings.insert(controlName, widgetType);
}

bool WidgetPresenter::present(Control *control)
{
	auto active = activeControls.value(control);
	if(active) {
		active->show();
		active->raise();
		active->activateWindow();
		return true;
	}

	auto ok = false;
	auto widgetMetaObject = findWidgetMetaObject(control->metaObject(), ok);
	if(ok) {
		auto parentControl = control->parentControl();
		auto parentWidget = parentControl ? activeControls.value(parentControl, nullptr) : nullptr;

		auto widget = qobject_cast<QWidget*>(widgetMetaObject.newInstance(Q_ARG(Control *, control),
																		  Q_ARG(QWidget*, parentWidget)));
		if(!widget) {
			qCritical() << "Failed to create widget of type"
						<< widgetMetaObject.className()
						<< "(did you mark the constructor as Q_INVOKABLE? Required signature: \"Contructor(Control *, QWidget*);\")";
			return false;
		}

		auto presented = false;
		auto tPresenter = dynamic_cast<IPresentingWidget*>(parentWidget);
		if(tPresenter)
			presented = tPresenter->tryPresent(widget);
		if(!presented)
			presented = tryPresent(widget, parentWidget);

		if(presented) {
			activeControls.insert(control, widget);
			widget->setAttribute(Qt::WA_DeleteOnClose);
			QObject::connect(widget, &QWidget::destroyed, [=](){
				activeControls.remove(control);
				control->onClose();
			});
			control->onShow();
		} else
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
	auto widget = activeControls.value(control);
	if(widget) {
		widget->close();
		return true;
	} else
		return false;
}

MessageResult *WidgetPresenter::showMessage(IPresenter::MessageType type, const QString &title, const QString &text, const QString &positiveAction, const QString &negativeAction, const QString &neutralAction, int inputType)
{
	QDialog *dialog = nullptr;
	if(type == Input)
		dialog = createInputDialog(title, text, inputType, positiveAction, negativeAction, neutralAction);
	else {
		auto msgBox = new QMessageBox();
		if(title.isEmpty())
			msgBox->setText(text);
		else {
			msgBox->setText(QStringLiteral("<b>%1</b>").arg(title));
			msgBox->setInformativeText(text);
		}

		QPushButton *aBtn = nullptr;
		QPushButton *rBtn = nullptr;
		QPushButton *nBtn = nullptr;
		if(!positiveAction.isNull())
			aBtn = msgBox->addButton(positiveAction, QMessageBox::AcceptRole);
		if(!negativeAction.isNull())
			rBtn = msgBox->addButton(negativeAction, QMessageBox::RejectRole);
		if(!neutralAction.isNull())
			nBtn = msgBox->addButton(neutralAction, QMessageBox::DestructiveRole);

		//default button
		if(aBtn)
			msgBox->setDefaultButton(aBtn);
		else if(rBtn)
			msgBox->setDefaultButton(rBtn);
		else if(nBtn)
			msgBox->setDefaultButton(nBtn);

		//escape button
		if(rBtn)
			msgBox->setEscapeButton(rBtn);
		else if(aBtn)
			msgBox->setEscapeButton(aBtn);
		else if(nBtn)
			msgBox->setEscapeButton(nBtn);

		switch (type) {
		case Information:
			msgBox->setIcon(QMessageBox::Information);
			msgBox->setWindowTitle(WidgetMessageResult::tr("Information"));
			break;
		case Question:
			msgBox->setIcon(QMessageBox::Question);
			msgBox->setWindowTitle(WidgetMessageResult::tr("Question"));
			break;
		case Warning:
			msgBox->setIcon(QMessageBox::Warning);
			msgBox->setWindowTitle(WidgetMessageResult::tr("Warning"));
			break;
		case Critical:
			msgBox->setIcon(QMessageBox::Critical);
			msgBox->setWindowTitle(WidgetMessageResult::tr("Error"));
			break;
		default:
			Q_UNREACHABLE();
			break;
		}

		dialog = msgBox;
	}

	if(dialog) {
		//TODO dialog->setParent(QApplication::activeWindow());
		dialog->setAttribute(Qt::WA_DeleteOnClose);

		//"dialog master" stuff
		Qt::WindowFlags flags = Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint;
		dialog->setSizeGripEnabled(false);
		if(dialog->parentWidget()) {
			dialog->setWindowModality(Qt::WindowModal);
			flags |= Qt::Sheet;
		} else {
			dialog->setWindowModality(Qt::ApplicationModal);
			flags |= Qt::Dialog;
		}
		dialog->setWindowFlags(flags);

		auto result = new WidgetMessageResult(dialog);
		if(type == Input) {
			QObject::connect(result, &WidgetMessageResult::positiveAction, result, [this, dialog, result](){
				result->setResult(extractInputResult(dialog));
			});
		}
		dialog->open();
		return result;
	} else
		return nullptr;
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

bool WidgetPresenter::tryPresent(QWidget *widget, QWidget *parent)
{
	auto metaObject = widget->metaObject();
	if(metaObject->inherits(&QDialog::staticMetaObject)) {
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

	widget->show();
	return true;
}

QDialog *WidgetPresenter::createInputDialog(const QString &title, const QString &text, int inputType, const QString &positiveText, const QString &negativeText, const QString &neutralText)
{
	Q_UNUSED(neutralText);

	QInputDialog::InputMode mode;
	switch (inputType) {
	case QMetaType::QString:
		mode = QInputDialog::TextInput;
		break;
	case QMetaType::Int:
		mode = QInputDialog::TextInput;
		break;
	case QMetaType::Double:
		mode = QInputDialog::TextInput;
		break;
	default:
		return nullptr;
	}

	auto dialog = new QInputDialog();
	dialog->setInputMode(mode);
	dialog->setWindowTitle(title);
	dialog->setLabelText(text);
	dialog->setOkButtonText(positiveText);
	dialog->setCancelButtonText(negativeText);

	return dialog;
}

QVariant WidgetPresenter::extractInputResult(QDialog *inputDialog)
{
	auto dialog = qobject_cast<QInputDialog*>(inputDialog);
	if(dialog) {
		switch (dialog->inputMode()) {
		case QInputDialog::TextInput:
			return dialog->textValue();
		case QInputDialog::IntInput:
			return dialog->intValue();
		case QInputDialog::DoubleInput:
			return dialog->doubleValue();
		default:
			Q_UNREACHABLE();
			break;
		}
	}

	return QVariant();
}
